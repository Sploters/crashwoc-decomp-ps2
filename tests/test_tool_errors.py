"""Unit tests for how failed toolchain runs are reported.

GNU ld prints context first ("x.o: in function `main':") and the actual reason
on the next line, so both the image link (link_image.tool_error) and the
per-function link (declib.verify.ToolError) must keep more than one line.
Pure Python; no toolchain, no game files.
"""
import subprocess
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from declib.verify import ToolError, _run_tool
from link_image import tool_error

LD_STDERR = (
    "ld: build/pal103/matching/game/main.o: in function `main':\n"
    "(.text+0x4e84): undefined reference to `DrawDeb3'\n"
)


def _failed(stderr):
    return subprocess.CalledProcessError(1, ["ld"], "", stderr)


class TestToolError(unittest.TestCase):
    def test_reason_line_is_kept(self):
        msg = tool_error(_failed(LD_STDERR), "ld failed")
        self.assertIn("in function `main'", msg)
        self.assertIn("undefined reference to `DrawDeb3'", msg)

    def test_empty_stderr_uses_default(self):
        self.assertEqual(tool_error(_failed(""), "ld failed"), "ld failed")
        self.assertEqual(tool_error(_failed(None), "ld failed"), "ld failed")

    def test_long_output_is_capped_and_counted(self):
        stderr = "\n".join(f"line {i}" for i in range(10))
        msg = tool_error(_failed(stderr), "ld failed")
        self.assertIn("line 3", msg)
        self.assertNotIn("line 4", msg)
        self.assertIn("+6 more lines", msg)

    def test_blank_lines_are_skipped(self):
        msg = tool_error(_failed("\n\n  first\n\n second \n"), "x")
        self.assertEqual(msg, "first | second")


class TestToolErrorException(unittest.TestCase):
    def test_is_still_a_calledprocesserror(self):
        exc = ToolError(1, ["ld"], "", LD_STDERR)
        self.assertIsInstance(exc, subprocess.CalledProcessError)

    def test_message_carries_stderr(self):
        text = str(ToolError(1, ["ld"], "", LD_STDERR))
        self.assertIn("non-zero exit status 1", text)
        self.assertIn("undefined reference to `DrawDeb3'", text)

    def test_message_without_stderr_is_unchanged(self):
        base = str(subprocess.CalledProcessError(1, ["ld"]))
        self.assertEqual(str(ToolError(1, ["ld"], "", "")), base)

    def test_run_tool_raises_with_stderr(self):
        cmd = [sys.executable, "-c",
               "import sys; sys.stderr.write('boom\\n'); sys.exit(3)"]
        with self.assertRaises(ToolError) as ctx:
            _run_tool(cmd)
        self.assertEqual(ctx.exception.returncode, 3)
        self.assertIn("boom", str(ctx.exception))

    def test_run_tool_returns_on_success(self):
        proc = _run_tool([sys.executable, "-c", "print('ok')"])
        self.assertEqual(proc.stdout.strip(), "ok")


if __name__ == "__main__":
    unittest.main()

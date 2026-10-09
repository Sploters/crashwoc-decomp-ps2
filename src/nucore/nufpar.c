/*
 * Unit: nucore/nufpar
 *
 * Functions:
 *   0x00103978 NuFParGetLine
 *   0x00103ca0 NuFParGetInt
 *   0x00103da8 NuFParGetFloat
 *   0x00103ec0 NuFParGetPos
 *   0x00103ef0 NuFParSetPos
 *   0x00103f30 NuFParPushCom
 *   0x00103f60 NuFParPopCom
 *   0x00103f78 NuFParInterpretWord
 *   0x00104048 NuFParUnGetWord
 *   0x00104058 NuFParGetWord
 *   0x00104140 NuFParClose
 *   0x00104168 NuFParOpen
 *   0x001041e0 NuFParDestroy
 *   0x00104220 NuFParCreate
 *   0x001042b8 NuGetChar
 */

/* Fields accessed by the command-stack and word-position helpers. */
typedef struct NuFPar {
    unsigned char pad00[0x1208];
    int word1208;
    int word_offset;
    int position;
    int buffer_start;
    int buffer_end;
    void *commands[8];
    int command_index;
} NuFPar;

typedef struct NuFParPos {
    int word1208;
    int word_offset;
    int position;
    int buffer_start;
    int buffer_end;
} NuFParPos;

/* The target names this word as _fbss + 0x8. */
extern int D_00633008;


void NuFParGetPos(NuFPar *parser, NuFParPos *pos)
{
    pos->word1208 = parser->word1208;
    pos->word_offset = parser->word_offset;
    pos->position = parser->position;
    pos->buffer_start = parser->buffer_start;
    pos->buffer_end = parser->buffer_end;
}


void NuFParSetPos(NuFPar *parser, NuFParPos *pos)
{
    parser->position = pos->position;
    parser->word_offset = pos->word_offset;
    if (parser->position < parser->buffer_start ||
        parser->position > parser->buffer_end) {
        parser->buffer_end = parser->position - 1;
    }
}


int NuFParPushCom(NuFPar *parser, void *command)
{
    int result;

    if (parser->command_index < 7) {
        parser->command_index++;
        parser->commands[parser->command_index] = command;
        result = parser->command_index;
    } else {
        result = -1;
    }
    return result;
}


void NuFParPopCom(NuFPar *parser)
{
    if (parser->command_index >= 0) {
        parser->command_index--;
    }
}


void NuFParUnGetWord(NuFPar *parser)
{
    parser->word_offset = D_00633008;
}

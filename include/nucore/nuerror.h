#ifndef NUCORE_NUERROR_H
#define NUCORE_NUERROR_H

/* NuErrorProlog(__FILE__, __LINE__)(fmt, ...): the prolog records the source
 * position and returns the function that prints the message. */
typedef void (*NuErrorFunc)(const char *fmt, ...);

void NuErrorFunction(const char *fmt, ...);
void NuWarningFunction(const char *fmt, ...);
void NuDebugMsgFunction(const char *fmt, ...);
NuErrorFunc NuErrorProlog(const char *file, int line);
NuErrorFunc NuWarningProlog(const char *file, int line);
NuErrorFunc NuDebugMsgProlog(const char *file, int line);
void NuAssertMsg(const char *msg, const char *file, int line);

#endif /* NUCORE_NUERROR_H */

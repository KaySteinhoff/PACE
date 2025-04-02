#include <PACEErrorHandling.h>
#if defined(__WIN32)
	#include <windows.h>
#elif defined(__unix__)
	#include <stdio.h>
#endif

static const char *errorStrings[] = {
	"Success",
	"Failure",
	"PACE unitialized",
	"NULL reference",
	"Invalid argument",
	"Invalid operation",
};
PAERRORHANDLER errorHandler = NULL;

void defaultHandler(char *msg, char *title)
{
#if defined(__WIN32)
	MessageBox(NULL, msg, title, MB_OK | MB_ICONERROR);
#elif defined(__unix__)
	printf("\x1b[1;31m[ERROR]\x1b[0m %s\n%s\n\n", title, msg);
#endif
}

PAERRORHANDLER SetErrorHandler(PAERRORHANDLER newHandler)
{
	PAERRORHANDLER old = errorHandler;
	errorHandler = newHandler;
	return old;
}

void ThrowError(char *msg, char *title)
{
	if(!errorHandler)
		errorHandler = defaultHandler;
	errorHandler(msg, title);
}

const char* PACEStringFromErrorCode(unsigned int errorCode)
{
	return errorStrings[errorCode];
}

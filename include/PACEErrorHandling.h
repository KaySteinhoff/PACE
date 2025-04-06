#ifndef PACE_ERROR_MESSAGE_H_
#define PACE_ERROR_MESSAGE_H_

static const unsigned int PACE_ERR_SUCCESS = 0;
static const unsigned int PACE_ERR_FAILURE = 1;
static const unsigned int PACE_ERR_UNINITIALIZED = 2;
static const unsigned int PACE_ERR_NULL_REFERENCE = 3;
static const unsigned int PACE_ERR_INVALID_ARGUMENT = 4;
static const unsigned int PACE_ERR_INVALID_OPERATION = 5;
static const unsigned int PACE_ERR_OUT_OF_MEMORY = 6;

typedef void(*PAERRORHANDLER)(char *msg, char *title);

// Sets the used error handler to the given one and returns the old one
PAERRORHANDLER SetErrorHandler(PAERRORHANDLER newHandler);
void ThrowError(char *msg, char *title);

const char* PACEStringFromErrorCode(unsigned int errorCode);

#endif

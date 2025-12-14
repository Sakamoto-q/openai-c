#ifndef OPENAI_REQUEST_H
#define OPENAI_REQUEST_H

#ifdef _WIN32
    #define snprintf _snprintf
    #define strdup _strdup
#endif

#include "client.h"

typedef struct {
    const char *model;
    const char *input;
} OpenAIRequest;

typedef struct {
    char *memory;
    size_t size;
} MemoryStruct;

int openai_request_to_json(const OpenAIRequest *request, char **json_output);

int openai_response_to_content(const char *response, char **res_json);

int openai_client_chat_request(const OpenAIClient *client, const OpenAIRequest *request, char **response);

#endif // OPENAI_REQUEST_H
#ifndef ESP32_OPENAI_REQUEST_H
#define ESP32_OPENAI_REQUEST_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "esp32-client.h"

typedef struct {
    const char *model;
    const char *input;
} OpenAIRequest;

int openai_request_to_json(const OpenAIRequest *request, char **json_output);

int openai_response_to_content(const char *response, char **res_json);

int openai_client_chat_request(const OpenAIClient *client, const OpenAIRequest *request, char **response);

#ifdef __cplusplus
}
#endif

#endif // ESP32_OPENAI_REQUEST_H

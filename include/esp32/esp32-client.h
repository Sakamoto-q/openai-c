#ifndef ESP32_OPENAI_CLIENT_H
#define ESP32_OPENAI_CLIENT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAIClient {
    size_t token_len;
    size_t authorization_len;
    char *token;
    char *authorization;
} OpenAIClient;

int new_openai_client(OpenAIClient *client, const char *token_str);

void openai_client_free(OpenAIClient *client);

int openai_client_init_authorization(OpenAIClient *client);

#ifdef __cplusplus
}
#endif

#endif // ESP32_OPENAI_CLIENT_H

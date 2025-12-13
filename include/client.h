#ifndef OPENAI_CLIENT_H
#define OPENAI_CLIENT_H

typedef struct OpenAIClient {
    size_t token_len;
    size_t authorization_len;
    char *token;
    char *authorization;
} OpenAIClient;

int new_openai_client(OpenAIClient *client, const char *token_str);

int openai_client_free(OpenAIClient *client);

int openai_client_init_authorization(OpenAIClient *client);

#endif // OPENAI_CLIENT_H
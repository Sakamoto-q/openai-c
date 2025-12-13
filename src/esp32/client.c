#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "error.h"

int new_openai_client(OpenAIClient *client, const char *token) {
    if (client == NULL) {
        return NEW_OPENAI_CLIENT_NULL_CLIENT;
    }

    if (token == NULL || *token == '\0') {
        return NEW_OPENAI_CLIENT_NULL_TOKEN;
    }

    size_t len = strlen(token);
    size_t total_size = len + 1;

    char *new_token = (char *)malloc(total_size);
    if (new_token == NULL) {
        return NEW_OPENAI_CLIENT_MALLOC;
    }

    memcpy(new_token, token, total_size);

    client->token_len = len;
    client->token = new_token;

    client->authorization_len = 0;
    client->authorization = NULL;

    return OPENAI_OK;
}

void openai_client_free(OpenAIClient *client) {
    if (client == NULL) {
        return;
    }

    if (client->token != NULL) {
        free(client->token);
        client->token = NULL;
    }

    if (client->authorization != NULL) {
        free(client->authorization);
        client->authorization = NULL;
    }

    client->token_len = 0;
    client->authorization_len = 0;
}

int openai_client_init_authorization(OpenAIClient *client) {
    if (client == NULL) {
        return OPENAI_CLIENT_INIT_AUTH_NULL_CLIENT;
    }

    if (client->authorization != NULL) {
        return OPENAI_CLIENT_INIT_AUTH_ALREADY_INITIALIZED;
    }

    if (client->token == NULL || *client->token == '\0') {
        return OPENAI_CLIENT_INIT_AUTH_NULL_TOKEN;
    }

    size_t len = strlen(client->token) + 22;
    size_t total_size = len + 1;

    char *new_authorization = (char *)malloc(total_size);
    if (new_authorization == NULL) {
        return OPENAI_CLIENT_INIT_AUTH_MALLOC;
    }

    int ret = snprintf(
        new_authorization,
        total_size,
        "Authorization: Bearer %s",
        client->token
    );

    if (ret < 0 || (size_t)ret >= total_size) {
        free(new_authorization);
        return OPENAI_CLIENT_INIT_AUTH_SNPRINTF;
    }

    client->authorization_len = len;
    client->authorization = new_authorization;

    return OPENAI_OK;
}

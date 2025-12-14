#include <stdio.h>
#include <stdlib.h>

#include <openai-c/openai-c.h>

int main() {
    int error;
    struct OpenAIClient client;
    
    error = new_openai_client(&client, "YOUR-OPENAI-API-KEY");
    if (error != 0) {
        printf("Error: %s\n", openai_error_to_string(error));
        return error;
    }

    error = openai_client_init_authorization(&client);
    if (error != 0) {
        printf("Error: %s\n", openai_error_to_string(error));
        return error;
    }

    char *response = NULL;
    OpenAIRequest request = {
        .model = "gpt-4o-mini",
        .input = "Hello, who are you?",
    };

    error = openai_client_chat_request(&client, &request, &response);
    if (error != 0) {
        printf("Error: %s\n", openai_error_to_string(error));
        return error;
    }

    printf("Respond: %s\n", response);

    error = openai_client_free(&client);
    if (error != 0) {
        printf("Error: %s\n", openai_error_to_string(error));
        return error;
    }

    if (response != NULL) {
        free(response); 
        response = NULL;
    }

    return 0;
}
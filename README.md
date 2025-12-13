> [!WARNING]
> This project requires the following libraries to build:
> - `cmake`
> - `cJSON`
> - `libcurl (curl)`

example

```c
#include <stdio.h>
#include <stdlib.h>
#include <openai-c/openai-c.h>

int main() {
    int error;
    struct OpenAIClient client;
    
    // Create a new OpenAI client with your API key
    error = new_openai_client(&client, "sk-xxxx");
    if (error != 0) {
        printf("Error: %s\n", openai_error_to_string(error));
        return error;
    }
    
    // Initialize the client's authorization
    error = openai_client_init_authorization(&client);
    if (error != 0) {
        printf("Error: %s\n", openai_error_to_string(error));
        return error;
    }
    
    // Prepare the chat request
    char *response = NULL;
    OpenAIRequest request = {
        .model = "gpt-5-nano",
        .input = "hi"
    };
    
    // Send the chat request to OpenAI API
    error = openai_client_chat_request(&client, &request, &response);
    if (error != 0) {
        printf("Error: %s\n", openai_error_to_string(error));
        return error;
    }
    
    // Print the response from the API
    printf("Respond: %s\n", response);
    
    // Clean up the client resources
    error = openai_client_free(&client);
    if (error != 0) {
        printf("Error: %s\n", openai_error_to_string(error));
        return error;
    }
    
    // Free the response buffer
    if (response != NULL) {
        free(response); 
        response = NULL;
    }
    
    return 0;
}
```
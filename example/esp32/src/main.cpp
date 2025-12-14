#include <WiFi.h>
#include <Arduino.h>
#include <esp32-openai-c.h>

const char* ssid     = "YOUR-WIFI-NAME";
const char* password = "YOUR-WIFI-PASSWORD";

const char* openai_api_key = "YOUR-OPENAI-API-KEY";

void init() {
    Serial.begin(115200);
    pinMode(2, OUTPUT);
}

void wifi_connect() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected!");
}

int openai_init(OpenAIClient *client) {
    int error;

    error = new_openai_client(client, openai_api_key);
    if (error != 0) {
        return error;
    }

    error = openai_client_init_authorization(client);
    if (error != 0) {
        return error;
    }

    return 0;
}

int print_openai_chat_request(const OpenAIClient *client, const OpenAIRequest *request) {
    int error;

    analogWrite(2, 255);

    char *response = NULL;
    error = openai_client_chat_request(client, request, &response);
    
    analogWrite(2, 0);
    if (error != 0) {
        return error;
    }

    if (response == NULL) {
        return OPENAI_RESPONSE_TO_CONTENT_NULL_OUTPUT;
    }

    Serial.println(response);

    free(response);

    return 0;
}

void setup() {
    init();
    delay(2000);
    
    wifi_connect();
    
    int error;
    OpenAIClient client;

    error = openai_init(&client);
    if (error != 0) {
        Serial.printf("error: %d\n", error);
        return;
    }
    
    OpenAIRequest request = {
        .model = "gpt-4o-mini",
        .input = "Hello, who are you?",
    };

    error = print_openai_chat_request(&client, &request);
    if (error != 0) {
        Serial.printf("error: %d\n", error);
        return;
    }
    
    openai_client_free(&client);
}

void loop() {
    delay(1000);
}
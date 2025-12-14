# OpenAI-C

A lightweight OpenAI API C client library with support for both Windows and Linux and ESP32 platforms.

---

> [!CAUTION]
> This project was fundamentally developed targeting the ESP32 platform.
>
> **Minimum Hardware Specifications:**
>
> * **RAM :** `45 KiB (46,940 bytes)`
> * **Flash :** `909 KiB (930,893 bytes)`

> [!IMPORTANT]
> The `ESP32` is fundamentally built with `PlatformIO`.
>
> Need to : `PlatformIO`, `ESP32 Board`

> [!IMPORTANT]
> The `Windows/Linux` builds are fundamentally done with `vcpkg`.
>
> Need to: `cmake`, `vcpkg`, `cJSON`, `libcurl (curl)`

---

### ESP32 Build

#### 1. Copy Library to Project

Copy the `openai-c` folder to your PlatformIO project's `lib` directory:

```
your-project/
└── lib/
    └── openai-c/  ← Copy entire folder here
```

#### 2. Configure platformio.ini

Add the following configuration to `platformio.ini`:

```ini
[env:esp32]
board = esp32dev
framework = arduino
platform = espressif32

upload_port = COM3
monitor_port = COM3
monitor_speed = 115200

lib_extra_dirs =
    lib

lib_deps =
    openai-c
    ArduinoJson
```

---

### Windows/Linux Build

#### 1. Run script from installed file

```sh
make install x64-windows
make install x64-linux
...
```

---

### ESP32 Example

```cpp
#include <WiFi.h>
#include <Arduino.h>
#include <openai-c.h>

const char* ssid     = "YOUR WIFI NAME";
const char* password = "YOUR WIFI PASSWORD";

const char* openai_api_key = "your-api-key";

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
```

---

### Windows/Linux Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <openai-c/openai-c.h>

int main() {
    int error;
    struct OpenAIClient client;
    
    error = new_openai_client(&client, "your-api-key");
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
        .input = "Hello, who are you?"
    };
    
    error = openai_client_chat_request(&client, &request, &response);
    if (error != 0) {
        printf("Error: %s\n", openai_error_to_string(error));
        return error;
    }
    
    printf("%s\n", response);
    
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
```
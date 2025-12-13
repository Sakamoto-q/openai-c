# OpenAI-C

A lightweight OpenAI API C client library with support for both desktop and ESP32 platforms.

## Requirements

### Desktop (Windows/Linux)
- `cmake`
- `vcpkg`
- `cJSON`
- `libcurl (curl)`

### ESP32
- `PlatformIO`
- `ESP32 board`
- `USB driver (CP2102 or CH340)`

## Installation

### Desktop Build

#### 1. Run script from installed file

```sh
make install x64-windows
make install x64-linux
...
```

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
lib_extra_dirs = lib
lib_deps =
    openai-c
    ArduinoJson
```

## Usage Examples

### Desktop

```c
#include <stdio.h>
#include <stdlib.h>
#include <openai-c/openai-c.h>

int main() {
    int error;
    struct OpenAIClient client;
    
    error = new_openai_client(&client, "sk-xxxx");
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

### ESP32

```cpp
#include <Arduino.h>
#include <WiFi.h>

extern "C" {
    #include <openai-c.h>
}

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
    
    int error;
    OpenAIClient client;

    error = new_openai_client(&client, "sk-your-api-key");
    if (error != 0) {
        Serial.printf("error: %d\n", error);
        return;
    }

    error = openai_client_init_authorization(&client);
    if (error != 0) {
        Serial.printf("error: %d\n", error);
        return;
    }
    
    OpenAIRequest request = {
        .model = "gpt-4o-mini",
        .input = "Hello, who are you?",
    };
    
    char *response = NULL;
    error = openai_client_chat_request(&client, &request, &response);
    if (error != 0) {
        Serial.printf("error: %d\n", error);
        return;
    }
    
    if (response != NULL) {
        Serial.println(response);
        free(response);
    }
    
    openai_client_free(&client);
}

void loop() {
    delay(1000);
}
```
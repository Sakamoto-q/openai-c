#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#include <cJSON.h>

#include "request.h"
#include "client.h"
#include "error.h"

int openai_request_to_json(const OpenAIRequest *request, char **req_json) {
    if (req_json == NULL) {
        return OPENAI_REQUEST_TO_JSON_NULL_REQ_JSON;
    }

    *req_json = NULL;

    if (request == NULL) {
        return OPENAI_REQUEST_TO_JSON_NULL_REQUEST;
    }

    if (request->model == NULL || request->input == NULL) {
        return OPENAI_REQUEST_TO_JSON_NULL_MODEL_OR_INPUT;
    }

    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return OPENAI_REQUEST_TO_JSON_CREATE_OBJECT;
    }

    if (!cJSON_AddStringToObject(root, "model", request->model)) {
        cJSON_Delete(root);
        return OPENAI_REQUEST_TO_JSON_ADD_MODEL;
    }

    if (!cJSON_AddStringToObject(root, "input", request->input)) {
        cJSON_Delete(root);
        return OPENAI_REQUEST_TO_JSON_ADD_INPUT;
    }

    char *json_string = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    if (json_string == NULL) {
        return OPENAI_REQUEST_TO_JSON_PRINT;
    }

    *req_json = json_string;
    return OPENAI_OK;
}

int openai_response_to_content(const char *response, char **res_json) {
    if (res_json == NULL) {
        return OPENAI_RESPONSE_TO_CONTENT_NULL_RES_JSON;
    }

    *res_json = NULL;

    if (response == NULL) {
        return OPENAI_RESPONSE_TO_CONTENT_NULL_RESPONSE;
    }

    cJSON *root = cJSON_Parse(response);
    if (!root) {
        return OPENAI_RESPONSE_TO_CONTENT_PARSE_JSON;
    }

    cJSON *output = cJSON_GetObjectItem(root, "output");
    if (!cJSON_IsArray(output)) {
        cJSON_Delete(root);
        return OPENAI_RESPONSE_TO_CONTENT_NULL_OUTPUT;
    }

    cJSON *msg = NULL;
    cJSON *item = NULL;
    cJSON_ArrayForEach(item, output) {
        cJSON *type = cJSON_GetObjectItem(item, "type");
        if (cJSON_IsString(type) &&
            strcmp(type->valuestring, "message") == 0) {
            msg = item;
            break;
        }
    }

    if (!msg) {
        cJSON_Delete(root);
        return OPENAI_RESPONSE_TO_CONTENT_NULL_OUTPUT_ITEM;
    }

    cJSON *content = cJSON_GetObjectItem(msg, "content");

    char *text = NULL;

    if (cJSON_IsArray(content)) {
        cJSON *first = cJSON_GetArrayItem(content, 0);
        if (cJSON_IsObject(first)) {
            cJSON *txt = cJSON_GetObjectItem(first, "text");
            if (cJSON_IsString(txt)) {
                text = strdup(txt->valuestring);
            }
        }
    } else if (cJSON_IsString(content)) {
        text = strdup(content->valuestring);
    }

    if (!text) {
        cJSON_Delete(root);
        return OPENAI_RESPONSE_TO_CONTENT_NULL_TEXT;
    }

    *res_json = text;
    cJSON_Delete(root);
    return OPENAI_OK;
}

int openai_client_chat_request(const OpenAIClient *client, const OpenAIRequest *request, char **response_json) {
    if (client == NULL ||
        client->authorization == NULL ||
        request == NULL ||
        response_json == NULL) {
        return OPENAI_CLIENT_CHAT_REQUEST_NULL_PARAMS;
    }

    *response_json = NULL;

    char *req_json = NULL;
    int err = openai_request_to_json(request, &req_json);
    if (err != OPENAI_OK) {
        return err;
    }

    WiFiClientSecure secure;
    secure.setInsecure();

    HTTPClient http;
    if (!http.begin(secure, "https://api.openai.com/v1/responses")) {
        free(req_json);
        return OPENAI_CLIENT_CHAT_REQUEST_HTTP_ERROR;
    }

    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", client->authorization + 15); 

    int httpCode = http.POST((uint8_t *)req_json, strlen(req_json));
    free(req_json);

    if (httpCode <= 0) {
        http.end();
        return OPENAI_CLIENT_CHAT_REQUEST_HTTP_ERROR;
    }

    if (httpCode != 200) {
        http.end();
        return OPENAI_CLIENT_CHAT_REQUEST_HTTP_ERROR;
    }

    String payload = http.getString();
    http.end();

    char *response_content = NULL;
    err = openai_response_to_content(payload.c_str(), &response_content);
    if (err != OPENAI_OK) {
        return err;
    }

    *response_json = response_content;
    return OPENAI_OK;
}
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include <curl/curl.h>
#include <cjson/cJSON.h>

#include "request.h"
#include "client.h"
#include "error.h"

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;
    char *ptr = realloc(mem->memory, mem->size + realsize + 1);

    if (ptr == NULL) {
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

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
    return OK;
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
        if (cJSON_IsString(type) && strcmp(type->valuestring, "message") == 0) {
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
    return OK;
}

int openai_client_chat_request(const struct OpenAIClient *client, const OpenAIRequest *request, char **response_json) {
    CURL *curl;
    CURLcode res;
    MemoryStruct chunk;
    struct curl_slist *headers = NULL;
    long http_code = 0;
    char *req_json = NULL;
    char *response_content = NULL;
    int error;

    if (client == NULL || client->authorization == NULL || request == NULL || response_json == NULL) {
        return OPENAI_CLIENT_CHAT_REQUEST_NULL_PARAMS;
    }

    *response_json = NULL;
    chunk.memory = malloc(1);
    chunk.size = 0;
    if (chunk.memory == NULL) {
        return OPENAI_CLIENT_CHAT_REQUEST_MALLOC;
    }

    error = openai_request_to_json(request, &req_json);
    if (error != OK || req_json == NULL) {
        free(chunk.memory);
        return error;
    }

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (!curl) {
        free(req_json);
        free(chunk.memory);
        return OPENAI_CLIENT_CHAT_REQUEST_CURL_INIT;
    }

    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, client->authorization);

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/responses");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req_json);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        free(req_json);
        free(chunk.memory);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        curl_global_cleanup();
        return OPENAI_CLIENT_CHAT_REQUEST_CURL_PERFORM;
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code > 0 && http_code != 200) {
        free(req_json);
        free(chunk.memory);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        curl_global_cleanup();
        return OPENAI_CLIENT_CHAT_REQUEST_HTTP_ERROR;
    }

    error = openai_response_to_content(chunk.memory, &response_content);
    if (error != OK) {
        free(req_json);
        free(chunk.memory);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        curl_global_cleanup();
        return error;
    }

    *response_json = response_content;

    free(req_json);
    free(chunk.memory);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    curl_global_cleanup();

    return OK;
}
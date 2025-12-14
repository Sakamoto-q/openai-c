#include "esp32-error.h"

const char* openai_error_to_string(OpenAIError error) {
    switch (error) {
        case OPENAI_OK:
            return "OK";

        case NEW_OPENAI_CLIENT_NULL_CLIENT:
            return "new_openai_client: NULL_CLIENT";
        case NEW_OPENAI_CLIENT_NULL_TOKEN:
            return "new_openai_client: NULL_TOKEN";
        case NEW_OPENAI_CLIENT_MALLOC:
            return "new_openai_client: MALLOC";

        case OPENAI_CLIENT_FREE_NULL_CLIENT:
            return "openai_client_free: NULL_CLIENT";

        case OPENAI_CLIENT_INIT_AUTH_NULL_CLIENT:
            return "openai_client_init_auth: NULL_CLIENT";
        case OPENAI_CLIENT_INIT_AUTH_ALREADY_INITIALIZED:
            return "openai_client_init_auth: ALREADY_INITIALIZED";
        case OPENAI_CLIENT_INIT_AUTH_NULL_TOKEN:
            return "openai_client_init_auth: NULL_TOKEN";
        case OPENAI_CLIENT_INIT_AUTH_MALLOC:
            return "openai_client_init_auth: MALLOC";
        case OPENAI_CLIENT_INIT_AUTH_SNPRINTF:
            return "openai_client_init_auth: SNPRINTF";

        case OPENAI_REQUEST_TO_JSON_NULL_REQUEST:
            return "openai_request_to_json: NULL_REQUEST";
        case OPENAI_REQUEST_TO_JSON_NULL_REQ_JSON:
            return "openai_request_to_json: NULL_REQ_JSON";
        case OPENAI_REQUEST_TO_JSON_NULL_MODEL_OR_INPUT:
            return "openai_request_to_json: NULL_MODEL_OR_INPUT";
        case OPENAI_REQUEST_TO_JSON_CREATE_OBJECT:
            return "openai_request_to_json: CREATE_OBJECT";
        case OPENAI_REQUEST_TO_JSON_ADD_MODEL:
            return "openai_request_to_json: ADD_MODEL";
        case OPENAI_REQUEST_TO_JSON_ADD_INPUT:
            return "openai_request_to_json: ADD_INPUT";
        case OPENAI_REQUEST_TO_JSON_PRINT:
            return "openai_request_to_json: PRINT";

        case OPENAI_RESPONSE_TO_CONTENT_NULL_RES_JSON:
            return "openai_response_to_content: NULL_RES_JSON";
        case OPENAI_RESPONSE_TO_CONTENT_NULL_RESPONSE:
            return "openai_response_to_content: NULL_RESPONSE";
        case OPENAI_RESPONSE_TO_CONTENT_PARSE_JSON:
            return "openai_response_to_content: PARSE_JSON";
        case OPENAI_RESPONSE_TO_CONTENT_NULL_OUTPUT:
            return "openai_response_to_content: NULL_OUTPUT";
        case OPENAI_RESPONSE_TO_CONTENT_NULL_OUTPUT_ITEM:
            return "openai_response_to_content: NULL_OUTPUT_ITEM";
        case OPENAI_RESPONSE_TO_CONTENT_NULL_CONTENT:
            return "openai_response_to_content: NULL_CONTENT";
        case OPENAI_RESPONSE_TO_CONTENT_NULL_CONTENT_ITEM:
            return "openai_response_to_content: NULL_CONTENT_ITEM";
        case OPENAI_RESPONSE_TO_CONTENT_NULL_TEXT:
            return "openai_response_to_content: NULL_TEXT";
        case OPENAI_RESPONSE_TO_CONTENT_INVALID_TEXT:
            return "openai_response_to_content: INVALID_TEXT";
        case OPENAI_RESPONSE_TO_CONTENT_STRDUP:
            return "openai_response_to_content: STRDUP";

        case OPENAI_CLIENT_CHAT_REQUEST_NULL_PARAMS:
            return "openai_client_chat_request: NULL_PARAMS";
        case OPENAI_CLIENT_CHAT_REQUEST_MALLOC:
            return "openai_client_chat_request: MALLOC";
        case OPENAI_CLIENT_CHAT_REQUEST_CURL_INIT:
            return "openai_client_chat_request: CURL_INIT";
        case OPENAI_CLIENT_CHAT_REQUEST_CURL_PERFORM:
            return "openai_client_chat_request: CURL_PERFORM";
        case OPENAI_CLIENT_CHAT_REQUEST_HTTP_ERROR:
            return "openai_client_chat_request: HTTP_ERROR";

        default:
            return "UNKNOWN_OPENAI_ERROR";
    }
}
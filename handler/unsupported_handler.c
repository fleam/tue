#include "unsupported_handler.h"

// 处理不支持的 URI
void handle_unsupported_uri(struct mg_connection *c) {
    cJSON *root = cJSON_CreateObject();
    cJSON *error_obj = cJSON_CreateString("Unsupported URI");
    cJSON_AddItemToObject(root, "error", error_obj);

    char *json_str = cJSON_Print(root);
    mg_http_reply(c, 500, "Content-Type: application/json\r\n", "%s", json_str);

    cJSON_free(json_str);
    cJSON_Delete(root);
}
    
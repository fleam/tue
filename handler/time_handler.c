#include "time_handler.h"
#include <time.h>

// 处理 /api/time/get 接口
void handle_time_get(struct mg_connection *c) {
    cJSON *root = cJSON_CreateObject();
    cJSON *time_obj = cJSON_CreateNumber((long long)time(NULL));
    cJSON_AddItemToObject(root, "time", time_obj);

    char *json_str = cJSON_Print(root);
    mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s", json_str);

    cJSON_free(json_str);
    cJSON_Delete(root);
}
    
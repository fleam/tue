#include "directory_handler.h"
#include <dirent.h>

// 处理 /api/directory/structure 接口
void handle_directory_structure(struct mg_connection *c) {
    DIR *dir;
    struct dirent *entry;
    cJSON *root = cJSON_CreateObject();
    cJSON *files_array = cJSON_CreateArray();

    if ((dir = opendir(".")) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            cJSON *file_obj = cJSON_CreateString(entry->d_name);
            cJSON_AddItemToArray(files_array, file_obj);
        }
        closedir(dir);
    }

    cJSON_AddItemToObject(root, "files", files_array);

    char *json_str = cJSON_Print(root);
    mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s", json_str);

    cJSON_free(json_str);
    cJSON_Delete(root);
}
    
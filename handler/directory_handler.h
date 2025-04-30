#ifndef DIRECTORY_HANDLER_H
#define DIRECTORY_HANDLER_H

#include "mongoose.h"
#include "cjson/cJSON.h"

void handle_directory_structure(struct mg_connection *c);

#endif
    
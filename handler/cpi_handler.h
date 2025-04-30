#ifndef CPI_HANDLER_H
#define CPI_HANDLER_H

#include "mongoose.h"
#include "cjson/cJSON.h"
#include "../lib/mysql_connection_pool.h"

void handle_cpi(struct mg_connection *c, struct mg_http_message *hm, ConnectionPool *pool);

#endif    
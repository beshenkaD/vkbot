#pragma once

#include <sqlite3.h>
#include <stdio.h>

typedef int (*db_callback_t)(void *data, int argc, char **argv, char **columns);

sqlite3 *sqlite_open(const char *filename);
void sqlite_exec(sqlite3 *db, const char *cmd, db_callback_t callback,
                 void *data);
void sqlite_close(sqlite3 *db);

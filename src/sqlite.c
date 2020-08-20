#include <sqlite.h>

sqlite3 *sqlite_open(const char *filename) {
  sqlite3 *db_handle = NULL;

  int code = sqlite3_open(filename, &db_handle);

  if (code) {
    fprintf(stderr, "Can't open database '%s'\nError: %s\n", filename,
            sqlite3_errmsg(db_handle));
    return NULL;
  }

  return db_handle;
}

void sqlite_exec(sqlite3 *db, const char *cmd, db_callback_t callback,
                 void *data) {
  char *error_msg;

  int code = sqlite3_exec(db, cmd, callback, data, &error_msg);

  if (code != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", error_msg);
    sqlite3_free(error_msg);
  }
}

void sqlite_close(sqlite3 *db) {
  int code = sqlite3_close(db);

  if (code) //
    fprintf(stderr, "Can't close database.\nError: %s\n", sqlite3_errmsg(db));
}

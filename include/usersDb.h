#pragma once

#include <module.h>
#include <sqlite.h>

#define DB_CREATE_MAIN_TABLE                                                   \
  "CREATE TABLE IF NOT EXISTS users ( \
  id          INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, \
  peer_id     INTEGER NOT NULL, \
  user_id     INTEGER NOT NULL, \
  permission  INTEGER NOT NULL);"

#define DB_INSERT_USER                                                         \
  "INSERT INTO users(user_id, permission, peer_id) VALUES (%i, %i, %i);"
#define DB_GET_USER_BY_ID "SELECT * FROM users WHERE user_id = %i;"
#define DB_GET_PERMISSION_BY_ID                                                \
  "SELECT permission FROM users WHERE user_id = %i AND peer_id = %i;"
#define DB_SET_PERMISSION_BY_ID                                                \
  "UPDATE users SET permission = %i WHERE user_id = %i AND peer_id = %i;"

int users_init();
int users_deinit();
permission_t get_permission_by_id(int user_id, uint32_t peer_id);
void set_permission_by_id(int user_id, uint32_t peer_id, int perm);
const char *get_permission_name(permission_t perm);
void new_user(int user_id, uint32_t peer_id, int perm);

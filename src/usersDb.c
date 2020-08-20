#include <sds.h>
#include <usersDb.h>

static sqlite3 *users_db = NULL;

int users_init() {
  users_db = sqlite_open("./db/users.sqlite3");

  if (!users_db) //
    return 1;

  sqlite_exec(users_db, DB_CREATE_MAIN_TABLE, NULL, NULL);

  return 0;
}

int users_deinit() {
  sqlite_close(users_db);
  return 0;
}

static int permission_callback(void *data, int argc, char **argv,
                               char **colums_name) {
  permission_t *priv = (permission_t *)(data);

  if (colums_name[0])
    *priv = atoi(argv[0]);

  return 0;
}

permission_t get_permission_by_id(int user_id, uint32_t peer_id) {
  sds s = sdsempty();

  s = sdscatfmt(s, DB_GET_PERMISSION_BY_ID, user_id, peer_id);

  permission_t ret = error;

  sqlite_exec(users_db, s, permission_callback, &ret);

  sdsfree(s);

  if (ret != error) //
    return ret;
  if (ret == error) //
    return user;
  /* new_user(user_id, peer_id, 1); */

  return error;
}

void new_user(int user_id, uint32_t peer_id, int perm) {
  sds s = sdsempty();

  s = sdscatfmt(s, DB_INSERT_USER, user_id, perm, peer_id);

  sqlite_exec(users_db, s, NULL, NULL);

  sdsfree(s);
}

void set_permission_by_id(int user_id, uint32_t peer_id, int perm) {
  sds s = sdsempty();

  s = sdscatfmt(s, DB_SET_PERMISSION_BY_ID, perm, user_id, peer_id);

  sqlite_exec(users_db, s, NULL, NULL);

  sdsfree(s);
}

const char *get_permission_name(permission_t perm) {
  switch (perm) {
  case error: return "Ошибка";
  case banned: return "Забанен";
  case user: return "Обычный пользователь";
  case helper: return "Хелпер";
  case admin: return "Админ";
  }
  return "Ошибка";
}

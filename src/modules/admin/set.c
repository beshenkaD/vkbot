#include <ctype.h>
#include <modules/admin/set.h>
#include <utils/stringutils.h>
#include <utils/vkutils.h>

command Set  = {"Set", set,
               "/",   "Установить привелегию пользователю",
               admin, {"set", "сет", "Set", "Сет"}};

#define ALREADY_HAVE  "Пользователь уже имеет эту привелегию"
#define SETTING_ADMIN "Невозможно установить привелегию для администратора"
#define SET_YOURSELF  "Невозможно менять привелегию самому себе"

static char *set_by_reply(message *msg, int argc, char **argv, int p);
static char *set_by_args(message *msg, int argc, char **argv, int p);
static permission_t get_permission_by_string(char *str);

void set(message *msg, int argc, char *argv[], int p) {
  update_admins_db(msg);
  p = get_permission_by_id(msg->from_id, msg->peer_id);

  if (p < admin) {
    messages_send(NO_PERM, msg->peer_id);
    return;
  }

  if (argc < 3) {
    char *ret = set_by_reply(msg, argc, argv, p);
    messages_send(ret, msg->peer_id);
    return;
  }

  if (argc > 2) {
    char *ret = set_by_args(msg, argc, argv, p);
    messages_send(ret, msg->peer_id);
    return;
  }
}

static char *set_by_reply(message *msg, int argc, char **argv, int p) {
  if (msg->r_from_id == 0) //
    return NO_ARGS;

  if (msg->r_from_id == msg->from_id) //
    return SET_YOURSELF;

  if (get_permission_by_id(msg->r_from_id, msg->peer_id) == admin) //
    return SETTING_ADMIN;

  if (argc < 2) //
    return NO_ARGS;

  permission_t perm = get_permission_by_string(argv[1]);
  permission_t current_perm =
      get_permission_by_id(msg->r_from_id, msg->peer_id);

  if (perm == error) {
    return va(BAD_ARG, argv[1]);
  }

  if (current_perm == perm) {
    return ALREADY_HAVE;
  }

  if (current_perm == user)
    new_user(msg->r_from_id, msg->peer_id, perm);
  else
    set_permission_by_id(msg->r_from_id, msg->peer_id, perm);
  return va("Привелегия %s успешно установлена", get_permission_name(perm));
}

static char *set_by_args(message *msg, int argc, char **argv, int p) {
  int count         = 0;
  permission_t perm = get_permission_by_string(argv[1]);

  if (perm == error)
    return va(BAD_ARG, argv[1]);

  for (int i = 2; i < argc; i++) {
    long id                   = get_id_by_mention(argv[i]);
    permission_t current_perm = get_permission_by_id(id, msg->peer_id);

    if (id == -2)
      return va(BAD_ARG, argv[i]);
    if (p < 3)
      return NO_PERM;
    if (current_perm == perm)
      return ALREADY_HAVE;
    if (current_perm == admin)
      return SETTING_ADMIN;

    if (current_perm == user)
      new_user(id, msg->peer_id, perm);
    else
      set_permission_by_id(id, msg->peer_id, perm);

    count++;
  }
  return va("Привелегия %s успешно установлена для %d пользователей",
            get_permission_name(perm), count);
}

static permission_t get_permission_by_string(char *str) {
  for (int i = 0; str[i]; i++) {
    str[i] = tolower(str[i]);
  }

  if (strncmp(str, "helper", 6) == 0)
    return helper;
  if (strncmp(str, "banned", 6) == 0)
    return banned;
  if (strncmp(str, "user", 4) == 0)
    return user;

  return error;
}

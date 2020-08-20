#include <modules/admin/kick.h>
#include <utils/stringutils.h>
#include <utils/vkutils.h>

command Kick = {"Kick", kick,
                "/",    "Исключить пользователя.",
                helper, {"kick", "кик", "Kick", "Кик"}};

static char *kick_by_reply(message *msg, int p);
static char *kick_by_args(message *msg, int argc, char **argv, int p);

void kick(message *msg, int argc, char *argv[], int p) {
  update_admins_db(msg);
  p = get_permission_by_id(msg->from_id, msg->peer_id);

  if (p < helper) {
    messages_send(NO_PERM, msg->peer_id);
    return;
  }

  if (argc == 1) {
    char *err = kick_by_reply(msg, p);
    if (err) {
      messages_send(err, msg->peer_id);
      return;
    }
    return;
  }

  if (argc > 1) {
    char *err = kick_by_args(msg, argc, argv, p);
    if (err) {
      messages_send(err, msg->peer_id);
      return;
    }
    return;
  }
}

static char *kick_by_reply(message *msg, int p) {
  if (msg->r_from_id == 0)
    return NO_ARGS;
  if (get_permission_by_id(msg->r_from_id, msg->peer_id) > 1 && p < 3)
    return NO_PERM;

  messages_remove_chat_user(msg->peer_id, msg->r_from_id);
  return NULL;
}

static char *kick_by_args(message *msg, int argc, char **argv, int p) {
  for (int i = 1; i < argc; i++) {
    int id = get_id_by_mention(argv[i]);

    if (id == -2)
      return va(BAD_ARG, argv[i]);

    int per = get_permission_by_id(id, msg->peer_id);
    if (per > 1 && p < 3)
      return NO_PERM;

    messages_remove_chat_user(msg->peer_id, id);
  }
  return NULL;
}


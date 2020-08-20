#include <modules/admin/ban.h>
#include <utils/stringutils.h>
#include <utils/vkutils.h>

command Ban = {"Ban", ban,
               "/",   "Исключить пользователя и добавить в бан-лист",
               admin, {"Ban", "ban", "Бан", "бан"}};

action_command Ban_act = {ban_act, "chat_invite_user"};

void ban(message *msg, int argc, char *argv[], int p) {
  update_admins_db(msg);
  p = get_permission_by_id(msg->from_id, msg->peer_id);

  if (p < admin) {
    messages_send(NO_PERM, msg->peer_id);
    return;
  }

  if (argc == 1) {
    if (msg->r_from_id == 0) {
      messages_send(NO_ARGS, msg->peer_id);
      return;
    }

    int perm = get_permission_by_id(msg->r_from_id, msg->peer_id);

    if (perm > 2) {
      messages_send(NO_PERM, msg->peer_id);
      return;
    }

    if (perm == 1)
      new_user(msg->r_from_id, msg->peer_id, 0);
    else
      set_permission_by_id(msg->r_from_id, msg->peer_id, 0);

    messages_remove_chat_user(msg->peer_id, msg->r_from_id);
  }

  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      int id = get_id_by_mention(argv[i]);

    if (id == -2) {
       messages_send(va(BAD_ARG, argv[i]), msg->peer_id);
       return;
    }

    int perm = get_permission_by_id(id, msg->peer_id);
    if (p < 3 || perm == 3) {
      messages_send(NO_PERM, msg->peer_id);
      return;
    }

    if (perm == 1)
      new_user(id, msg->peer_id, 0);
    else
      set_permission_by_id(id, msg->peer_id, 0);

    messages_remove_chat_user(msg->peer_id, id);
    }
  }
}

void ban_act(message *msg) {
  int p = get_permission_by_id(msg->member_id, msg->peer_id);
  if (p == 0) {
    messages_send("ВАМ БАН", msg->peer_id);
    messages_remove_chat_user(msg->peer_id, msg->member_id);
  }
}

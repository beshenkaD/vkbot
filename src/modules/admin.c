#include <modules/admin.h>

void kick(message *msg) {
  messages_remove_chat_user(msg->peer_id - 2000000000, msg->r_from_id);
}
module Kick = {"Kick", kick, "/", "Исключить пользователя.", 4, {"kick", "кик", "Kick", "Кик"}};

#include <main.h>
#include <modules/std/std.h>

void off(message *msg, int argc, char *argv[], int p) {
  if (msg->from_id == hostid)
    running = false;
  else
    messages_send("Access denied. Only host can run this command",
                  msg->peer_id);
}

void hello(message *msg) {
  messages_send("ЗДАРОВА ЕБАТЬ ДАВНО НЕ ВИДЕЛИСЬ", msg->peer_id);
}

action_command Hello = {hello, "chat_invite_user"};

command Off = {"Off", off, "/", "Отключает бота.", user, {"off"}};

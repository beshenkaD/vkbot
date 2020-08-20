#include <modules/std/ping.h>

command Ping = {"Ping", ping,
                "/",    "Проверка работоспособности бота.",
                user,   {"ping", "пинг", "Ping", "Пинг"}};

void ping(message *msg, int argc, char *argv[], int p) {
  messages_send("pong", msg->peer_id);
}


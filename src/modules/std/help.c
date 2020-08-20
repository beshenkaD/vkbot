#include <modules/std/help.h>
#include "../../../config/config.h"

command Help = {"Help",   help, "/",
                "Помощь", user, {"help", "помощь", "Help", "Помощь"}};

void help(message *msg, int argc, char *argv[], int p) {
  messages_send(helpurl, msg->peer_id);
}


#include <modules/admin/info.h>
#include <utils/stringutils.h>
#include <utils/vkutils.h>

static void get_info(int id, uint32_t peer_id, int p);

command Info = {"Info", info,
                "/",    "Информация о пользователе",
                user,   {"info", "Info", "Инфо", "инфо"}};

void info(message *msg, int argc, char *argv[], int p) {
  if (msg->r_from_id != 0) {
    int p = get_permission_by_id(msg->r_from_id, msg->peer_id);
    get_info(msg->r_from_id, msg->peer_id, p);
    return;
  }

  if (argc < 2) {
    int p = get_permission_by_id(msg->from_id, msg->peer_id);
    get_info(msg->from_id, msg->peer_id, p);
    return;
  }

  if (argc == 2) {
    int id = get_id_by_mention(argv[1]);

    if (id == -2) {
      messages_send(va(BAD_ARG, argv[1]), msg->peer_id);
      return;
    }

    int p = get_permission_by_id(id, msg->peer_id);
    get_info(id, msg->peer_id, p);
    return;
  }

  if (argc > 2) {
    messages_send("Слишком много аргументов", msg->peer_id);
    return;
  }
}

static void get_info(int id, uint32_t peer_id, int p) {
  members_t *users = get_conv_members(peer_id, "", false, true, false);

  for (int i = 0; i < users->members_count; i++) {
    if (users->members[i].member_id == id) {
      char buf[21];

      time_t add_time = users->members[i].join_date - 19800;
      struct tm ts;
      ts = *localtime(&add_time);
      strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M", &ts);

      time_t now, wasted; // wasted time... beshenka emo-kid
      time(&now);
      wasted = (now - 19800) - add_time;
      wasted = wasted / (3600);

      if (users->members[i].is_admin)
        p = 3;
      else
        p = get_permission_by_id(id, peer_id);

      if (users->members[i].member_id > 0)
        messages_send(va("Информация о [id%d|пользователе]\nВ беседе с:\n-- "
                         "%s (%d ч.)\nПривелегия: %s",
                         id, buf, wasted, get_permission_name(p)),
                      peer_id);
      else
        messages_send(va("Информация о [club%d|боте]\nВ беседе с:\n-- "
                         "%s (%d ч.)\nПривелегия: %s",
                         id * -1, buf, wasted, get_permission_name(p)),
                      peer_id);
      members_cleanup(users);
      return;
    }
  }
  members_cleanup(users);
}

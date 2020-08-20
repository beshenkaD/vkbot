#include <modules/admin/heads.h>
#include <sds.h>
#include <utils/stringutils.h>

command Heads = {"Heads", heads,
                 "/",     "Информация о привелегированных участниках беседы",
                 user,    {"heads", "Heads", "Главы", "главы"}};

void heads(message *msg, int argc, char **argv, int p) {
  update_admins_db(msg);

  sds str     = sdsempty();
  sds admins  = sdsnew("\n");
  sds helpers = sdsnew("\n");

  members_t *users = get_conv_members(msg->peer_id, "", true, false, true);

  for (int i = 0; i < users->groups_count + users->profiles_count; i++) {
    int group_p   = get_permission_by_id(users->groups[i].id, msg->peer_id);
    int profile_p = get_permission_by_id(users->profiles[i].id, msg->peer_id);

    if (group_p == 2) {
      helpers = sdscatfmt(helpers, "[club%i|%s]\n", users->groups[i].id,
                          users->groups[i].name);
    }

    if (group_p == 3) {
      admins = sdscatfmt(admins, "[club%i|%s]\n", users->groups[i].id,
                         users->groups[i].name);
    }

    if (profile_p == 2) {
      helpers = sdscatfmt(helpers, "[id%i|%s %s]\n", users->profiles[i].id,
                          users->profiles[i].first_name,
                          users->profiles[i].last_name);
    }

    if (profile_p == 3) {
      admins = sdscatfmt(admins, "[id%i|%s %s]\n", users->profiles[i].id,
                         users->profiles[i].first_name,
                         users->profiles[i].last_name);
    }
  }
  str = sdscatfmt(str, "Администраторы:%s\nХелперы:%s\n", admins, helpers);
  messages_send(str, msg->peer_id);

  sdsfree(str);
  sdsfree(admins);
  sdsfree(helpers);
  members_cleanup(users);
}

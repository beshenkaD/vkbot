#include <modules/admin.h>
#include <utils/stringutils.h>

static bool is_helper(long user_id, long peer_id) {
  permission_t a = get_permission_by_id(user_id, peer_id);

  if (a == HELPER) return true;

  return false;
}

static bool is_admin(long user_id, long peer_id) {
  cJSON *root = messages_get_conv_members(peer_id, "");
  cJSON *response = cJSON_GetObjectItem(root, "response");
  cJSON *items = cJSON_GetObjectItem(response, "items");
  int n = cJSON_GetArraySize(items);

  for (int i = 0; i < n; i++) {
    cJSON *item = cJSON_GetArrayItem(items, i);
    cJSON *member_id = cJSON_GetObjectItem(item, "member_id");
    cJSON *is_admin = cJSON_GetObjectItem(item, "is_admin");

    if ((member_id->valueint == user_id) & cJSON_IsTrue(is_admin)) {
      cJSON_Delete(root);
      return true;
    }
  }
  cJSON_Delete(root);
  return false;
}

void set(message *msg, int argc, char *argv[]) { 
  if (msg->from_id != hostid) //
    return;

  new_user(msg->r_from_id, msg->peer_id, HELPER); 
}

command Set = {"Set", set, "/", " ", 2, ADMIN, {"set", "сет"}};

void kick(message *msg, int argc, char *argv[]) {
  if (msg->r_from_id == (atoi(botid) * -1)) {
    return;
  }

  if (is_admin(msg->from_id, msg->peer_id) || is_helper(msg->from_id, msg->peer_id)) {
    if (argc == 1)
      messages_remove_chat_user(msg->peer_id - 2000000000, msg->r_from_id);

    else {
      for (int i = 1; i < argc; i++) {
        cJSON *root = utils_resolve_screen_name(argv[i]);
        cJSON *response = cJSON_GetObjectItem(root, "response");
        cJSON *obj_id = cJSON_GetObjectItem(response, "object_id");

        if (!obj_id) {
          messages_send(va("Неверный аргумент: %s", argv[i]), msg->peer_id);
          cJSON_Delete(root);

          return;
        }
        messages_remove_chat_user(msg->peer_id - 2000000000, obj_id->valueint);
      }
    }

  } else
    messages_send("У вас нет привелегий для выполнения этой команды.", msg->peer_id);
}

command Kick = {"Kick", kick, "/", "Исключить пользователя.", 4, HELPER, {"kick", "кик", "Kick", "Кик"}};

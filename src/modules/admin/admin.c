#include <modules/admin/kick.h>
#include <modules/admin/set.h>

void update_admins_db(message *msg) {
  cJSON *root  = messages_get_conv_members(msg->peer_id, "");
  cJSON *resp  = cJSON_GetObjectItem(root, "response");
  cJSON *items = cJSON_GetObjectItem(resp, "items");
  int n        = cJSON_GetArraySize(items);

  for (int i = 0; i < n; i++) {
    cJSON *item      = cJSON_GetArrayItem(items, i);
    cJSON *member_id = cJSON_GetObjectItem(item, "member_id");
    cJSON *is_admin  = cJSON_GetObjectItem(item, "is_admin");

    int p = get_permission_by_id(member_id->valueint, msg->peer_id);

    if (is_admin && p != 1) {
      set_permission_by_id(member_id->valueint, msg->peer_id, admin);
    }

    if (is_admin && p == 1) {
      new_user(member_id->valueint, msg->peer_id, admin);
    }

    p = get_permission_by_id(member_id->valueint, msg->peer_id);

    if (p == 3 && !cJSON_IsTrue(is_admin)) {
      set_permission_by_id(member_id->valueint, msg->peer_id, user);
    }
  }
  cJSON_Delete(root);
}

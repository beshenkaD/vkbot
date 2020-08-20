#include <vkapi/users.h>
#include <vkapi/vkapi.h>

void members_cleanup(members_t *m) {
  if (m->members)
    free(m->members);
  if (m->groups)
    free(m->groups);
  if (m->profiles)
    free(m->profiles);
  free(m);
}

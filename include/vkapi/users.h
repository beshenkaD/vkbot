#include <stdbool.h>
#include <stdint.h>

typedef struct {
  int member_id;
  uint32_t join_date;
  bool is_admin; // cache friendly LOL
} member;

typedef struct {
  int id;
  char first_name[32];
  char last_name[32];
  short sex;
} profile;

typedef struct {
  int id;
  char name[64];
} group;

typedef struct {
  member *members;
  profile *profiles;
  group *groups;

  int members_count;
  int profiles_count;
  int groups_count;
} members_t;

void members_cleanup(members_t *m);

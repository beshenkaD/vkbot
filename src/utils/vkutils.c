#include <ctype.h>
#include <stdbool.h>
#include <utils/vkutils.h>

int get_id_by_mention(char *mention) {
  if (strncmp(mention, "[id", 3) != 0) {
    if (strncmp(mention, "[club", 5) != 0) {
      return -2;
    }
  }

  bool is_group = (mention[1] == 'c');

  char str[16];

  int i = 0, c = 0;
  for (; i < strlen(mention); i++) {
    if (mention[i] == '|')
      goto end;

    if (isdigit(mention[i])) {
      str[c] = mention[i];
      c++;
    }
  }
  str[c + 1] = '\0';
end:
  if (is_group)
    return (atoi(str) * -1);
  else
    return atoi(str);
}


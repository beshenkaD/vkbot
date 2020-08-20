#include <module.h>
#include <usersDb.h>
#include <utils/stringutils.h>

int module_count = 0;
int act_commands_count = 0;

static void load_cmd(command *m) {
  int i = 0;
  while (m->triggers[i] != NULL) {
    map_set(&commands, va("%s%s", m->prefix, m->triggers[i]), m->cmd_func);
    i++;
  }
}

static void load_act_cmd(action_command *a) {
  act_commands[act_commands_count] = *a;
  act_commands_count++;
}

void load_module(module *m) {
  printf("Loading module '%s': ", m->name);
  modules[module_count] = *m;

  if (module_count == max_modules_count) {
    printf("ERROR. max_modules_count reached\n");
    return;
  }

  int i = 0;
  while (m->cmds[i].cmd_func != NULL) {
    load_cmd(&m->cmds[i]);
    i++;
  }

  i = 0;
  while (m->a_cmds[i].act_func != NULL) {
    load_act_cmd(&m->a_cmds[i]);
    i++;
  }

  printf("OK\n");

  module_count++;
}

// Генерируемый текст выводится в stdout. Предполагается что вы поместите его в
// посте/статье на стене бота
void generate_help() {
  int i = 0, j = 0, k = 0;

  while (modules[i].name) {
    printf("Модуль: %s - %s\n", modules[i].name, modules[i].description);

    j = 0;
    while (modules[i].cmds[j].cmd_func) {
      printf("* %s - %s\n", modules[i].cmds[j].name,
             modules[i].cmds[j].description);
      printf("  Привелегия: %s\n",
             get_permission_name(modules[i].cmds[j].permission));
      printf("  Команды: ");

      k = 0;
      while (modules[i].cmds[j].triggers[k]) {
        printf("%s%s ", modules[i].cmds[j].prefix,
               modules[i].cmds[j].triggers[k]);
        k++;
      }
      printf("\n");
      j++;
    }
    i++;
  }
}

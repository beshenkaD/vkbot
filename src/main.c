#include <main.h>
#include <modules/admin/admin.h>
#include <modules/fun/fun.h>
#include <modules/std/std.h>

bool running = true;

static void get_time() {
  time_t rawtime;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  printf("Bot started. at: %s\n", asctime(timeinfo));
}

int main(int argc, char **argv) {
  users_init();
  get_time();

  module std = {"Standart",
                "Стандартные функции бота.",
                {Ping, Stat, Off, Help},
                {Hello}};

  module adm = {"Admin",
                "Функции для администрирования бесед",
                {Kick, Set, Info, Ban, Heads},
                {Ban_act}};

  module fun = {"Fun", "Смехуечки", {Anek}};

  load_module(&std);
  load_module(&adm);
  load_module(&fun);

  if (argc > 1) {
    printf("\n");
    generate_help();
    return 0;
  }

  vk_init();

  while (running) {
    cJSON *lp    = get_longpoll_json();
    message *msg = get_longpoll_events(lp);

    handler(msg);

    cJSON_Delete(lp);
    free(msg);
  }

  users_deinit();

  vk_destroy();
  return 0;
}

#include <modules/std/stat.h>
#include <sys/resource.h>
#include <utils/stringutils.h>
#include <main.h>

#define maxSize 125

command Stat = {"Statistic",        stat, "/",
                "Статистика бота.", user, {"stat", "стат", "Stat", "Стат"}};

void stat(message *msg, int argc, char *argv[], int p) {
  /* get module names */
  char result[maxSize] = {0x0};
  for (int i = 0; i < module_count; i++) {
    strncat(result, va("「%s」 ", modules[i].name),
            maxSize - strlen(result) - 1);
  }
  /* get memory usage */
  struct rusage r_usage;
  getrusage(RUSAGE_SELF, &r_usage);

  float mem = r_usage.ru_maxrss / 1000.0;

  messages_send(va("Бот запущен: %s\nЗагружен с модулями: %s\nПотребление "
                   "памяти: %01.3f MB",
                   asctime(timeinfo), result, mem),
                msg->peer_id);
}

#include <modules/std.h>
#include <main.h>
#include <sys/resource.h>
#include <utils/stringutils.h>
#define maxSize 128

void ping(message *msg, int argc, char *argv[]) {
  messages_send("pong", msg->peer_id);  //
}

void stat(message *msg, int argc, char *argv[]) {
  /* get module names */
  char result[maxSize] = {0x0};
  for (int i = 0; i < module_count; i++) {
    strncat(result, va("「%s」 ", modules[i].name), maxSize - strlen(result) - 1);
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

void off(message *msg, int argc, char *argv[]) {
  if (msg->from_id == hostid)
    running = false;
  else
    messages_send("Access denied. Only host can run this command", msg->peer_id);
}

command Ping = {
    "Ping", ping, "/", "Проверка работоспособности бота.", 4, USER, {"ping", "пинг", "Ping", "Пинг"}};

command Stat = {"Statistic", stat, "/", "Статистика бота.", 4, USER, {"stat", "стат", "Stat", "Стат"}};

command Off = {"Off", off, "/", "Отключает бота.", 1, USER, {"off"}};

#pragma once

#include <vkapi.h>
#include <methods.h>
#include <module.h>
#include <time.h>

void ping(message *msg, int argc, char *argv[]);
void off(message *msg, int argc, char *argv[]);
void mem(message *msg, int argc, char *argv[]);

/* modules */
extern command Ping;
extern command Stat;
extern command Off;

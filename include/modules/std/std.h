#pragma once

#include <module.h>
#include <vkapi/methods.h>
#include <vkapi/vkapi.h>

void off(message *msg, int argc, char *argv[], int p);

/* modules */
extern command Ping;
extern command Stat;
extern command Off;
extern command Help;

extern action_command Hello;

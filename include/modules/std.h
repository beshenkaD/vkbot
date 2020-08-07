#pragma once

#include <vkapi.h>
#include <methods.h>
#include <module.h>
#include <time.h>

void ping(message *msg);
void off(message *msg);
void mem(message *msg);

/* modules */
extern module Ping;
extern module Stat;
extern module Off;

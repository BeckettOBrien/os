#pragma once
#include "cpu/isr.h"

void ps2_interrupt_handler(interrupt_state state);
void initialize_keyboard(void);
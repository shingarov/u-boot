/*
 * Copyright 2015 IBM Corp.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <config.h>
#include <spl.h>

void board_init_f(unsigned long dummy)
{
	board_init_r(NULL, 0);
}

void board_init_r(gd_t *p, ulong n)
{
	while(1);
}

void coloured_LED_init(void)
{
}

void red_led_on(void)
{
}

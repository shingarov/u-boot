/*
 * Copyright 2012 ASPEED Technology Inc.
 * Copyright 2016 IBM Corporation
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _AST_SDMC_H
#define _AST_SDMC_H

/* Register for SDMC */
#define AST_SDMC_PROTECT	0x00		/* protection key register */
#define AST_SDMC_CONFIG		0x04		/* Configuration register */

/*	AST_SDMC_PROTECT: 0x00  - protection key register */
#define SDMC_PROTECT_UNLOCK			0xFC600309

/*	AST_SDMC_CONFIG : 0x04	- Configuration register */
#define SDMC_CONFIG_VER_NEW		(0x1 << 28)
#define SDMC_CONFIG_MEM_GET(x)		(x & 0x3)

#define SDMC_CONFIG_CACHE_EN		(0x1 << 10)
#define SDMC_CONFIG_EEC_EN		(0x1 << 7)

#endif

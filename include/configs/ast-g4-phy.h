/*
 * Copyright 2016 IBM Corporation
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _AST_G4_NCSI_CONFIG_H
#define _AST_G4_NCSI_CONFIG_H

#define CONFIG_ARCH_AST2400
#define CONFIG_SYS_LOAD_ADDR		0x43000000

#define CONFIG_MISC_INIT_R

#include <configs/ast-common.h>

/* Ethernet */
#define CONFIG_MAC_NUM			2
#define CONFIG_FTGMAC100
#define CONFIG_PHY_MAX_ADDR		32
#define CONFIG_FTGMAC100_EGIGA

/* platform.S settings */
#define CONFIG_CPU_420			1
#define CONFIG_DRAM_528			1

#endif	/* _AST_G4_NCSI_CONFIG_H */

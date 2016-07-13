/*
 * Copyright 2016 IBM Corporation
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _AST_G5_NCSI_CONFIG_H
#define _AST_G5_NCSI_CONFIG_H

#define CONFIG_ARCH_AST2500
#define CONFIG_SYS_LOAD_ADDR		0x83000000

#include <configs/ast-common.h>

/* arm1176/start.S */
#define CONFIG_SYS_UBOOT_BASE		CONFIG_SYS_TEXT_BASE

/* Ethernet */
#define CONFIG_LIB_RAND
#define CONFIG_ASPEEDNIC

/* platform.S settings */
#define	CONFIG_DRAM_ECC_SIZE		0x10000000

#endif	/* _AST_G5_NCSI_CONFIG_H */

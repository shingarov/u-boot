/*
 * (C) Copyright ASPEED Technology Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <stdlib.h>

#include <common.h>
#include <log.h>
#include <asm/io.h>

#include <linux/bitops.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/iopoll.h>

#include "aspeed_hace.h"

static int ast_hace_wait_isr(u32 reg, u32 flag, int timeout_us)
{
	u32 val;

	return readl_poll_timeout(reg, val, (val & flag) == flag, timeout_us);
}

int digest_object(const void *src, unsigned int length, void *digest,
		  u32 method)
{
	writel((u32)src, ASPEED_HACE_HASH_SRC);
	writel((u32)digest, ASPEED_HACE_HASH_DIGEST_BUFF);
	writel(length, ASPEED_HACE_HASH_DATA_LEN);
	writel(HACE_SHA_BE_EN | method, ASPEED_HACE_HASH_CMD);

	return ast_hace_wait_isr(ASPEED_HACE_STS, HACE_HASH_ISR, 1000);
}

static bool crypto_enabled;

#define SCU_BASE	0x1e6e2000
static void enable_crypto(void)
{
	if (crypto_enabled)
		return;
	else
		crypto_enabled = true;

	writel(BIT(4), SCU_BASE + 0x040);
	udelay(300);
	writel(BIT(24), SCU_BASE + 0x084);
	writel(BIT(13), SCU_BASE + 0x084);
	mdelay(30);
	writel(BIT(4), SCU_BASE + 0x044);

}

void hw_sha1(const unsigned char *pbuf, unsigned int buf_len,
	       unsigned char *pout, unsigned int chunk_size)
{
	int rc;

	enable_crypto();

	rc = digest_object(pbuf, buf_len, pout, HACE_ALGO_SHA1);
	if (rc)
		debug("HACE failure\n");
}

void hw_sha256(const unsigned char *pbuf, unsigned int buf_len,
	       unsigned char *pout, unsigned int chunk_size)
{
	int rc;

	enable_crypto();

	rc = digest_object(pbuf, buf_len, pout, HACE_ALGO_SHA256);
	if (rc)
		debug("HACE failure\n");
}

void hw_sha512(const unsigned char *pbuf, unsigned int buf_len,
	       unsigned char *pout, unsigned int chunk_size)
{
	int rc;

	enable_crypto();

	rc = digest_object(pbuf, buf_len, pout, HACE_ALGO_SHA512);
	if (rc)
		debug("HACE failure\n");
}

struct aspeed_sg {
	uint32_t addr;
	uint32_t size;
};

#define HACE_SG_MAX	128

struct ctx {
	struct aspeed_sg sg[HACE_SG_MAX];
	int count;
};

#if IS_ENABLED(CONFIG_SHA_PROG_HW_ACCEL)
int hw_sha_init(struct hash_algo *algo, void **ctxp)
{
	enable_crypto();

	*ctxp = calloc(1, sizeof(struct ctx));

	return 0;
}

int hw_sha_update(struct hash_algo *algo, void *ctx, const void *buf,
			    unsigned int size, int is_last)
{
	struct ctx *c = ctx;
	struct aspeed_sg *sg;

	if (c->count > HACE_SG_MAX)
		return -EINVAL;

	sg = &c->sg[c->count++];
	sg->addr = (uint32_t)buf;
	sg->size = size;

	if (is_last)
		sg->addr |= BIT(31);

	return 0;
}

int hw_sha_finish(struct hash_algo *algo, void *ctx, void *dest_buf,
		     int size)
{
	struct ctx *c = ctx;

	writel((u32)c->sg, ASPEED_HACE_HASH_SRC);
	writel((u32)dest_buf, ASPEED_HACE_HASH_DIGEST_BUFF);
	writel(c->count * sizeof(struct aspeed_sg), ASPEED_HACE_HASH_DATA_LEN);
	writel(HACE_SHA_BE_EN | HACE_SG_EN | HACE_ALGO_SHA512, ASPEED_HACE_HASH_CMD);

	return ast_hace_wait_isr(ASPEED_HACE_STS, HACE_HASH_ISR, 1000);
}
#endif

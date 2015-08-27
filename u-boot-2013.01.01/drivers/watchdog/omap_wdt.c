/*
 * omap_wdt.c
 *
 * (C) Copyright 2013
 * Heiko Schocher, DENX Software Engineering, hs@denx.de.
 *
 * Based on:
 *
 * Watchdog driver for the TI OMAP 16xx & 24xx/34xx 32KHz (non-secure) watchdog
 *
 * Author: MontaVista Software, Inc.
 *	 <gdavis@mvista.com> or <source@mvista.com>
 *
 * 2003 (c) MontaVista Software, Inc. This file is licensed under the
 * terms of the GNU General Public License version 2. This program is
 * licensed "as is" without any warranty of any kind, whether express
 * or implied.
 *
 * History:
 *
 * 20030527: George G. Davis <gdavis@mvista.com>
 *	Initially based on linux-2.4.19-rmk7-pxa1/drivers/char/sa1100_wdt.c
 *	(c) Copyright 2000 Oleg Drokin <green@crimea.edu>
 *	Based on SoftDog driver by Alan Cox <alan@lxorguk.ukuu.org.uk>
 *
 * Copyright (c) 2004 Texas Instruments.
 *	1. Modified to support OMAP1610 32-KHz watchdog timer
 *	2. Ported to 2.6 kernel
 *
 * Copyright (c) 2005 David Brownell
 *	Use the driver model and standard identifiers; handle bigger timeouts.
 */

#include <common.h>
#include <watchdog.h>
#include <asm/arch/hardware.h>
#include <asm/io.h>
#include <asm/processor.h>
#include <asm/arch/cpu.h>


/*
 * Watchdog:
 * Using the prescaler, the OMAP watchdog could go for many
 * months before firing.  These limits work without scaling,
 * with the 60 second default assumed by most tools and docs.
 */
#define TIMER_MARGIN_MAX	(24 * 60 * 60)	/* 1 day */
#define TIMER_MARGIN_DEFAULT	60	/* 60 secs */
#define TIMER_MARGIN_MIN	1

#define PTV			0	/* prescale */
#define GET_WLDR_VAL(secs)	(0xffffffff - ((secs) * (32768/(1<<PTV))) + 1)
#define WDT_WWPS_PEND_WCLR	BIT(0)
#define WDT_WWPS_PEND_WLDR	BIT(2)
#define WDT_WWPS_PEND_WTGR	BIT(3)
#define WDT_WWPS_PEND_WSPR	BIT(4)

#define WDT_WCLR_PRE		BIT(5)
#define WDT_WCLR_PTV_OFF	2


/* Hardware timeout in seconds */
#define WDT_HW_TIMEOUT 60

static unsigned int wdt_trgr_pattern = 0x1234;

void hw_watchdog_reset(void)
{
	struct wd_timer *wdt = (struct wd_timer *)WDT_BASE;

	/* wait for posted write to complete */
	while ((readl(&wdt->wdtwwps)) & WDT_WWPS_PEND_WTGR)
		;

	wdt_trgr_pattern = ~wdt_trgr_pattern;
	writel(wdt_trgr_pattern, &wdt->wdtwtgr);

	/* wait for posted write to complete */
	while ((readl(&wdt->wdtwwps) & WDT_WWPS_PEND_WTGR))
		;
}

static int omap_wdt_set_timeout(unsigned int timeout)
{
	struct wd_timer *wdt = (struct wd_timer *)WDT_BASE;
	u32 pre_margin = GET_WLDR_VAL(timeout);

	/* just count up at 32 KHz */
	while (readl(&wdt->wdtwwps) & WDT_WWPS_PEND_WLDR)
		;

	writel(pre_margin, &wdt->wdtwldr);
	while (readl(&wdt->wdtwwps) & WDT_WWPS_PEND_WLDR)
		;

	return 0;
}

void hw_watchdog_init(void)
{
	struct wd_timer *wdt = (struct wd_timer *)WDT_BASE;

	/* initialize prescaler */
	while (readl(&wdt->wdtwwps) & WDT_WWPS_PEND_WCLR)
		;

	writel(WDT_WCLR_PRE | (PTV << WDT_WCLR_PTV_OFF), &wdt->wdtwclr);
	while (readl(&wdt->wdtwwps) & WDT_WWPS_PEND_WCLR)
		;

	omap_wdt_set_timeout(WDT_HW_TIMEOUT);

	/* Sequence to enable the watchdog */
	writel(0xBBBB, &wdt->wdtwspr);
	while ((readl(&wdt->wdtwwps)) & WDT_WWPS_PEND_WSPR)
		;

	writel(0x4444, &wdt->wdtwspr);
	while ((readl(&wdt->wdtwwps)) & WDT_WWPS_PEND_WSPR)
		;
}

void hw_watchdog_disable(void)
{
	struct wd_timer *wdt = (struct wd_timer *)WDT_BASE;

	/*
	 * Disable watchdog
	 */
	writel(0xAAAA, &wdt->wdtwspr);
	while (readl(&wdt->wdtwwps) != 0x0)
		;
	writel(0x5555, &wdt->wdtwspr);
	while (readl(&wdt->wdtwwps) != 0x0)
		;
}

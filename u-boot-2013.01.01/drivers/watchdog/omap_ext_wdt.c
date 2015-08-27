/******************************************************************************

  Copyright (C), 2005-2014, CVTE.

 ******************************************************************************
  File Name     : omap_ext_wdt.c
  Version       : Initial Draft
  Author        : qiuweibo
  Created       : 2015/6/3
  Last Modified :
  Description   : driver for TPS3828 watchdog
  Function List :
  History       :
  1.Date        : 2015/6/3
    Author      : qiuweibo
    Modification: Created file

******************************************************************************/
#include <common.h>
#include <watchdog.h>
#include <asm/gpio.h>
#include <asm/arch/mux.h>

#ifdef CONFIG_EXTERNAL_WATCHDOG
void ext_watchdog_reset(void)
{
    if (gpio_get_value(GPIO_TO_PIN(1,30)))
    {
        gpio_set_value(GPIO_TO_PIN(1,30), 0);
    }
    else
    {
        gpio_set_value(GPIO_TO_PIN(1,30), 1);
    }
}

void ext_watchdog_init(void)
{
    struct module_pin_mux ext_wdt_pin_mux[] = {
	{OFFSET(gpmc_csn1), (MODE(7) | PULLUDEN)},              /* AM335X_WDI */
	{-1},
    };
    
    configure_module_pin_mux(ext_wdt_pin_mux);
    if (0 == gpio_request(GPIO_TO_PIN(1,30), "ext_wdt"))
    {
        gpio_direction_output(GPIO_TO_PIN(1,30), 0);
        ext_watchdog_reset();
        ext_watchdog_reset();
    }
}

#endif /* CONFIG_EXTERNAL_WATCHDOG */


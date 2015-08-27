/*
 * Copyright 2008, Network Appliance Inc.
 * Author: Jason McMullan <mcmullan <at> netapp.com>
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <malloc.h>
#include <spi_flash.h>

#include "spi_flash_internal.h"

struct gigadevice_spi_flash_params {
	uint16_t	id;
	uint16_t	nr_blocks;
	const char	*name;
};

static const struct gigadevice_spi_flash_params gigadevice_spi_flash_table[] = {
	{
		.id				= 0x4011,
		.nr_blocks		= 2,
		.name			= "GD25Q10",
	},
	{
		.id				= 0x4012,
		.nr_blocks		= 4,
		.name			= "GD25Q20",
	},
	{
		.id 			= 0x4013,
		.nr_blocks		= 8,
		.name			= "GD25Q40",
	},
	{
		.id 			= 0x6013,
		.nr_blocks		= 8,
		.name			= "GD25LQ40",
	},
	{
		.id 			= 0x4014,
		.nr_blocks		= 16,
		.name			= "GD25Q80",
	},
	{
		.id 			= 0x6014,
		.nr_blocks		= 16,
		.name			= "GD25LQ80",
	},
	{
		.id 			= 0x4015,
		.nr_blocks		= 32,
		.name			= "GD25Q16",
	},
	{
		.id 			= 0x6015,
		.nr_blocks		= 32,
		.name			= "GD25LQ16",
	},
	{
		.id 			= 0x4016,
		.nr_blocks		= 64,
		.name			= "GD25Q32",
	},
	{
		.id 			= 0x6016,
		.nr_blocks		= 64,
		.name			= "GD25LQ32",
	},
	{
		.id 			= 0x4017,
		.nr_blocks		= 128,
		.name			= "GD25Q64",
	},
	{
		.id 			= 0x6017,
		.nr_blocks		= 128,
		.name			= "GD25LQ64",
	},
};

struct spi_flash *spi_flash_probe_gigadevice(struct spi_slave *spi, u8 *idcode)
{
	const struct gigadevice_spi_flash_params *params;
	struct spi_flash *flash;
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(gigadevice_spi_flash_table); i++) {
		params = &gigadevice_spi_flash_table[i];
		if (params->id == ((idcode[1] << 8) | idcode[2]))
			break;
	}

	if (i == ARRAY_SIZE(gigadevice_spi_flash_table)) {
		debug("SF: Unsupported Winbond ID %02x%02x\n",
				idcode[1], idcode[2]);
		return NULL;
	}

	flash = malloc(sizeof(*flash));
	if (!flash) {
		debug("SF: Failed to allocate memory\n");
		return NULL;
	}

	flash->spi = spi;
	flash->name = params->name;

	flash->write = spi_flash_cmd_write_multi;
	flash->erase = spi_flash_cmd_erase;
	flash->read = spi_flash_cmd_read_fast;
	flash->page_size = 256;
	flash->sector_size = 4096;
	flash->size = 4096 * 16 * params->nr_blocks;

	return flash;
}

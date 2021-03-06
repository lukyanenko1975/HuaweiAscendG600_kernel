/* Copyright (c) 2012, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _ARCH_ARM_MACH_MSM_OCMEM_CORE_H
#define _ARCH_ARM_MACH_MSM_OCMEM_CORE_H

/** All interfaces in this header should only be used by OCMEM driver
 *  Client drivers should use wrappers available in ocmem.h
 **/

#include "ocmem.h"
#include <mach/msm_iomap.h>
#include <asm/io.h>

#define OCMEM_PHYS_BASE 0xFEC00000
#define OCMEM_PHYS_SIZE 0x180000

struct ocmem_zone;

struct ocmem_zone_ops {
	unsigned long (*allocate) (struct ocmem_zone *, unsigned long);
	int (*free) (struct ocmem_zone *, unsigned long, unsigned long);
};

struct ocmem_zone {
	int owner;
	int active_regions;
	int max_regions;
	struct list_head region_list;
	unsigned long z_start;
	unsigned long z_end;
	unsigned long z_head;
	unsigned long z_tail;
	unsigned long z_free;
	struct gen_pool *z_pool;
	struct ocmem_zone_ops *z_ops;
};

struct ocmem_req {
	struct rw_semaphore rw_sem;
	/* Chain in sched queue */
	struct list_head sched_list;
	/* Chain in zone list */
	struct list_head zone_list;
	int owner;
	int prio;
	uint32_t req_id;
	unsigned long req_min;
	unsigned long req_max;
	unsigned long req_step;
	/* reverse pointers */
	struct ocmem_zone *zone;
	struct ocmem_buf *buffer;
	unsigned long state;
	/* Request assignments */
	unsigned long req_start;
	unsigned long req_end;
	unsigned long req_sz;
};

struct ocmem_handle {
	struct ocmem_buf buffer;
	struct mutex handle_mutex;
	struct ocmem_req *req;
};

struct ocmem_zone *get_zone(unsigned);
unsigned long allocate_head(struct ocmem_zone *, unsigned long);
int free_head(struct ocmem_zone *, unsigned long, unsigned long);
unsigned long allocate_tail(struct ocmem_zone *, unsigned long);
int free_tail(struct ocmem_zone *, unsigned long, unsigned long);
#endif

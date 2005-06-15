/*
 * Copyright (c) 2005 Cisco Systems.  All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * $Id$
 */

#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <endian.h>
#include <byteswap.h>

#include <infiniband/verbs.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN
static inline uint64_t be64_to_cpu(uint64_t x) { return bswap_64(x); }
#elif __BYTE_ORDER == __BIG_ENDIAN
static inline uint64_t be64_to_cpu(uint64_t x) { return x; }
#endif

int main(int argc, char *argv[])
{
	struct dlist *dev_list;
	struct ibv_device *ib_dev;
	struct ibv_context *context;
	struct ibv_device_attr attr;

	dev_list = ibv_get_devices();

	dlist_start(dev_list);
	ib_dev = dlist_next(dev_list);

	if (!ib_dev) {
		fprintf(stderr, "No IB devices found\n");
		return 1;
	}

	context = ibv_open_device(ib_dev);
	if (!context) {
		fprintf(stderr, "Couldn't get context for %s\n",
			ibv_get_device_name(ib_dev));
		return 1;
	}

	if (ibv_query_device(context, &attr)) {
		fprintf(stderr, "Couldn't query device for %s\n",
			ibv_get_device_name(ib_dev));
		return 1;
	}

	printf("%s properties:\n", ibv_get_device_name(ib_dev));
	printf("\tNum ports:\t%d\n", attr.phys_port_cnt);
	printf("\tNode GUID:\t%016llx\n", be64_to_cpu(attr.node_guid));
	printf("\tMax QPs:\t%d\n", attr.max_qp);
	printf("\tMax CQs:\t%d\n", attr.max_cq);
	printf("\tMax PDs:\t%d\n", attr.max_pd);
	printf("\tMax AHs:\t%d\n", attr.max_ah);

	return 0;
}
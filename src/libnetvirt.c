/*
 Libnetvirt - the network virtualization library
 Copyright (C) 2011  Daniel Turull <danieltt@kth.se>

 Libnetvirt is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Libnetvirt is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with libnetvirt.  If not, see <http://www.gnu.org/licenses/>.

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "config.h"
#include "libnetvirt/of-nox.h"
#include "libnetvirt/dummy.h"
#include "libnetvirt/libnetvirt.h"
#include "libnetvirt/mpls.h"

struct libnetvirt_info* libnetvirt_init(int driver) {
	/*Choose correct functions*/
	struct libnetvirt_info* info = malloc(sizeof(struct libnetvirt_info));
	if (info < 0)
		return NULL;
	memset(info, 0, sizeof(struct libnetvirt_info));
	info->driver = driver;
	/*initialize driver*/
	switch (driver) {
	case DRIVER_OF_NOX:
		info->ops.connect = of_nox_connect;
		info->ops.stop = of_nox_stop;
		info->ops.instantiate_fns = of_nox_instantiate_fns;
		info->ops.remove_fns = of_nox_remove_fns;
		info->ops.modify_fns_add = of_nox_modify_fns_add;
		info->ops.modify_fns_del = of_nox_modify_fns_del;
		info->ops.request_ids = of_nox_request_ids;
		break;
	case DRIVER_DUMMY:
		info->ops.connect = dummy_connect;
		info->ops.stop = dummy_stop;
		info->ops.instantiate_fns = dummy_instantiate_fns;
		break;
	case DRIVER_MPLS:
		info->ops.connect = mpls_connect;
		info->ops.stop = mpls_stop;
		info->ops.instantiate_fns = mpls_instantiate_fns;
		info->ops.remove_fns = mpls_remove_fns;
		info->ops.modify_fns_add = mpls_modify_fns_add;
		info->ops.modify_fns_del = mpls_modify_fns_del;
		break;
	}
	return info;
}

int libnetvirt_disconnect(struct libnetvirt_info* info) {
	int status;
	if (!info)
		return -1;
	info->ops.stop();
	wait(&status);
	return 0;
}

int libnetvirt_stop(struct libnetvirt_info* info) {
	int status;
	if (!info)
		return 0;
	info->ops.stop();
	wait(&status);

	free(info);
	return 0;
}
int libnetvirt_connect(struct libnetvirt_info* info, char* address, int port) {
	return info->ops.connect(address, port);
}
int libnetvirt_create_fns(struct libnetvirt_info* info, fnsDesc* fns) {
	if (!fns)
		return -1;
	return info->ops.instantiate_fns(fns);
}
int libnetvirt_modify_fns_add(struct libnetvirt_info* info, fnsDesc* fns) {
	info->ops.modify_fns_add(fns);
	return 0;
}
int libnetvirt_modify_fns_del(struct libnetvirt_info* info, fnsDesc* fns) {
	info->ops.modify_fns_del(fns);
	return 0;
}
int libnetvirt_remove_fns(struct libnetvirt_info* info, fnsDesc* fns) {
	info->ops.remove_fns(fns);
	return 0;
}
int libnetvirt_request_ids(struct libnetvirt_info* info) {
	info->ops.request_ids();
	return 0;
}

fnsDesc* create_local_fns(uint64_t uuid, int nEp, char* name) {

	fnsDesc* ret = NULL;
	int nPath = 0;
	ret = (fnsDesc*) malloc(GET_FNS_SIZE(nEp, nPath));
	if (ret == NULL) {
		fprintf(stderr, "out of memory\n");
		return (NULL);
	}
	memset(ret, 0, GET_FNS_SIZE(nEp, nPath));
	ret->uuid = uuid;
	ret->nEp = nEp;
	if (strlen(name) < 20)
		memcpy(ret->name, name, strlen(name));
	return ret;
}

endpoint* add_local_epoint(fnsDesc* fns, int index, uint64_t uuid,
		uint64_t swId, uint32_t port, uint32_t vlan, uint32_t mpls) {
	if (index < fns->nEp) {
		endpoint* entry = GET_ENDPOINT(fns, index);
		entry->uuid = uuid;
		entry->swId = swId;
		entry->port = port;
		entry->mpls = mpls;
		entry->vlan = vlan;
		return entry;
	}
	return NULL;
}

uint16_t getNepFromFNS(fnsDesc* fns) {
	return fns->nEp;
}
uint64_t getUuidFromFNS(fnsDesc* fns) {
	return fns->uuid;
}

endpoint* getEndpoint(fnsDesc *fns, int pos) {
	return GET_ENDPOINT(fns, pos);
}

uint64_t getUuidFromEp(endpoint* ep) {
	return ep->uuid;
}

uint64_t getSwIdFromEp(endpoint* ep) {
	return ep->swId;
}

uint16_t getPortFromEp(endpoint* ep) {
	return ep->port;
}

uint32_t getMplsFromEp(endpoint* ep) {
	return ep->mpls;
}

uint16_t getVlanFromEp(endpoint* ep) {
	return ep->vlan;
}

char* getNetFromEp(endpoint* ep) {
	return ep->address;
}
uint8_t getMaskFromEp(endpoint* ep) {
	return ep->mask;
}

char* getAddressPEFromEp(endpoint* ep) {
	return ep->address;
}

char* getAddressCEFromEp(endpoint* ep) {
	return ep->addressEx;
}

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

#ifndef LIBNETVIRT_HH_
#define LIBNETVIRT_HH_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define DRIVER_OF_NOX 1
#define DRIVER_MPLS 2
#define DRIVER_DUMMY 3

#define LIBNETVIRT_FORWARDING_L2 2
#define LIBNETVIRT_FORWARDING_L3 3
#define LIBNETVIRT_FORWARDING_L3VPN 4

#define LIBNETVIRT_CONSTRAINT_MINBW 1
#define LIBNETVIRT_CONSTRAINT_MAXBW 2

#define MAX_NAME_SIZE 20

typedef struct EndPoint {
	uint64_t uuid;
	uint64_t swId;
	uint16_t port;
	uint32_t mpls;
	uint16_t vlan;
	char address[MAX_NAME_SIZE];
//	uint8_t pad[2];
} endpoint;

typedef struct constraint {
	uint8_t type;
	uint64_t src;
	uint64_t dst;
	uint32_t value;
} constraint;

typedef struct fns_desc {
	char name[MAX_NAME_SIZE];
	uint64_t uuid;
	uint16_t nEp;
	uint16_t nCons;
	uint8_t forwarding;
	uint8_t data[];
} fnsDesc;

#define GET_ENDPOINT(fns, i) (endpoint *) (&fns->data[i*sizeof(endpoint)])
#define GET_CONSTRAINT(fns, i) (constraint *) (&fns->data[fns->nEp * sizeof(endpoint) + i * sizeof(constraint)])
#define GET_FNS_SIZE(nEp, nCons) (sizeof(fnsDesc) + sizeof(endpoint) * nEp + sizeof(constraint) * nCons)
#define GET_VN_SIZE(vn) (sizeof(fnsDesc) + sizeof(endpoint) * vn->nEp + sizeof(constraint) * vn->nCons)

/*Operations*/
struct libnetvirt_ops {
	int (*connect)(char* addr, int port);
	int (*add_fns)(fnsDesc* info);
	int (*instantiate_fns)(fnsDesc* fns);
	int (*remove_fns)(fnsDesc* fns);
	int (*modify_fns_add)(fnsDesc* fns);
	int (*modify_fns_del)(fnsDesc* fns);
	int (*stop)(void);
	int (*request_ids)(void);

};

struct libnetvirt_info {
	int driver;

	struct libnetvirt_ops ops;
};

/**
 *	Start the library internal structures.
 */
struct libnetvirt_info* libnetvirt_init(int driver);

/**
 *	Stop and free all the iternal structures defined in the libnetvirt_info;
 */
int libnetvirt_stop(struct libnetvirt_info* info);

int libnetvirt_connect(struct libnetvirt_info* info, char* address, int port);
int libnetvirt_disconnect(struct libnetvirt_info* info);
int libnetvirt_create_fns(struct libnetvirt_info* info, fnsDesc* fns);
int libnetvirt_modify_fns_add(struct libnetvirt_info* info, fnsDesc* fns);
int libnetvirt_modify_fns_del(struct libnetvirt_info* info, fnsDesc* fns);
int libnetvirt_remove_fns(struct libnetvirt_info* info, fnsDesc* fns);
int libnetvirt_request_ids(struct libnetvirt_info* info);

fnsDesc* parse_fns(char* filename);
fnsDesc* parse_fns_Mem(const char *content, int length);

endpoint* add_local_epoint(fnsDesc* fns, int index, uint64_t uuid,
		uint64_t swId, uint32_t port, uint32_t vlan, uint32_t mpls);
endpoint* add_local_epoint_l3(fnsDesc* fns, int index, uint64_t uuid,
		uint64_t swId, uint32_t port, uint32_t vlan, char* net);
fnsDesc* create_local_fns(uint64_t uuid, int nEp, char* name, uint8_t type);

/* FNS GETs */
uint16_t getNepFromFNS(fnsDesc* fns);
uint64_t getUuidFromFNS(fnsDesc* fns);

/* Endpoint GETs*/
endpoint* getEndpoint(fnsDesc *fns, int pos);
uint64_t getUuidFromEp(endpoint* ep);
uint64_t getSwIdFromEp(endpoint* ep);
uint16_t getPortFromEp(endpoint* ep);
uint32_t getMplsFromEp(endpoint* ep);
uint16_t getVlanFromEp(endpoint* ep);
char* getAddressFromEp(endpoint* ep);

extern void printFNS(fnsDesc *cur);
/*Substrate functions*/
//int define_phy_topology(struct libnetvirt_info*, const char* xml);
//void enable_phy_discovery(int bool);
//struct topology* get_phy_topology_view(struct libnetvirt_info* info);

//int add_phy_endpoint(struct node_info *endpoint);
//struct node_info* get_phy_endpoints();

/*FNS manipulation*/
//char* read_fns_definition(const char* f);
//struct node_info* get_fns_endpoints();
//struct topology* get_fns_topology_view(struct fns_info* fns);
//void get_fns_stats();
//int add_static_path(struct path* path);

//int compute_path();
//int select_encapsulation(int option);

//extern fnsDesc* parse_fns(char* filename);

#ifdef __cplusplus
}
#endif

#endif

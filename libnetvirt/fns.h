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

#ifndef FNS_HH_
#define FNS_HH_

#ifdef __cplusplus
extern "C" {
#endif 


#define MAX_ENDPOINTS 5

#define TCP_PORT  2000
#define MSG_SIZE  1480
#define MAX_CONNECTIONS  1
#define SELECT_TIMEOUT 2

#define FNS_MSG_ADD 1
#define FNS_MSG_DEL 2
#define FNS_MSG_MOD_ADD 3
#define FNS_MSG_MOD_DEL 4
#define FNS_MSG_SW_IDS 5

#define FNS_FWD_BROADCAST 1
#define FNS_FWD_ARP_PROXY 2
#define FNS_FWD_ARP_BROADCAST 3
#define FNS_FWD_L2_LEARNING 4
#define FNS_FWD_L3_FORWARDNG 5

#include <stdint.h>
#include "libnetvirt/libnetvirt.h"
struct msg_fns {
	/*Common header*/
	uint8_t type;
	uint16_t size;
	/*Payload*/
	fnsDesc fns;
};

struct msg_ids {
	uint8_t type;
	uint16_t nEp;
	endpoint endpoints[];
};


/*Helper functions*/




#ifdef __cplusplus
}
#endif 

#endif //FNS__HH_

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

#ifndef FNS_MSG_HH_
#define FNS_MSG_HH_

#ifdef __cplusplus
extern "C" {
#endif 
#include <stdint.h>
static const uint32_t TCP_PORT = 2000;
static const int MSG_SIZE = 1480;
static const uint32_t MAX_CONNECTIONS = 1;

#define FNS_MSG_LINK 1
#define FNS_MSG_SWITCH_DESC 2
typedef uint64_t id_sw;
enum LINK_MSG {
	ADD, REMOVE
};

struct msg_hdr {
	uint8_t type;
	uint16_t size;
};

struct msg_sw_info {
	int numPorts;
	id_sw id;
};

struct msg_link {
	/*Common header*/
	uint8_t type;
	uint16_t size;
	/*Payload*/
	id_sw dpsrc;
	id_sw dpdst;
	uint16_t sport;
	uint16_t dport;
	uint8_t action;
};



#define SIZE_MSG_LINK sizeof(msg_hdr) + sizeof(msg_link);

#ifdef __cplusplus
}
#endif 

#endif //FNS_MSG_HH_ 

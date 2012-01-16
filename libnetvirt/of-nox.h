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

#ifndef OFNOX_H_
#define OFNOX_H_

#include <pthread.h>
#include "libnetvirt/fns.h"

struct of_nox_info{
	int sockfd;
	int sock_listen_fd;
	pthread_t pth;
	int run;
};

int of_nox_connect(char* addr, int port);
int of_nox_stop(void);
int of_nox_instantiate_fns(fnsDesc *desc);
int of_nox_remove_fns(fnsDesc *desc);
int of_nox_modify_fns_add(fnsDesc *desc);
int of_nox_modify_fns_del(fnsDesc *desc);
int of_nox_request_ids(void);

#endif /* OFNOX_H_ */

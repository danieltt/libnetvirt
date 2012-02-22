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

#ifndef DUMMY_H_
#define DUMMY_H_

#include <pthread.h>
#include "libnetvirt/fns.h"

int dummy_connect(char* addr, int port);
int dummy_stop(void);
int dummy_instantiate_fns(fnsDesc *desc);
int dummy_remove_fns(fnsDesc *desc);
int dummy_modify_fns_add(fnsDesc *desc);
int dummy_modify_fns_del(fnsDesc *desc);
int dummy_request_ids(void);

#endif /* dummy_H_ */

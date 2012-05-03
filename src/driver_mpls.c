/*
 Libnetvirt - the network virtualization library
 Copyright (C) 2012  Daniel Turull <danieltt@kth.se>

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

#include "libnetvirt/mpls.h"
#include "python_helper.h"



#define MPLS_SCRIPT "driver_mpls"

struct mpls_info {
	PyObject *pModule;
} info_mpls;

int mpls_connect(char* addr, int port) {
	return python_call_init(MPLS_SCRIPT);

}
int mpls_stop(void) {

	return python_stop();
}

int mpls_instantiate_fns(fnsDesc *desc) {
	return python_call_fns("mpls_create_fns",desc);
}
int mpls_remove_fns(fnsDesc *desc) {
	return python_call_fns("mpls_remove_fns",desc);
}
int mpls_modify_fns_add(fnsDesc *desc) {
	return python_call_fns("mpls_modify_fns_add",desc);
}
int mpls_modify_fns_del(fnsDesc *desc) {
	return python_call_fns("mpls_modify_fns_del",desc);
}
int mpls_request_ids(void) {
	return 0;
}

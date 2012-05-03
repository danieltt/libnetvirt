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

#ifndef PY_HELPER_HH_
#define PY_HELPER_HH_
#include "libnetvirt/fns.h"
#include <Python.h>

PyObject *pModule;
int python_call_fns(char* function,fnsDesc *desc);
int python_call_init(char* script_name);
int python_stop(void);

#endif

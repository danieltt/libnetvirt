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

#include "libnetvirt/libnetvirt.h"
#include "libnetvirt/dummy.h"
#include "libnetvirt/fns.h"

#include <Python.h>

#define DUMMY_SCRIPT "driver_dummy"

struct dummy_info {
	PyObject *pModule;
} info_dummy;

int dummy_connect(char* addr, int port) {
	PyObject *pName, *pFunc, *pValue;
	if (Py_IsInitialized())
		return -1;

	printf("Starting python interpreter\n");
	Py_Initialize();
	pName = PyString_FromString(DUMMY_SCRIPT);
	info_dummy.pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (info_dummy.pModule != NULL) {
		pFunc = PyObject_GetAttrString(info_dummy.pModule, "dummy_connect");
		if (pFunc && PyCallable_Check(pFunc)) {
			pValue = PyObject_CallFunctionObjArgs(pFunc, PyString_FromString("localhost"), PyInt_FromLong(12),NULL);
			if (pValue != NULL) {
				printf("Result of call: %ld\n", PyInt_AsLong(pValue));
				Py_DECREF(pValue);
			} else {
				Py_DECREF(pFunc);
				PyErr_Print();
				fprintf(stderr, "Call failed\n");
				return 1;
			}
		} else {
			if (PyErr_Occurred())
				PyErr_Print();
			fprintf(stderr, "Cannot find function \n");
		}
		Py_XDECREF(pFunc);

	}else{
		PyErr_Print();
		fprintf(stderr, "Failed to load \"%s\"\n", DUMMY_SCRIPT);
	}
	return 0;
}
int dummy_stop(void) {

	if (Py_IsInitialized()) {
		if(info_dummy.pModule)
			Py_DECREF(info_dummy.pModule);

		Py_Finalize();
	}
	return 0;
}
int dummy_instantiate_fns(fnsDesc *desc) {
	return 0;
}
int dummy_remove_fns(fnsDesc *desc) {
	return 0;
}
int dummy_modify_fns_add(fnsDesc *desc) {
	return 0;
}
int dummy_modify_fns_del(fnsDesc *desc) {
	return 0;
}
int dummy_request_ids(void) {
	return 0;
}

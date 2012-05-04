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

#include "python_helper.h"
#include "libnetvirt_wrap.c"

int python_call_init(char* script_name) {
	PyObject *pName;
	if (Py_IsInitialized()){
		perror("error initializing python");
		return -1;
	}
	printf("Starting python interpreter\n");
	Py_Initialize();

	pName = PyString_FromString(script_name);
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	return 0;
}

int python_stop(void) {

	if (Py_IsInitialized()) {
		if (pModule)
			Py_DECREF(pModule);

		Py_Finalize();
	}
	return 0;
}

int python_call_fns(char* function,fnsDesc *desc){
	PyObject *pFunc = NULL, *pValue= NULL, *pArg = NULL;
		int ret=0;
		if (!Py_IsInitialized())
			return -1;
		SWIG_init();		// Initialise SWIG types

		if (pModule != NULL) {
			pFunc = PyObject_GetAttrString(pModule, function);
			if (pFunc && PyCallable_Check(pFunc)) {
				pArg = SWIG_NewPointerObj(SWIG_as_voidptr(desc), SWIGTYPE_p_fns_desc, SWIG_POINTER_NEW |  0 );
				if (pArg) {
					pValue = PyObject_CallFunction(pFunc, "O", pArg);
				}
				if (pValue != NULL) {
					ret =  PyInt_AsLong(pValue);
					//printf("Result of call: %d\n",ret);
					Py_DECREF(pValue);
				} else {
					Py_DECREF(pFunc);
					PyErr_Print();
					fprintf(stderr, "Call failed\n");
					return -1;
				}
			} else {
				if (PyErr_Occurred())
					PyErr_Print();
				fprintf(stderr, "Cannot find function \n");
			}
			Py_XDECREF(pFunc);

		} else {
			PyErr_Print();
			fprintf(stderr, "Failed to load \"%s\"\n");
		}
		return ret;
}

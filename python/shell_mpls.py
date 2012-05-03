#!/usr/bin/python

# Copyright (C) 2011 Daniel Turull - KTH Royal Institute of Technology
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library.  If not, see <http://www.gnu.org/licenses/>.


import libnetvirt
info = libnetvirt.libnetvirt_init(libnetvirt.DRIVER_MPLS)
for num in range(1,10000):
	libnetvirt.libnetvirt_connect(info, "127.0.0.1",2000) 
fns_xml ="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
	<description xmlns=\"http://www.sail-project.eu/fns\">\
	<fns name=\"two-fns\" uuid=\"1\">\
		<endpoint uuid=\"3\"> <swId>3</swId> <port>2</port> </endpoint>\
		<endpoint uuid=\"2\"> <swId>2</swId><port>2</port> </endpoint>\
		<endpoint uuid=\"4\"> <swId>4</swId><port>1</port> </endpoint>\
	</fns>\
	</description>"
fns = libnetvirt.parse_fns_Mem(fns_xml,len(fns_xml))
#fns = libnetvirt.parse_fns("fns.xml")
libnetvirt.libnetvirt_create_fns(info,fns)

fns1 = libnetvirt.create_local_fns(20,2,"fns_api")
ep1 = libnetvirt.add_local_epoint(fns1,0,10,3,2,10,0)
ep2 = libnetvirt.add_local_epoint(fns1,1,20,2,2,20,0)
#print ep1

libnetvirt.printFNS(fns1)
libnetvirt.libnetvirt_create_fns(info,fns1)
libnetvirt.libnetvirt_remove_fns(info,fns1)

libnetvirt.libnetvirt_disconnect(info)


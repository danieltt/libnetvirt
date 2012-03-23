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
import sys
from datetime import datetime

info = libnetvirt.libnetvirt_init(libnetvirt.DRIVER_OF_NOX)
errors = 0
address="127.0.0.1"
if (len(sys.argv) == 2):
	n = int(sys.argv[1])
elif (len(sys.argv) == 3):
	n = int(sys.argv[1])
	address = sys.argv[2]
else:
	print "use %s [n] [ip]" % ( sys.argv[0],)
	exit(0)
	
start = datetime.now()
for num in range(1,n):
	libnetvirt.libnetvirt_connect(info, address,2000) 
	fns_xml ="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
	<description xmlns=\"http://www.sail-project.eu/fns\">\
	<fns name=\"testing\" uuid=\"%d\">\
		<endpoint uuid=\"%d \"> <swId>%d</swId> <port>1</port> </endpoint>\
		<endpoint uuid=\"%d \"> <swId>%d</swId><port>2</port> </endpoint>\
		<forward>L2</forward>\
	</fns>\
	</description>" % (num, num, num, num, num,)
	
	fns = libnetvirt.parse_fns_Mem(fns_xml,len(fns_xml))
	#fns = libnetvirt.parse_fns("fns.xml")
	if (libnetvirt.libnetvirt_create_fns(info,fns) < 0):
		errors = errors +1
	libnetvirt.libnetvirt_disconnect(info)

elapsed = (datetime.now() - start)

print "errors %d/%d time %s" % (errors, n, elapsed)
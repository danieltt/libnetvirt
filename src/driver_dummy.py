# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

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
import socket,struct

def dummy_connect(addr,port):
    print "connect to " +addr
    return 0

def dummy_stop():
    print "stop"
    
def dummy_create_fns(desc):
    print "create fns"
    libnetvirt.printFNS(desc)
    print "FNS uuid ", libnetvirt.getUuidFromFNS(desc)
    for i in range(0,libnetvirt.getNepFromFNS(desc)):

        ep = libnetvirt.getEndpoint(desc,i)
        print "ep uuid", libnetvirt.getUuidFromEp(ep)
        print "\t swid ", libnetvirt.getSwIdFromEp(ep)
        print "\t port", libnetvirt.getPortFromEp(ep)
        print "\t vlan", libnetvirt.getVlanFromEp(ep)
        print "\t PE: ", socket.inet_ntoa(struct.pack('I', libnetvirt.getAddressPEFromEp(ep)))
        print "\t CE:", socket.inet_ntoa(struct.pack('I', libnetvirt.getAddressCEFromEp(ep)))

    print "create"
    return 0
    
def dummy_remove_fns(desc):
    print "remove"
    
def dummy_modify_fns_add(desc):
    print "modify add "
        
def dummy_modify_fns_del(desc):
    print "modify del"
    
def dummy_request_ids():
    print ids
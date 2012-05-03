# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# Copyright (C) 2012 Daniel Turull - KTH Royal Institute of Technology



import libnetvirt
import socket,struct

import NetworkManager

def mpls_connect(addr,port):
    print "connect to " +addr
    return 0

def mpls_stop():
    print "stop"
    
def mpls_create_fns(desc):
    print "create fns"
    libnetvirt.printFNS(desc)
    print "FNS uuid ", libnetvirt.getUuidFromFNS(desc)
    for i in range(0,libnetvirt.getNepFromFNS(desc)):

        ep = libnetvirt.getEndpoint(desc,i)
        print "ep uuid", libnetvirt.getUuidFromEp(ep)
        print "\t swid ", libnetvirt.getSwIdFromEp(ep)
        print "\t PE: ", socket.inet_ntoa(struct.pack('I', libnetvirt.getAddressPEFromEp(ep)))
        print "\t CE:", socket.inet_ntoa(struct.pack('I', libnetvirt.getAddressCEFromEp(ep)))
        
        #call Kalle's script

    return 0
    
def mpls_remove_fns(desc):
    print "remove"
    
def mpls_modify_fns_add(desc):
    print "modify add "
        
def mpls_modify_fns_del(desc):
    print "modify del"
    
def mpls_request_ids():
    print ids
    
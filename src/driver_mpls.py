# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# Copyright (C) 2012 Daniel Turull - KTH Royal Institute of Technology



import libnetvirt

import commands

from NetworkManager import NetworkManager,Database



def mpls_connect(addr,port):
    print "connect to " +addr
    return 0

def mpls_stop():
    print "stop"
    
def mpls_create_fns(desc):
    print "create fns"
    #libnetvirt.printFNS(desc)
    fns_id= libnetvirt.getUuidFromFNS(desc)
    for i in range(0,libnetvirt.getNepFromFNS(desc)):

        ep = libnetvirt.getEndpoint(desc,i)
        r_id = libnetvirt.getSwIdFromEp(ep)
        D = Database ('libnetvirt.sqlite')
        # Get router name
        #r_name = str(libnetvirt.getSwIdFromEp(ep))
        r_name = D.getRouterName(r_id)

        # Get the router user name
        r_uname = D.getRouterUserName(r_id)

        # Get the router interface
        r_interface = D.getRouterInterface(r_id)+str(libnetvirt.getPortFromEp(ep))

        #r_uname = "root"
        #r_interface = "eth3"
        r_d_net = '192.66.23.0'
        
        pe_address = libnetvirt.getAddressFromEp(ep)
        #ce_address = libnetvirt.getAddressCEFromEp(ep)
        
        #we need net address for CE
        tmp = commands.getoutput('ipcalc -n ' + str(pe_address))
        tmp = tmp.split()
        ce_net = tmp[16]
        
        vlan =  libnetvirt.getVlanFromEp(ep)
        vrf = 'vrf' +  str(fns_id)
        
        #get address from global configuration database
        r_d =  r_d_net + ':' + str(fns_id)
        
        #call script for PE configuration
        #Calling the scripts for configuration. Uncomment when the network is available
        NM = NetworkManager(r_name, r_uname)
        NM.start_configuration(r_interface, pe_address, ce_net, r_d, vlan, vrf)
        NM.close_ssh()


    return 0
    
def mpls_remove_fns(desc):
    print "remove"
    #libnetvirt.printFNS(desc)
    fns_id= libnetvirt.getUuidFromFNS(desc)
    for i in range(0,libnetvirt.getNepFromFNS(desc)):

        ep = libnetvirt.getEndpoint(desc,i)
        r_id = libnetvirt.getSwIdFromEp(ep)
        D = Database ('libnetvirt.sqlite')
        # Get router name
        #r_name = str(libnetvirt.getSwIdFromEp(ep))
        r_name = D.getRouterName(r_id)
        # Get the router user name
        r_uname = D.getRouterUserName(r_id)             
        vlan =  libnetvirt.getVlanFromEp(ep)
        vrf = 'vrf' +  str(fns_id)
        
        #Calling the scripts for configuration. Uncomment when the network is available
        NM = NetworkManager(r_name, r_uname)
        NM.stop_configuration(vlan, vrf)
        NM.close_ssh()
        


    return 0
    
def mpls_modify_fns_add(desc):
    print "modify add "
    #get current fns
    
    mpls_create_fns(desc)
        
def mpls_modify_fns_del(desc):
    print "modify del"
    
def mpls_request_ids():
    print ids
    
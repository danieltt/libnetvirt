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

def dummy_connect(addr,port):
    print "connect to " +addr
    return 0

def dummy_stop():
    print "stop"
    
def dummy_instantiate_fns(desc):
    print "create"
    
def dummy_remove_fns(desc):
    print "remove"
    
def dummy_modify_fns_add(desc):
    print "modify add "
        
def dummy_modify_fns_del(desc):
    print "modify del"
    
def dummy_request_ids():
    print ids
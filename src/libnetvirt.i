/*
 Libnetvirt - the network virtualization library
 Copyright (C) 2011  Daniel Turull <danieltt@kth.se>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

%module libnetvirt

%{
 /* Includes the header in the wrapper code */
 #define SWIG_PYTHON_EXTRA_NATIVE_CONTAINERS 
 #include "libnetvirt.h"
 %}
 
 /* Parse the header file to generate wrappers */
%include "libnetvirt.h"
%include <stdint.i>

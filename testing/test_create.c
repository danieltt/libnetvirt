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

#include "libnetvirt/libnetvirt.h"
#include "libnetvirt/fns.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>

#define BUFFER_SIZE 1<<16
#define ARR_SIZE 1<<16
#define PORT_NOX 2000


int main(int argc, char *argv[]) {
	int i,n ;

	struct libnetvirt_info* info;

	info = libnetvirt_init(DRIVER_OF_NOX);

	fnsDesc* fns = NULL;
	char* address="127.0.0.1";
	if (argc == 2)
		n = atoi(argv[1]);
	else if (argc == 3)
	{
		n = atoi(argv[1]);
		address = argv[2];
	}
	else
		exit(0);

	fnsDesc* fns2;
	int errors=0;
	int value = 1;
	struct timespec start, finish;
	double elapsed;

	clock_gettime(CLOCK_MONOTONIC, &start);

	for (i=0;i<n;i++) {
		libnetvirt_connect(info, address,2000);
		value = i + 1;
		fns2 = create_local_fns(i, 2,"name-testing");
		add_local_epoint(fns2, 0, value, value, 1, 0, 0); /*fns, pos, uuid, swid, port, vlan, mpls */
		add_local_epoint(fns2, 1, value, value, 2, 0, 0);

		if (fns2){
			//printFNS(fns2);
			if(libnetvirt_create_fns(info, fns2))
				errors++;
			free(fns2);
		}
		libnetvirt_disconnect(info);
	}

	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

	printf("function: errors %d/%d. time: %5.2f\n",errors,n,elapsed);


/*
	while (1) {
		printf("$ ");
		if (!fgets(buffer, BUFFER_SIZE, stdin))
			break;
		parse_args(buffer, args, ARR_SIZE, &nargs);

		if (nargs == 0)
			continue;
		if (!strcmp(args[0], "exit"))
			break;

		if (!strcmp(args[0], "connect")) {
			int port = PORT_NOX;
			char* address="127.0.0.1";
			if (nargs == 3){
				port = atoi(args[2]);
				address = args[1];
			}


		}
		if (!strcmp(args[0], "create")) {
			fns = parse_fns(args[1]);
			if (fns){
				libnetvirt_create_fns(info, fns);
				printFNS(fns);
			}
		}
		if (!strcmp(args[0], "view")) {
			fns = parse_fns(args[1]);
			if (fns)
				printFNS(fns);
		}
		if (!strcmp(args[0], "modify_add")) {
			fns = parse_fns(args[1]);
			if (fns){
				printFNS(fns);
				libnetvirt_modify_fns_add(info, fns);
			}
		}
		if (!strcmp(args[0], "modify_del")) {
			fns = parse_fns(args[1]);
			if (fns){
				printFNS(fns);
				libnetvirt_modify_fns_del(info, fns);
			}
		}
		if (!strcmp(args[0], "remove")) {
			fns = parse_fns(args[1]);
			if (fns)
				libnetvirt_remove_fns(info, fns);
		}
		if (!strcmp(args[0], "request_ids")) {
			if (fns)
				libnetvirt_request_ids(info);
		}
		if (!strcmp(args[0], "help")) {
			usage();
		}
		if (!strcmp(args[0], "memory")) {
			fns = parse_fns_Mem(fns_xml, strlen(fns_xml));
			if (fns)
				info->ops.instantiate_fns(fns);
		}
		if (!strcmp(args[0], "parse")) {
			fns = parse_fns(args[1]);
		}
		if (!strcmp(args[0], "disconnect")) {

			if(fns)
				free(fns);
		}

	}
*/
	puts("Finishing");
	if(fns)
		free(fns);
	if(info)
	libnetvirt_stop(info);
	return 0;

}

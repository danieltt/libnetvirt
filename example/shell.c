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

#define BUFFER_SIZE 1<<16
#define ARR_SIZE 1<<16
#define PORT_NOX 2000

void usage(void) {
	puts("create: sends request to server");
	puts("connect [ip] [port]: connect to server");
	puts("remove: remove fns from server");
	puts("modify_add: add endpoints to a current fns");
	puts("modify_del: remove the given endpoints of a current fns");
	puts("request_ids: requests the IDs of the nodes");
	puts("view fns description from a file");
	puts("exit");
}

void parse_args(char *buffer, char** args, size_t args_size, size_t *nargs) {
	char *buf_args[args_size]; /* You need C99 */
	char **cp;
	char *wbuf;
	size_t i, j;

	wbuf = buffer;
	buf_args[0] = buffer;
	args[0] = buffer;

	for (cp = buf_args; (*cp = strsep(&wbuf, " \n\t")) != NULL;) {
		if ((*cp != '\0') && (++cp >= &buf_args[args_size]))
			break;
	}

	for (j = i = 0; buf_args[i] != NULL; i++) {
		if (strlen(buf_args[i]) > 0)
			args[j++] = buf_args[i];
	}

	*nargs = j;
	args[j] = NULL;
}

int main(int argc, char *argv[]) {
	char buffer[BUFFER_SIZE];
	char *args[ARR_SIZE];
	char *driver = "openflow";

	size_t nargs;

	struct libnetvirt_info* info;

	if(argc==2){
		driver=argv[1];
	}

	if (!strcmp(driver,"openflow"))
		info = libnetvirt_init(DRIVER_OF_NOX);
	else if (strcmp(driver,"mpls")==0)
		info = libnetvirt_init(DRIVER_MPLS);

	if(!info){
		printf("Error loading driver\n");
		exit(-1);
	}

	printf("LibNetVirt CLI\nUsing driver: %s\n", driver);

	fnsDesc* fns = NULL;

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

			libnetvirt_connect(info, address, port);
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

		if (!strcmp(args[0], "parse")) {
			fns = parse_fns(args[1]);
		}
		if (!strcmp(args[0], "disconnect")) {
			libnetvirt_disconnect(info);
		}

	}

	puts("Finishing");
	if(fns)
		free(fns);
	if(info)
		libnetvirt_stop(info);
	return 0;

}

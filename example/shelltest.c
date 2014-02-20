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
#define MAX_END 20

void usage(void) {
	puts("create: sends request to server");
	puts("connect: connect to server");
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
	char
			*fns_xml =
					"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
			<description xmlns=\"http://www.sail-project.eu/fns\">\
			<fns name=\"two-fns\" uuid=\"1\">\
				<endpoint id=\"3\">	\
					<port>2</port>\
				</endpoint>\
				<endpoint id=\"2\">\
					<port>2</port>\
				</endpoint>\
				<endpoint id=\"4\">\
					<port>1</port>\
				</endpoint>\
			</fns>\
			</description>";

	size_t nargs;

	struct libnetvirt_info* info;

	puts("LibNetVirt test\n");
	info = libnetvirt_init(DRIVER_OF_NOX);

	/*Test parse XML for FNS*/
	//	libnetvirt_connect(info, "127.0.0.1", 2000);

	fnsDesc* fnsA[20];
	//	fnsDesc* fns1 = NULL;
	int num=6+99+254;
	
	int iter=num/MAX_END;
	int index=0;
	int j;

	for (j=0;j<iter;j++){
		printf("Creating fns %d\n",j);
		fnsA[j] = create_local_fns(10, MAX_END ,"name",LIBNETVIRT_FORWARDING_L2);
	}	
	fnsA[j] = create_local_fns(10, num%MAX_END ,"name",LIBNETVIRT_FORWARDING_L2);
	printf("Creation done\n");
	fnsDesc* fns = fnsA[index];
	add_local_epoint(fns, 0, 1, 1, 1, 65535, 0); /*fns, pos, id, port, vlan,mpls */
	add_local_epoint(fns, 1, 2, 2, 1, 65535, 0);
	add_local_epoint(fns, 2, 3, 3, 1, 65535, 0);
	add_local_epoint(fns, 3, 4, 4, 1, 65535, 0);
	
	add_local_epoint(fns, 4, 5, 1, 3, 65535, 0);
	add_local_epoint(fns, 5, 6, 5, 101, 65535, 0);
	
	int id=7;
	int pos=6;
	int i;
	int total=pos;
	index=0;
	printf("Adding first loop\n");
	for (i=2;i<102;i++){
		if(total%MAX_END==0){
			printf("New set\n");
			index++;
			fns = fnsA[index];
			pos=0;
		}
		add_local_epoint(fns, pos, id, 5, i, 65535, 0);
		pos++;
		id++;
		total++;
	}
	for (i=2;i<256;i++){
		if(total%MAX_END==0){
			printf("New set\n");
			index++;
			fns = fnsA[index];
			pos=0;
		}
		add_local_epoint(fns, pos, id, 6, i, 65535, 0);
		pos++;
		id++;
		total++;
	}

	if (fns){

		libnetvirt_connect(info,"127.0.0.1",PORT_NOX);
		printFNS(fnsA[0]);
		libnetvirt_create_fns(info,fnsA[0]);
		for(j=1;j<iter+1;j++){
		//libnetvirt_remove_fns(info, fns);

			sleep(1);
			fns= fnsA[j];
			printFNS(fns);
			libnetvirt_modify_fns_add(info,fns);

			printf("size%lu\n",sizeof(*fns)*8);
	

		}
	}
	exit(0);
	//	fns = parse_fns("fns2.xml");
	//	if (fns)
	//		info->ops.instantiate_fns(fns);
	//	fns = parse_fns("fns-add.xml");
	//	if (fns)
	//		info->ops.modify_fns_add(fns);
	/*Wait input from user*/
	//fnsDesc* fns2;
	//fns2 = create_local_fns(10, 2,"name");
	//add_local_epoint(fns2, 0, 30, 3, 2, 0, 0); /*fns, pos, id, port, mpls */
	//add_local_epoint(fns2, 1, 20, 2, 2, 0, 0);

	//if (fns2)
	//	printFNS(fns2);
	//	libnetvirt_create_fns(info, fns2);

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
			if (nargs == 2)
				port = atoi(args[1]);

			libnetvirt_connect(info, "127.0.0.1", port);
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
			libnetvirt_disconnect(info);
			if(fns)
				free(fns);
		}

	}

	puts("Finishing");
	if(fns)
		free(fns);
	if(info)
	libnetvirt_stop(info);
	return 0;

}

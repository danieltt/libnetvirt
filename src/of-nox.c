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

#include <config.h>
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
//#include "libnetvirt/fns-msg.h"

#include "libnetvirt/libnetvirt.h"
#include "libnetvirt/of-nox.h"
#include "libnetvirt/fns.h"

#define SELECT_TIMEOUT 2

void process_ids_msg(void *buf);
void* process_events(void *arg);

static struct of_nox_info of_nox_info;


int start_server(char *address, int portno) {
//	socklen_t clilen;
	struct sockaddr_in serv_addr;
	int yes = 1;
	int server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock_fd < 0) {
		perror("ERROR opening socket");
		return -1;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(address);
	serv_addr.sin_port = htons(portno);

	if (setsockopt(server_sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
			== -1) {
		perror("ERROR on setsockopt()");
		return -1;
	}

	/*Connect to server*/
	if (connect(server_sock_fd, (struct sockaddr *) &serv_addr,
			sizeof(serv_addr)) == -1) {
		perror("ERROR in connect");
		return -1;
	}
	of_nox_info.sockfd = server_sock_fd;

	/*Start listening thread*/
	//pthread_create(&(of_nox_info.pth), NULL, process_events, 0);

	return 0;
}

void* process_events(void *arg) {

	fd_set master; /* master file descriptor list */
	fd_set read_fds; /* temp file descriptor list for select() */

//	struct sockaddr_in clientaddr; /* client address */
	int nbytes;
//	int addrlen;
	struct timeval tv;

	void* buf = (void*) malloc(MSG_SIZE);
	if (buf == NULL)
		perror("ERROR in malloc");
	/* clear the master and temp sets */
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	int s = of_nox_info.sockfd;

	FD_SET(s, &master); /* add to master set */
	while (of_nox_info.run) {
		/* copy it */
		read_fds = master;

		/*Set timeout*/
		tv.tv_sec = SELECT_TIMEOUT;
		tv.tv_usec = 0;
		if (select(s + 1, &read_fds, NULL, NULL, &tv) == -1) {
			perror("Server-select()");
			exit(1);
		}

		if (FD_ISSET(s, &read_fds)) {
			/* handle data from a client */
			if ((nbytes = recv(s, buf, MSG_SIZE, 0)) <= 0) {
				/* got error or connection closed by client */
				if (nbytes == 0) {
					/* connection closed */
					printf("socket %d hung up\n", s);
					of_nox_info.run = 0;
				} else {
					perror("recv()");
				}
				/* close it... */
				close(s);
				/* remove from master set */
				FD_CLR(s, &master);
			} else {
				/* we got some data from a client*/
				struct msg_fns *msg = (struct msg_fns*) buf;
				switch (msg->type) {
				case FNS_MSG_SW_IDS:
					process_ids_msg(buf);
					break;
				default:
					printf("Invalid message: %s\n", (char*) buf);
					break;
				}
			}

		}
	}
	printf("closing socket\n");
	if(buf)
		free(buf);
	/*Close all sockets*/
	close(s);
	of_nox_info.run = 0;
	return NULL;
}

int send_msg(int sockfd, void* msg, int size) {
	int n;
	//printf("Sending msg to %d\n", sockfd);
	n = write(sockfd, msg, size);
	if (n < 0)
		perror("ERROR writing to socket");
	return n;
}
void process_ids_msg(void *buf) {
	struct msg_ids *msg = (struct msg_ids *) buf;
	int numEp = msg->nEp;
	int i = 0;
	printf("Num of switches in the controller: %d\n", numEp);
	for (i = 0; i < numEp; i++) {
		printf("%lu: ports: %d\n", msg->endpoints[i].uuid, msg->endpoints[i].port);
	}

}

int close_con(int fd) {
	close(fd);
	return 0;
}

int of_nox_connect(char* addr, int port) {

	if (of_nox_info.run)
		return -1;
	//printf("Connection OF-NOX driver to: %s:%d\n", addr, port);
	of_nox_info.run = 1;
	return start_server(addr, port);
}

int of_nox_stop(void) {
	of_nox_info.run = 0;
	close(of_nox_info.sockfd);
	return 0;
}

int of_nox_send_msg(fnsDesc *desc, int type) {
//	printFNS(desc);
	int size= sizeof(struct msg_fns) + GET_VN_SIZE(desc);
	void *buf = malloc(size);
	struct msg_fns* msg= (struct msg_fns*) buf;
	memset(buf,0,size);
	msg->type = type;
	msg->size = size;
	memcpy(&msg->fns, desc, GET_FNS_SIZE(desc->nEp, desc->nCons));
	if(send_msg(of_nox_info.sockfd, (void *) buf, size)){
		/* Wait for answer*/
		if(recv(of_nox_info.sockfd, buf, size, 0) == 0 )
			printf("confirmation not received ");
	}
	if(buf)free(buf);
	return 0;
}

int of_nox_instantiate_fns(fnsDesc *desc) {
//	printf("Processing request ...\n");
	if(of_nox_info.sockfd)
		return of_nox_send_msg(desc, FNS_MSG_ADD);
	else
		return -1;
}

int of_nox_remove_fns(fnsDesc *desc) {
//	printf("Processing remove ...\n");
	return of_nox_send_msg(desc, FNS_MSG_DEL);
}

int of_nox_modify_fns_add(fnsDesc *desc) {
//	printf("Processing modify ...\n");
	return of_nox_send_msg(desc, FNS_MSG_MOD_ADD);
}

int of_nox_modify_fns_del(fnsDesc *desc) {
//	printf("Processing modify ...\n");
	return of_nox_send_msg(desc, FNS_MSG_MOD_DEL);
}

int of_nox_request_ids(void) {
	//printf("Sending request ids ...\n");
	struct msg_fns msg;
	msg.type = FNS_MSG_SW_IDS;
	msg.size = 0;
	send_msg(of_nox_info.sockfd, (void *) &msg, sizeof(msg));
	return 0;
}

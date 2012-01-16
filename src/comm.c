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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "comm.h"
#include "fns-msg.h"

void process_link_msg(void *buf);

void error(const char *msg) {
	perror(msg);
	exit(0);
}

int start_server(char *address, int portno) {
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int server_sock_fd = 0;
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock_fd < 0)
		error("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		error("ERROR on binding");
		close(sock_fd);
		return -1;
	}
	listen(sock_fd, MAX_CONNECTIONS);
	clilen = sizeof(cli_addr);
	//while(1){
	/*Wait incomming connections. Only 1 accepted*/
	server_sock_fd = accept(sock_fd, (struct sockaddr *) &cli_addr, &clilen);
	if (server_sock_fd < 0) {
		error("ERROR on accept");
		return 0;
	}

	return server_sock_fd;
}

void start_threads(int sockfd) {
	pthread_t pth; // communication thread
	pthread_create(&pth, NULL, process_events, (void*) &sockfd);
	pthread_join(pth, NULL);

}

void process_link_msg(void *buf) {
	struct msg_link *msg = (struct msg_link*) buf;
	printf("SRC: %lx:%d \tDST: %lx:%d\n", msg->dpsrc, msg->sport, msg->dpdst,
			msg->dport);
}

void* process_events(void *arg) {
	printf("Events thread\n");
	int s = *((int*) arg);
	void* buf = (void*) malloc(MSG_SIZE);
	while (1) {
		int ret = recv(s, (void*) buf, MSG_SIZE, 0);
		if (ret < 0) {
			printf("Error receiving msg\n");
		} else if (ret == 0) {
			printf("connection lossed");
			break;
		} else {
			printf("Msg received from the server\n");
			struct msg_hdr *msg = (struct msg_hdr*) buf;
			switch (msg->type) {
			case FNS_MSG_LINK:
				process_link_msg(buf);
				break;
			default:
				printf("Invalid message\n");
				break;
			}
		}
	}
	free(buf);
	close(s);
	return NULL;
}

int send_msg(int sockfd, char* msg, int size) {
	int n;
	n = write(sockfd, msg, size);
	if (n < 0)
		error("ERROR writing to socket");
}

int close_con(int sockfd) {
	close(sockfd);
	return 0;
}

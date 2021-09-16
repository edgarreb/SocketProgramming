#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>

int printAddressInfo(struct addrinfo* Address) {
	
	printf("|Socket Type: ");
	if(Address->ai_socktype == SOCK_STREAM) {
		printf("Stream socket\n");
		printf("|Protocol: Transmission Control Protocol (TCP)\n");
	}
	else if(Address->ai_socktype == SOCK_DGRAM) {
		printf("Datagram socket\n");
		printf("|Protocol: User Datagram Protocol (UDP)\n");
	}

	char ipv4_address[INET_ADDRSTRLEN];
	char ipv6_address[INET6_ADDRSTRLEN];

	printf("|IP Address: ");
	if(Address->ai_family == AF_INET) {
		inet_ntop(AF_INET, &((struct sockaddr_in*)Address->ai_addr)->sin_addr, ipv4_address, INET_ADDRSTRLEN);
		printf("%s\n", ipv4_address);		
	}

	printf("|Port Number: ");
	unsigned short int port_number = ntohs(((struct sockaddr_in*)Address->ai_addr)->sin_port);
	printf("%i\n", port_number);
		
	printf("\n");

	return 1;
}

int main() {

	struct addrinfo hints, *res;
	
	// Initialize hints.
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int status;
	
	if((status = getaddrinfo(NULL, "13000", &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));	
		return 1;
	}

	// Create a socket.
	int socket_fd;
	
	if((socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) { 
		perror("socket");
		return 1;
	}

	// Bind the socket to an IP Address and Port number.
	if((bind(socket_fd, res->ai_addr, res->ai_addrlen)) == -1) {
		close(socket_fd);
		perror("bind");
		return 1;
	}

	// Print Server Information.
	printf("\nServer Info\n");
	printAddressInfo(res);

	// Free linked list.
	freeaddrinfo(res);

	// Listen on socket_fd for an incoming connection.
	if((listen(socket_fd, 10)) == -1) {
		perror("listen");
		return 1;
	}

	
	// Accept any of the incoming connections.
	int new_socket_fd;
		
	struct sockaddr_storage their_addr;
	socklen_t sin_size;

	sin_size = sizeof their_addr;

	printf("Server Waiting for connection...\n\n");
		
	if((new_socket_fd = accept(socket_fd, (struct sockaddr*)&their_addr, &sin_size)) == -1) {
		perror("accpet");
		printf("ERROR: Unable to accept new connection\n");
		return 1;
	}

	// Print remote host information.
	//printf("\nRemote Host\n");
	//printAddressInfo((struct addrinfo*)&their_addr);
	
	char buffer[1024];
	int recv_status;
	int send_status;

	printf("Connection established\n");
	
	while(1) {
		
		memset(buffer, 0, sizeof buffer);

		recv_status = recv(new_socket_fd, (void*)&buffer, sizeof buffer, 0);

		if(recv_status == -1) {
			perror("recv");
		}
		else if (recv_status == 0) {
			printf("Connection closed by client.\n");
			close(new_socket_fd);
			break;
		}
		else {
			printf("%s\n", buffer);
		}
		
		send_status = send(new_socket_fd, (void*)buffer, sizeof buffer, 0);
		
		if(send_status == -1) {
                	perror("send");
		}
		
	}

	close(new_socket_fd);
	close(socket_fd);

	return 0;
}

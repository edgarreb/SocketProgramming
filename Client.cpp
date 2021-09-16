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

	// Setup client addrinfo struct.
	struct addrinfo hints, *res;
	
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	if(getaddrinfo(NULL, "13001", &hints, &res) != 0) {
		printf("ERROR: getaddrinfo()\n");
		return 0;
	}

	// Create client socket.	
	int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
	if(socket_fd == -1) {
		printf("ERROR: socket()");
		return 0;
	}

	// Bind the client socket.
	if(bind(socket_fd, res->ai_addr, res->ai_addrlen) == -1) {
		printf("ERROR: bind()");
	}

	// Print client information.
	printf("\nClient Info\n");
	printAddressInfo(res);

	// Connect to remote server.
	
	struct addrinfo server_hints, *server_info;
	
	server_hints.ai_flags = AI_PASSIVE;
	server_hints.ai_family = AF_INET;
	server_hints.ai_socktype = SOCK_STREAM;
	
	
	if (getaddrinfo(NULL, "13000", &server_hints, &server_info) != 0) {
		printf("ERROR: getaddrinfo() server side/n");
		return 0;
	}

	if(connect(socket_fd, server_info->ai_addr, server_info->ai_addrlen) == -1) {
		printf("ERROR: connect()\n");
	}
	
	// Free addrinfo linked lists. 
	freeaddrinfo(res);
	freeaddrinfo(server_info);

	// Send server packets.
	char send_buffer[1024];
	char recv_buffer[1024];

	while(1) {
		memset(&send_buffer, 0, sizeof send_buffer);
		memset(&recv_buffer, 0, sizeof recv_buffer);
		
		printf("-> ");
		std::cin >> send_buffer;

		if(send(socket_fd, (void*)send_buffer, sizeof send_buffer, 0 ) == -1 ) {
			printf("ERROR: send()");
			return 0;
		}

		printf("Server Response: ");	
		
		if(recv(socket_fd, (void*)recv_buffer, sizeof recv_buffer, 0) == -1) {
			printf("ERROR: recv()");
			return 0;
		}
		else {
			printf("%s\n", recv_buffer);
		}

	}

	// close the socket. 
	close(socket_fd);

	return 1;
}

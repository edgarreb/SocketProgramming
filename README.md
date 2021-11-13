#### Server General Setup

In order to setup a server using stream sockets the following data structres and funtion calls need to be made.

1. Create a struct addrinfo and fill it out with general information such as IP version (ai_family), socket type (ai_socktype), and (ai_flags) if applicable. This addrinfo data structure is used to setup the Server sockaddr data structure.

2. Call getaddrinfo() with the IP address, port/service, addrinfo struct created in step 1, and a addrinfo** that will hold the result of the call to getaddrinfo(). get addrinfo() will return a linked list of addrinfo data structures that are filled out for us with the IP address and port number we specified. Whenever the linked list is not needed anymore make sure to free it by calling freeaddrinfo().

3. Create a socket by calling the socket() function. 

4. Bind the socket created in step 4 to an IP and Port number by calling the bind() function. Use the linked list returned from the call to getaddrinfo in step 2 as arguments to the bind() function.

5. Listen for an incoming connection request on the socket created in step 3 by calling the listen() function.

6. Accpet the incoming connection request on a **NEW** socket. You have to create a **NEW** socket to accept the connection, the original socket from step 3 (if continued to use) will listen for more incomming connection requests. A new socket has to be created for handling new connections (just like this one).

7. Send and recieve data on the socket created in step 6 by using the send() and recv() functions.

8. Close the socket when no longer needed by calling the close() function.

---

#### Client General Setup

In order to setup a client using stream sockets the following data structures and function calls need to be made. 

1. Same as step 1 for Server setup.

2. Same as step 2 for Server Setup.

3. Create a socket by calling the bind() function.

4. If you want to bind to a certian IP address and Port number then call the bind() function. If the bind() function isn't called I believe the Operating System assigns a random Port and its local IP address to the socket.

5. Connect to the server by calling the connect() function. 

6. Send and recieve data by calling the send() and recv() functions.

8. Close the socket by calling the close() function.

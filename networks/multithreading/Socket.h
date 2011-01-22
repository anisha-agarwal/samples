/**
 * @class : Socket.h
 * @desc  : This implements the socket class
 */

#ifndef _Socket_h_
#define _Socket_h__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

class Socket
{
 public:
  int sockfd,c_id;

      /**
       * @brief : Constructor which creates a new socket identifier generated by accept
       * @param[in] : socket ID
       * @param[out] : n/a
       */
      Socket(int sock_id)
	{
	  sockfd = sock_id;
	}

      /**
       * @brief : default constructor
       */
      Socket()
	{
	  if ((sockfd = socket(AF_INET, SOCK_STREAM,0)) == -1)
	    {
	      perror("socket");
	      exit(1);
	    }
	}

      /**
       * @brief : desctructor
       */
      ~Socket() {}
      
      /**
       * @brief : Bind the socket with port of local machine
       */
      int Bind(int my_PORT, char* rec)
      {
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;             // host byte order
	my_addr.sin_port = my_PORT;               // short, network byte order
	my_addr.sin_addr.s_addr = INADDR_ANY;     // automatically fill with my IP

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1)
	  {
	    perror("bind");
	    exit(1);
	  }
      }

      /**
       * @brief : listen to incoming connections
       */
      int Listen()
      {
	int backlog = 20;
	if(listen(sockfd,backlog) == -1)
	  {
	    perror("listen");
	    exit(1);
	  }
      }

      /**
       * @brief      : Accepts a new connection
       * @param[out] : Returns the new socket if successful, else NULL
       */
      Socket* Accept(char* dest)
      {
	int new_fd;
	socklen_t sin_size;
	struct hostent *dest_addr;
	struct sockaddr_in their_addr;

	if((dest_addr = gethostbyname(dest)) == NULL)
	  {
	    perror("gethostbyname");
	    exit(1);
	  }

	their_addr.sin_addr = *((struct in_addr *)dest_addr->h_addr);

	sin_size = sizeof their_addr;
	if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1)
	  {
	    perror("accept");
	  }
	Socket* new_sock = new Socket(new_fd);
	return new_sock;
      }

      /**
       * @brief : close the socket
       */
      int Close()
      {
	if((c_id = close(sockfd)) == -1)
	  {
	    perror("close");
	  }
      }

      /**
       * @brief : used by client to connect to a remote server
       */
      int Connect(int dest_PORT, char* dest)
      {
	struct hostent *dest_addr;
	struct sockaddr_in their_addr;

	if((dest_addr = gethostbyname(dest)) == NULL)
	  {
	    perror("gethostbyname");
	    exit(1);
	  }

	their_addr.sin_family = AF_INET;         // host byte order
	their_addr.sin_port = dest_PORT;         // short,network byte order
	their_addr.sin_addr = *((struct in_addr *)dest_addr->h_addr); 

	if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof their_addr) == -1)
	  {
	    perror("connect");
	    exit(1);
	  }
      }

      /**
       * @brief : Read len bytes into buffer, returns actual bytes read
       */
      int Read(char* buffer, int len)
      {
	int numbytes;
	if((numbytes = recv(sockfd,buffer,len,0)) == -1)
	  {
	    perror("recv");
	    exit(1);
	  }
	buffer[numbytes] = '\0';
	return numbytes;
      }

      /**
       * @brief : send len bytes from buffer. returns actual bytes sent
       */
      int Send(const char* buffer, int len)
      {
	if (send(sockfd, buffer, len, 0) == -1)
	  {
	    perror("send");
	  }
	return 0;
      }

      /**
       * @brief : Sends an integer to the client when server receives a get call
       */

};

#endif
 
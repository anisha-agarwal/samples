#ifndef _socket_h_
#define _socket_h_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "socket.h"
#include <arpa/inet.h>

/* 
 * class for a socket
 * TCP or UDP
 * implement read/write/connect/accept functions
 */

class Socket
{
 public:
  enum type_t {TCP, UDP};
  int sockfd,c_id;
  
  // Constructor which creates accepts a new socket identifier generated by accept
  Socket(int sock_id) 
    {
      sockfd = sock_id;
    }
  
  
  // Constructor - specifies the different type of sockets
  Socket(type_t t = TCP)  
    {
      m_type = t;
      if(m_type == TCP)
	{
	  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	    {
	      perror("socket");
	      exit(1);
	    }
	}
      
      if(m_type == UDP)
	{
	  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	    {
	      perror("socket");
	      exit(1);
	    }
	}
      int on = 1;
      if(setsockopt(sockfd,
		    SOL_SOCKET,
		    SO_REUSEADDR,
		    &on,
		    sizeof(int)))
	printf("Error setting SO_REUSEADDR\n");
      
    }
  
  
  
  
  ~Socket() {} //Destructor
  
  
  
  
  // try to read len bytes into buffer. returns actual bytes read
  int Read(char* buffer, int len)
    {
      int numbytes;
      if((numbytes = recv(sockfd,buffer,len,0)) == -1)  // Receive System Call
	{
	  perror("recv");
	  exit(1);
	}
      buffer[numbytes] = '\0';
      return numbytes;
    }
  
  
  
  
  
  // send len bytes from buffer. returns actual btes sent
  int Send(const char* buffer, int len) 
    {
      if (send(sockfd, buffer, len, 0) == -1)
	perror("send");
      return 0; 
    }
  




  // Sending for UDP socket
  int Sendto(const char* buffer, int len, int dest_PORT, char* dest)
    {
      struct hostent *dest_addr;
      struct sockaddr_in their_addr;
      
      if((dest_addr = gethostbyname(dest))==NULL)
	{
	  perror("gethostbyname");
	  exit(1);
	}
      
      their_addr.sin_family = AF_INET;    // host byte order
      their_addr.sin_port = dest_PORT;  // short, network byte order
      their_addr.sin_addr = *((struct in_addr *)dest_addr->h_addr);
      
      if(sendto(sockfd, buffer, len, 0,(struct sockaddr *)&their_addr, sizeof their_addr) == -1)
	perror("sendto");
      return 0;
    } 
  



  // Receiving for UDP socket
  int Recvfrom(char* buffer, int len)
    {
      int numbytes;
      struct sockaddr_in their_addr;
      int sz = sizeof(their_addr);
      if((numbytes = recvfrom(sockfd, buffer, len, 0, (struct sockaddr *)&their_addr, (socklen_t*)&sz)) == -1)
	{ 
	  perror("recvfrom");
	  exit(1);
	}
      buffer[numbytes] = '\0';
      return numbytes;
    }
  



  // connect to a remote connection
  int Connect(int dest_PORT, char* dest)
    {
      struct hostent *dest_addr;
      struct sockaddr_in their_addr;
      
      if((dest_addr = gethostbyname(dest))==NULL)
	{
	  perror("gethostbyname");
	  exit(1);
	}
      
      their_addr.sin_family = AF_INET;    // host byte order
      their_addr.sin_port = dest_PORT;  // short, network byte order
      their_addr.sin_addr = *((struct in_addr *)dest_addr->h_addr);
      
      if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof their_addr) == -1)   // Connect System Call 
	{
	  perror("connect");
	  exit(1);
	}
    }
  



  // To associate the socket with a port on the local machine
  int Bind(int my_PORT, char* rec)
    {
      struct sockaddr_in my_addr; 
      my_addr.sin_family = AF_INET;            // host byte order
      my_addr.sin_port = my_PORT;        // short, network byte order
      my_addr.sin_addr.s_addr = INADDR_ANY;    // automatically fill with my IP

      if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) 
	{
	  perror("bind");
	  exit(1);
	}
    }




  // Listen to incoming connections
  int Listen()
    {
      int backlog = 20;
      if(listen(sockfd,backlog) == -1)
	{
	  perror("listen");
	  exit(1);
	}
    } 



  // Close the socket
  int Close()
    {
      if((c_id = close(sockfd)) == -1)
	{
	  perror("close");
	}
    }
   




  // accept a new connection.
  // returns the new socket if successful, else NULL
  Socket* Accept(char* dest)
    {
      int new_fd;
      socklen_t sin_size;
      struct hostent *dest_addr;
      struct sockaddr_in their_addr;
      
      if((dest_addr = gethostbyname(dest))==NULL)
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


  void Type(type_t t) { m_type = t; }
  type_t Type() { return m_type; }



  // To return the port number
  int Sock_Name()
    {
      int i;
      struct sockaddr_in addr;
      socklen_t addr_len;
      addr_len = sizeof addr;
      if(getsockname(sockfd,(struct sockaddr*)&addr,&addr_len) == -1)
	{
	  perror("getsockname()");
	  return -1;
	}
      i = (int)ntohs(addr.sin_port);
      return i;
    }



  // To print the IP address
  char* ip()
    {
      char hostname[128];

      gethostname(hostname, sizeof hostname);
      //printf("My hostname: %s\n", hostname);
      struct hostent * he = gethostbyname(hostname);
      //printf("IP address: %s\n", inet_ntoa(*(struct in_addr*)he->h_addr));
      return inet_ntoa(*(struct in_addr*)he->h_addr);
    }


 private:
  type_t m_type; // type of socket  
};

#endif


	}
      i = (int)ntohs(addr.sin_port);
      return i;
    }



  // To print the IP address
  char* ip()
    {
      char hostname[128];

      gethostname(hostname, sizeof hostname);
      //printf("My hostname: %s\n", hostname);
     
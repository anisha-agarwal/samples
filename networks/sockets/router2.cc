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
#include "messenger.h"

void handleConnection(Socket* s, Socket r1)
{
  char buf[1024];
  int pack_no = 1;
  int no1 = 0;
  int no2 = 0;

  while(1)
    {
      int i;
      char data[1024];
      char* payload;
      int read = s->Read(buf,1024);    // Read the incoming message into a character array returning the number of bytes read           
      if (read<=0)                     // finished reading
	break;
      buf[read] = '\0';                // Setting the first character after the received message in the array to \0
      Packet* ack = new Packet("Router2",1,1,1,"ack");
      char ack_str[512];
      ack->to_string(ack_str, 512);
      s->Send(ack_str, strlen(ack_str));
      Packet* p = new Packet(buf);     // Creating a packet out of the incoming string of message
      payload = p->Word(); 
      i = p->Sent_No();                // Determining the sentence number of the incoming message
      printf("Received Packet %d containing %s from Messenger over TCP\n",pack_no,payload);
      p->SetIdString("Router2");       // Changing the string identifier to Router1
      p->to_string(data,1024);         // Changing the packet back to string to send over the network
      if(i == 1)
	{
	  r1.Sendto(data,strlen(data),3219,"nunki.usc.edu");           // Sending to receiver 1
          no1 = no1+1;
	}
      else if(i == 2)
	{
	  r1.Sendto(data,strlen(data),3319,"nunki.usc.edu");           // Sending to receiver 2
	  no2 = no2+1;
	}
      printf("Forwarded packet %d containing %s to Receiver %d over UDP \n",pack_no,payload,i);
      pack_no = pack_no+3;
    }
  printf("Completed transfer of %d packets to Receiver 1 \n",no1);
  printf("Completed transfer of %d packets to Receiver 2 \n",no2);
  printf("Completed Phase 2 for Router2\n");
}


int main()
{
  int i;

  Socket p(Socket::TCP);                         // Creating the TCP socket
  Socket* s = 0;                                 // The child socket returned after accept()
  Socket r1(Socket::UDP);                        // The dynamic UDP socket to send packets forward
  r1.Bind(0,"nunki.usc.edu");
  i = r1.Sock_Name();
  printf("Router2 has TCP port 21319 and UDP port %d\n",i);
  char* IP_Add;
  p.Bind(21319,"nunki.usc.edu");                 // Binding the TCP socket to the static port 21219
  IP_Add = p.ip();
  printf("Router2 has IP Address %s\n",IP_Add);
  p.Listen();                                    // Listening for incoming requests
  s = p.Accept("nunki.usc.edu");                 // Accepting the incoming connection 
  printf("Router2 accepted connection request from Messenger\n");
  handleConnection(s,r1);            
  s->Close();                                    // Closing the TCP port
  r1.Close();                                    // Closing the UDP port
  p.Close();
}


 21219
  IP_Add = p.ip();
  printf("Router2 has IP Address %s\n",IP_A
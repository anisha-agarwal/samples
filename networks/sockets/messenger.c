#include <stdio.h>
#include <stdlib.h>
#include "messenger.h"
#include <string>
#include <vector>
#include "socket.h"

using namespace std;

int main()
{
  FILE* fp;
  char inp[1024];
  int i =0;
  int res;
  int num,n;
  int sock_num[3];
  char num1;
  char delims[] = " ";
  char *data = NULL;
  int sent_no = 0;
  char* buf = new char[1024];
  vector<Packet*> packs;             // A vector to store the packets being created
  Socket s[3];                       // Creating the 3 dynamic TCP sockets
  Socket r(Socket::UDP);             // Creating the static UDP port
  r.Bind(4219,"nunki.usc.edu");      // Binding to the port
  vector<string> sents;              // A vector to store the sentences being received
  sents.resize(2);
  int count[3] = {0};
  struct hostent *dest;
  struct in_addr addr;

  s[0].Bind(0,"nunki.usc.edu");
  sock_num[0] = s[0].Sock_Name();

  s[1].Bind(0,"nunki.usc.edu");
  sock_num[1] = s[1].Sock_Name();

  s[2].Bind(0,"nunki.usc.edu");
  sock_num[2] = s[2].Sock_Name();

  printf("Messenger has TCP port numbers %d %d %d \n",sock_num[0],sock_num[1],sock_num[2]);
  printf("Messenger has UDP port number 4219 \n");

  char* IP_Add;                      // To retrieve the IP address of the server
  IP_Add = r.ip();
  printf("Messenger has IP Address %s \n",IP_Add);

  s[0].Connect(21219,"nunki.usc.edu");   // Connecting to router 1
  printf("Messenger is now connected to Router 1 \n");
  
  s[1].Connect(21319,"nunki.usc.edu");   // Connecting to router 2
  printf("Messenger is now connected to Router 2 \n");

  s[2].Connect(21419,"nunki.usc.edu");   // Connecting to router 3
  printf("Messenger is now connected to Router 3 \n");
   
  fp = fopen("msg.txt","r");
  
  if(fp == NULL)
    {
      fprintf(stderr,"No message to transfer");
      exit(1);
    }
  
  for(i=0;i<=1;i++)
  {
    fgets(inp,1024,fp);   // Reading the input file one line at a time
    
    if (inp[strlen(inp)-1] == '\n')   // Replacing the last character in the first line by a NULL
      inp[strlen(inp)-1] = '\0';


    sent_no++;
    num = atoi(inp);
      
    data = strtok(inp,delims);                  // Dividing the line into words, separated by spaces
    int ctr = 0;
    while(data != NULL)
      {
	if(ctr!=0)
	  {
	    Packet* p = new Packet("messenger",sent_no,ctr,num,data);      // Creating packets
	    if (ctr != 1)
		sents[i] = sents[i] + " ";
	    sents[i] = sents[i] + data;
	    packs.push_back(p);                          // Inserting the packets into the vector
	  }          
	data = strtok(NULL,delims);
	ctr = ctr+1;
      }
  }
    
  for(i=0; i < packs.size(); i++)
    { 
      char data[1024];
      char* payload;
      char ack[1024];
      packs[i]->to_string(data,1024);                   // Converting the packet to a string to be sent over the network
      n = i%3;
      s[n].Send(data,strlen(data));
      payload = packs[i]->Word();
      printf("Sent packet %d containing %s to Router %d over TCP \n",i+1,payload,n+1);
      count[n] = count[n]+1;
      int read = s[n].Read(ack,1024);
      Packet ack_p(ack);
      printf("Received Ack of Packet %d from %s over TCP \n",i+1,ack_p.id());
    }

  for(n=0;n<3;n++)
    {
      printf("Completed transfer of %d packets to Router %d \n", count[n],n+1);
    }
    

  printf("End of Phase 1 for Messenger \n\n");
    
  
  Packet* recv_sents[2];     // Packets to store the sentences being received
  for(i=0;i<2;i++)
    {
      r.Recvfrom(buf,1024);
      recv_sents[i] = new Packet(buf);   
      printf("Received The Sentence %s from Receiver %d over UDP \n",recv_sents[i]->Word(),i+1);
    }
  
  for(i=0;i<2;i++)
    {
      res = strcmp(recv_sents[i]->Word(), sents[i].c_str());    // Comparing the string sent and the one received
      if(res == 0)
	printf("Sentence %d is a match \n",i+1);
    }
 
  printf("\n End of Phase 3 for Messenger\n");
  
  
  s[0].Close();
  s[1].Close();
  s[2].Close();
}
   {
      r.Recvfrom(buf,1024)
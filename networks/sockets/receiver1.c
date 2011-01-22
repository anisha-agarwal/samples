#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>
#include "socket.h"
#include "messenger.h"

using namespace std;

int main()
{
  char* buf = new char[1024];
  char data[1024];
  Socket s(Socket::UDP);                               // Creating the static UDP socket
  Socket r1(Socket::UDP);                              // Creating the dynamic UDP socket
  s.Bind(3219,"nunki.usc.edu");                        //Binding the static UDP socket
  r1.Bind(0,"nunki.usc.edu");
  int sock_no;
  sock_no = r1.Sock_Name();
  printf("Receiver1 has UDP ports - static 3219 and dynamic %d \n",sock_no);
  char* IP_Add;
  IP_Add = s.ip();
  printf("Receiver1 has IP Address %s \n",IP_Add);
  int i,n,m;
  vector<char*> words;
  char* word;
  string sent;
  char* id_string;
  //printf("Read %d \n", s.Recvfrom(buf,1024));
  s.Recvfrom(buf,1024);


  Packet* p = new Packet(buf);
  i =  p->No_Words();
  words.resize(i);
  m = p->word_no();
  //printf("Word no is %d",m);
  word = p->Word();
  words[m-1] = strdup(word);
  id_string = p->id();
  printf("Received Packet 1 containing %s from %s over UDP\n",word,id_string); 
  for(n=1;n<=i-1;n++)
  {
    s.Recvfrom(buf,1024);
  
    Packet* p = new Packet(buf);
    m = p->word_no();
    word = p->Word();
    words[m-1] = strdup(word);
    id_string = p->id();
    printf("Received Packet %d containing %s from %s over UDP\n",n+1,word,id_string);
  }
  printf("End of Phase 2 for Receiver1 \n");
 
  for(m=0;m<i;m++)
  {
    sent = sent + words[m];
    if(m!=i-1)
      sent = sent + " ";
  }
  
  //cout<<"\n"<<sent;
  
  Packet P_S("Receiver1",1,1,1,sent.c_str());
  //P_S.print_packet();
  P_S.to_string(data,1024);
  r1.Sendto(data,strlen(data),4219,"nunki.usc.edu");
  printf("Forwarded Sentence1 to messenger over UDP\n");
  printf("End of phase 3 for Receiver1 \n");
  r1.Close();
  s.Close();
}
 over UDP\n",n+1,word,id_string);
  }
  printf("End of Phase 2 for Receiver1 \n");
 
  for(m=0;m<i;m++)
  {
    sent = sent + words[m];
    if(m!=i-1)
      sent = sent + " ";
  }
  
  //cout<<"\n"<<sent;
  
  Packet P_S("Receiver1",1,1,1,sent.c_str());
  //P_S.print_packet();
  P_S.to_string(data,1024);
  r1.Sendto(data,strlen(data),4219,"nunki.usc.edu");
  printf("Forwarded Sentence1 to messenger over UDP\n");
  printf("End of phase 3 for
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include "Socket.h"

using namespace std;

pthread_mutex_t lock;

int main(int argc, char* argv[])
{
  while(1)
    {
      int count_val;
      Socket s;
      s.Connect(10092,argv[1]); 
      char buf[1024];
      char* call = new char[1024];
      
      cin>>call;
      
      if(!strncasecmp(call,"get",3))
	{
	  s.Send(call,strlen(call));
	  int read = s.Read(buf,4);
	  buf[read] = '\0';
	  cout<<"Count is "<<buf<<endl;
	}
      else
	{
	  if(!strncasecmp(call,"set",3))
	    {
	      cin>>count_val;
	      sprintf(buf,"%s %d",call, count_val);
	      s.Send(buf,1024);
	    }
	  else 
	    {
	      if (!strncasecmp(call, "stop", 4))
		{
		  s.Send(call, strlen(call));
		}
	      else 
		{
		  if (!strncasecmp(call, "continue", 8))
		    {
		      s.Send(call, strlen(call));
		    }
		  else 
		    {
		      if (!strncasecmp(call, "quit", 4))
			break;
		    }
		}
	    }
	}
      s.Close();
    }
}


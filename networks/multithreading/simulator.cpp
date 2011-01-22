#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include "Socket.h"
#include <sstream>

using namespace std;

#define NUM_THREADS 2
int count_num;
pthread_mutex_t lock;
bool stopped;


void *IncrementCount(void *arg)
{
  while(1)
    {
      int rand_integer;
      rand_integer = (rand()%5) + 1;

      if(!stopped)
	{
	  pthread_mutex_lock(&lock);
	  count_num++;
	  pthread_mutex_unlock(&lock);

	  cout<<"count : "<<count_num<<endl;
	}

      sleep(rand_integer);
    }
}

void *server(void *arg)
{
 
  Socket p;
  Socket* s = 0;                       // The child socket returned after accept
  p.Bind(10092,"localhost");
  p.Listen();
  while(1)
    {
      s = p.Accept("sol.scudc.scu.edu");
      char buf[1024];
      int read = s->Read(buf,1024);
      if(0 == read)
	{
	  sleep(2);
	  continue;
	}
      buf[read] = '\0';
      if(!strncasecmp(buf,"get",3))
	{
	  sprintf(buf,"%d",count_num);
	  pthread_mutex_lock(&lock);
	  s->Send(buf,4);
	  pthread_mutex_unlock(&lock);
	}
      else
	{
	  if(!strncasecmp(buf,"set",3))
	    {
	      int set_count;
	      char call[1024];
	      sscanf(buf, "%s %d", call, &set_count);
	      pthread_mutex_lock(&lock);
	      count_num = set_count;
	      pthread_mutex_unlock(&lock);
	    }
	  else
	    {
	      if(!strncasecmp(buf, "stop", 4))
		{
		  stopped = true;
		}
	      else
		{
		  if(!strncasecmp(buf, "continue",8))
		    {
		      stopped = false;
		    }
		}
	    }
	}
      s->Close();
      delete s;
    }
}

int main(int argc, char* argv[])
{

  pthread_t threads[NUM_THREADS];
  count_num = 0;
  void *exit_status;
  pthread_create(&threads[0],NULL,IncrementCount, (void *)0);
  pthread_create(&threads[1],NULL,server,(void *)0);

  for(int i=0;i<2;i++)
    {
      pthread_join(threads[i], NULL);
    }
  
  

}

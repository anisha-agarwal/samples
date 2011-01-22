#ifndef _messenger_h
#define _messenger_h
#include <string>

/*
 *  Defines a packet
 */

class Packet
{
 public:    
  Packet(char* id_string, int sent_no, int frag_no, int total, const char* payload)
    : m_sent_no(sent_no), m_frag_no(frag_no), m_total(total)
    {
      m_id_string = strdup(id_string);
      m_payload = strdup(payload);
    }
  
  void print_packet()
    {
      printf("identifier string = %s \n", m_id_string);
      printf("sentence number = %d", m_sent_no);
      printf("fragment number = %d", m_frag_no);
      printf("total words = %d", m_total);
      printf("payload is = %s", m_payload);
    }
  
  void to_string(char* buffer,int len)
    {
      snprintf(buffer,len,"%d,%d,%d,%s,%s",m_sent_no, m_frag_no, m_total, m_payload, m_id_string);
    }
  
  Packet(char* inbuffer)
    {
      char delims[] = ",";
      char buffer[1024];
      strcpy(buffer,inbuffer);
      m_sent_no = atoi(strtok(buffer,delims));
      m_frag_no = atoi(strtok(NULL,delims));
      m_total = atoi(strtok(NULL,delims));
      m_payload = strdup(strtok(NULL,delims));
      m_id_string = strdup(strtok(NULL,delims));
    }
  
  ~Packet() 
    {
      free(m_id_string);
      free(m_payload);
    }
  
  
  
  // Convert the packet into a string buffer
  const char* Buffer()
    {
      return "This is a packet";
    }
  
  
  // To obtain the sentence number
  int Sent_No()
    {
      return m_sent_no;
    }
  
  
  // To obtain the total number of words in the sentence
  int No_Words()
    {
      return m_total;
    }


  // To obtain the payload
  char* Word()
    {
      return m_payload;
    }


  // To obtain the word number
  int word_no()
    {
      return m_frag_no;
    }


  // To obtain identifier string
  char* id()
    {
      return m_id_string;
    }



  // To change identifier string
  void SetIdString(char* str)
    {
      m_id_string = strdup(str);
    }



  private :
    char* m_id_string;
  int m_sent_no;
  int m_frag_no;
  int m_total;
  char* m_payload;

  // The identifier String
  const char* m_identifier;

  // Sentence Number
  int m_sentence_number;

};

#endif
er
  int word_no()
    {
      return m_frag_no;
    }


  // To obtain identifier string
  char* id()
    {
      return m_id_string;
    }



  // To change identifier string
  void SetIdString(char* str)
    {
      m_id_string = strdup(str);
    }



  private :
    char* m_id_string;
  in
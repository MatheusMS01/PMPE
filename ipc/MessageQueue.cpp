#include "MessageQueue.h"

#include <iostream>

MessageQueue::MessageQueue(int key)
   : m_key(key)
   , m_id(0)
   , m_buffer()
{
   // Open Message Queue
   m_id = msgget(m_key, IPC_CREAT | 0777);
}

MessageQueue::~MessageQueue()
{
}

/**
   Writes string in MessageQueue

   @param message to be written
   @return true if succeeds, false otherwise
*/
bool MessageQueue::write(const std::string& message)
{
   if(m_id > -1)
   {
      sprintf(m_buffer.data, "%.255s", message.c_str());
      return (msgsnd(m_id, m_buffer.data, sizeof(m_buffer.data), 0) != -1);
   }
   
   return false;
}

/**
   Reads the first message in MessageQueue

   @param reference of message to be read,
          size to be read
   @return true if succeeds, false otherwise
*/
bool MessageQueue::read(std::string& message)
{
   if(m_id > -1)
   {
      Buffer *buffer = new Buffer();
      if(msgrcv(m_id, buffer, sizeof(buffer->data), 0,  0) != -1)
      {
         message = buffer->data;
         return true;
      }
   }

   return false;
}

int MessageQueue::getId()
{
   return m_id;
}
#include "MessageQueue.h"

#include <iostream>

MessageQueue::MessageQueue(int key)
   : m_key(key)
   , m_id(0)
{
   // Open Message Queue
   m_id = msgget(m_key, IPC_CREAT | 0777);
}

MessageQueue::~MessageQueue()
{
}

/**
   Writes string in MessageQueue, for a specific type

   @param message to be written,
          type to be read
   @return true if succeeds, false otherwise
*/
bool MessageQueue::write(const std::string& message, const unsigned int type)
{
   if(m_id > -1)
   {
      Buffer *buffer = new Buffer();
      buffer->type = type;
      sprintf(buffer->data, "%.255s", message.c_str());
      return (msgsnd(m_id, buffer, sizeof(buffer->data), 0) != -1);
   }
   
   return false;
}

/**
   Reads the first message type in MessageQueue

   If no type is specified, then read first message
   in Queue by default
   @param reference of message to be read,
          type to be read
   @return true if succeeds, false otherwise
*/
bool MessageQueue::read(std::string& message, const unsigned int type)
{
   if(m_id > -1)
   {
      Buffer *buffer = new Buffer();
      if(msgrcv(m_id, buffer, sizeof(buffer->data), type,  0) != -1)
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
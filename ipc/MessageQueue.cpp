#include "MessageQueue.h"

MessageQueue::MessageQueue(const key_t key)
   : m_key(key)
{
   // Open Message Queue
   m_id = msgget(m_key, IPC_CREAT | 0777);
}

/**
   Writes string in MessageQueue, for a specific type.
   If Queue is full, caller is blocked untill enough 
   space is freed

   @param message to be written,
          type to be written
   @return true if succeeds, false otherwise
*/
bool MessageQueue::write(const std::string& message, const unsigned int type)
{
   if(m_id > 1)
   {
      Buffer *buffer = new Buffer();

      buffer->type = type;
      sprintf(buffer->data, "%.127s", message.c_str());
      const auto result = msgsnd(m_id, buffer, sizeof(buffer->data), 0) != -1;

      delete buffer;
      return result;
   }
   
   return false;
}

/**
   Blocks caller until there is an specified message type to read;

   @param reference of message to be read,
          type to be read
   @return true if succeeds, false otherwise
*/
bool MessageQueue::read(std::string& message, const unsigned int type)
{
   if(m_id > -1)
   {
      Buffer *buffer = new Buffer();

      const auto result = msgrcv(m_id, buffer, sizeof(buffer->data), type,  0) != -1;
      if(result == true)
      {
         message = buffer->data;
      }

      delete buffer;
      return result;
   }

   return false;
}

bool MessageQueue::remove()
{
   return msgctl(m_id, IPC_RMID, NULL) == -1;
}

/**
   @return Maximum number of bytes allowed in queue
*/

msglen_t MessageQueue::getLenght()
{
   msglen_t lenght = 0xFFFFFFFF;
   struct msqid_ds *buffer = new struct msqid_ds();

   if(msgctl(m_id, IPC_STAT, buffer) != 1)
   {
      lenght = buffer->msg_qbytes;
   }

   delete buffer;
   return lenght;
}

/**
   @return Current number of bytes in queue (nonstandard)   
*/
unsigned long MessageQueue::getCurrentBytes()
{
   unsigned long bytes = 0xFFFFFFFFFFFFFFFF;
   struct msqid_ds *buffer = new struct msqid_ds();

   if(msgctl(m_id, IPC_STAT, buffer) != 1)
   {
      bytes = buffer->__msg_cbytes;
   }

   delete buffer;
   return bytes;
}

unsigned int MessageQueue::getBufferSize()
{
   Buffer buffer;
   return sizeof(buffer.data);
}
#include "Node.h"
#include <unistd.h>

#include <iostream>

Node::Node(int id)
   : m_id(id)
   , m_processType(id + MessageQueue::SchedulerId + 1)
   , m_messageQueue(MessageQueue::MainQueueKey)
{
}

Node::~Node()
{
}

void Node::execute()
{
   while(true)
   {
      std::string message;
      if(m_messageQueue.read(message, m_processType))
      {
         std::cout << "Node[" << m_id << "]: " << message << "\n";
      }
   }
}
#include "Node.h"
#include <unistd.h>

#include <iostream>

Node::Node(int id)
   : m_id(id)
   , m_processType(id + MessageQueue::SchedulerId + 1)
   , m_messageQueue(MessageQueue::MainQueueKey)
{
   m_x = id % 4;
   m_y = id / 4;

   buildNeighborhood();
}

Node::~Node()
{
}

void Node::buildNeighborhood()
{
   for(int adjacencyX = -1; adjacencyX <= 1; adjacencyX += 2)
   {
      Neighbor neighbor((m_x + adjacencyX)%4, m_y);
      m_neighborList.push_back(neighbor);
   }

   for(int adjacencyY = -1; adjacencyY <= 1; adjacencyY += 2)
   {
      Neighbor neighbor(m_x, (m_y + adjacencyY)%4);
      m_neighborList.push_back(neighbor);
   }
}

void Node::execute()
{
   while(true)
   {
      std::string message;
      if(m_messageQueue.read(message, m_processType))
      {
         std::cout << "Node[" << m_x << ", " << m_y << "]: " << message << "\n";
      }
   }
}
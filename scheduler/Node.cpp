#include "Node.h"
#include "Utils.h"

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
      if(!m_messageQueue.read(message, m_processType))
      {
         continue;
      }

      switch(Utils::getProtocolId(message))
      {
         case IProtocol::ExecuteProgramPostponed:
         {
            ExecuteProgramPostponedProtocol epp;
            epp.parse(message);
            treat(epp);
         }
         break;

         default:
         {
         }
         break;
      }
   }
}

void Node::treat(ExecuteProgramPostponedProtocol epp)
{
   if(epp.getDestinationNode() == m_id)
   {
      std::cout << "I am node " << m_id << " and I am going to execute " << epp.getProgramName() << "\n"; 
   }
   else
   {
      route(epp.serialize(), epp.getDestinationNode());
   }
}

void Node::route(const std::string& pdu, int destinationNode)
{
   for(const auto& neighbor : m_neighborList)
   {
      if(destinationNode == neighbor.m_id)
      {
         std::cout << "Node " << m_id << " routing to neighbor " << neighbor.m_id << "\n";
         m_messageQueue.write(pdu, neighbor.m_processType);
         return;
      }
   }

   // Destination node is not on neighborhood
   // Find best neighbor to route to
   auto bestNeighbor = std::make_pair(m_neighborList.front(),
      Utils::distanceBetweenNodes(m_neighborList.front().m_id, destinationNode));
   for(const auto& neighbor : m_neighborList)
   {
      auto distance = Utils::distanceBetweenNodes(neighbor.m_id, destinationNode);
      if(distance < bestNeighbor.second)
      {
         bestNeighbor.first = neighbor;
         bestNeighbor.second = distance;
      }
   }

   std::cout << "Node " << m_id << " routing to best neighbor " << bestNeighbor.first.m_id << " - Destination: " << destinationNode << "\n";
   m_messageQueue.write(pdu, bestNeighbor.first.m_processType);
}
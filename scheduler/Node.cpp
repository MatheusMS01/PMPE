#include "Node.h"
#include "Utils.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <ctime>

int g_id;

void SIGCHLDHandler(int signal)
{
   pid_t pid;
   int status;

   while ((pid = waitpid(-1, &status, WNOHANG)) != -1)
   {
      // MessageQueue messageQueue(MessageQueue::MainQueueKey);
      // messageQueue.write(std::to_string(time(NULL)), g_id);
   }
}

Node::Node(int id)
   : m_id(id)
   , m_processType(id + MessageQueue::SchedulerId + 1)
   , m_messageQueue(MessageQueue::MainQueueKey)
   , m_ns()
{
   g_id = id;
   m_x = id % 4;
   m_y = id / 4;

   buildNeighborhood();

   // Set action for SIGCHLD

   struct sigaction sa;

   memset(&sa, 0, sizeof(sa));
   sa.sa_handler = SIGCHLDHandler;

   sigaction(SIGCHLD, &sa, NULL);
}

Node::~Node()
{
}

void Node::buildNeighborhood()
{
   for(int adjacencyX = -1; adjacencyX <= 1; adjacencyX += 2)
   {
      Neighbor neighbor((m_x + adjacencyX) % 4, m_y);
      m_neighborList.push_back(neighbor);
   }

   for(int adjacencyY = -1; adjacencyY <= 1; adjacencyY += 2)
   {
      Neighbor neighbor(m_x, (m_y + adjacencyY) % 4);
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

      if(message.find(";") == std::string::npos)
      {
         // m_ns.setEndTime(std::stol(message));
         // route(m_ns.serialize(), 0);

         continue;
      }

      std::cout << message << "\n";
      
      switch(Utils::getProtocolId(message))
      {
         case IProtocol::ExecuteProgramPostponed:
         {
            ExecuteProgramPostponedProtocol epp;
            epp.parse(message);
            treat(epp);
         }
         break;

         case IProtocol::NotifyScheduler:
         {
            NotifySchedulerProtocol ns;
            ns.parse(message);
            treat(ns);
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
      m_ns.setNodeId(m_id);
      m_ns.setDelay(epp.getDelay());
      m_ns.setProgramName(epp.getProgramName());
      m_ns.setBeginTime(time(NULL));
      
      {
         pid_t pid;

         pid = fork();

         if(pid == -1)
         {
            return;
         }

         if(pid == 0)
         {
            // Run Program
            sleep(1);

            _exit(0);
         }
      }
   }
   else
   {
      route(epp.serialize(), epp.getDestinationNode());
   }
}

void Node::treat(NotifySchedulerProtocol ns)
{
   if(m_id == 0)
   {
      m_messageQueue.write(ns.serialize(), MessageQueue::SchedulerId);
   }
   else
   {
      route(ns.serialize(), 0);
   }
}

// void Node::finish()
// {
//    m_ns.setEndTime(time(NULL));
//    route(m_ns.serialize(), 0);
// }

void Node::route(const std::string& pdu, int destinationNode)
{
   for(const auto& neighbor : m_neighborList)
   {
      if(destinationNode == neighbor.id)
      {
         m_messageQueue.write(pdu, neighbor.processType);
         return;
      }
   }

   // Destination node is not on neighborhood
   // Find best neighbor to route to
   auto bestNeighbor = std::make_pair(m_neighborList.front(),
      Utils::distanceBetweenNodes(m_neighborList.front().id, destinationNode));
   for(const auto& neighbor : m_neighborList)
   {
      auto distance = Utils::distanceBetweenNodes(neighbor.id, destinationNode);
      if(distance < bestNeighbor.second)
      {
         bestNeighbor.first = neighbor;
         bestNeighbor.second = distance;
      }
   }

   m_messageQueue.write(pdu, bestNeighbor.first.processType);
}
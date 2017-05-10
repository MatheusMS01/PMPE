#include "Node.h"
#include "Utils.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#include <cstring>
#include <iostream>
#include <ctime>
#include <algorithm>

int g_type;

namespace node
{
   void SIGCHLDHandler(int signal)
   {
      int status;

      while(waitpid(-1, &status, WNOHANG) != -1)
      {
         MessageQueue messageQueue(MessageQueue::MainQueueKey);

         TimestampProtocol ts;
         ts.setTimestamp(std::time(nullptr));

         messageQueue.write(ts.serialize(), g_type);
      }
   }
}


Node::Node(const int id)
   : m_id(id)
   , m_processType(id + MessageQueue::SchedulerId + 1)
   , m_messageQueue(MessageQueue::MainQueueKey)
   , m_waitingTimestamp(false)
   , m_ns()
   , m_log(std::to_string(id))
{
   g_type = m_processType;

   m_x = id % 4;
   m_y = id / 4;

   buildNeighborhood();

   // Set action for SIGCHLD
   struct sigaction sa_chld;

   memset(&sa_chld, 0, sizeof(sa_chld));
   sa_chld.sa_handler = node::SIGCHLDHandler;

   sigaction(SIGCHLD, &sa_chld, NULL);
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

         case IProtocol::Timestamp:
         {
            TimestampProtocol ts;
            ts.parse(message);
            treat(ts);
         }
         break;

         default:
         {
         }
         break;
      }
   }
}

void Node::treat(const ExecuteProgramPostponedProtocol& epp)
{
   if(epp.getDestinationNode() == m_id)
   {
      m_log.write("Received execution: " + epp.serialize());

      m_ns.setNodeId(m_id);
      m_ns.setDelay(epp.getDelay());
      m_ns.setProgramName(epp.getProgramName());
      m_ns.setBeginTime(std::time(nullptr));
      m_ns.setSubmittalTime(epp.getSubmittalTime());
      m_ns.setPID(getpid());
      
      pid_t pid;

      pid = fork();

      if(pid == -1)
      {
         return;
      }

      if(pid == 0)
      {
         // @TODO: Run Program
         sleep(1);

         _exit(0);
      }

      m_waitingTimestamp = true;
   }
   else
   {
      route(epp.serialize(), epp.getDestinationNode());
   }
}

void Node::treat(const NotifySchedulerProtocol& ns)
{
   if(m_id == 0)
   {
      m_log.write("Sending notification to scheduler: " + ns.serialize());
      m_messageQueue.write(ns.serialize(), MessageQueue::SchedulerId);
   }
   else
   {
      route(ns.serialize(), 0);
   }
}

void Node::treat(const TimestampProtocol& ts)
{
   m_log.write("Child has terminated execution");

   if(m_waitingTimestamp)
   {
      m_waitingTimestamp = false;
      m_ns.setEndTime(ts.getTimestamp());
      route(m_ns.serialize(), 0);
   }
}

void Node::route(const std::string& pdu, const int destinationNode)
{
   const auto neighborItr = find_if(m_neighborList.begin(), m_neighborList.end(), 
      [&] (const Neighbor& itr) 
      {
         return itr.id == destinationNode;
      });

   if(neighborItr != m_neighborList.end())
   {
      m_log.write("Routing to neighbor " + std::to_string(neighborItr->id) + ": " + pdu);

      m_messageQueue.write(pdu, neighborItr->processType);
      return;
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

   m_log.write("Routing to best neighbor " + std::to_string(bestNeighbor.first.id) + ": " + pdu);

   m_messageQueue.write(pdu, bestNeighbor.first.processType);
}
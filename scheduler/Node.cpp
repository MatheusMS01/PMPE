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

#include <errno.h>

int g_type;

namespace node
{
   void SignalHandler(int signal)
   {
      switch(signal)
      {
         case SIGCHLD:
         {
            int status;

            while(waitpid(-1, &status, WNOHANG) != -1)
            {
               MessageQueue messageQueue(MessageQueue::MainQueueKey);

               TimestampProtocol ts;
               ts.setTimestamp(std::time(nullptr));
               ts.setSuccess(status == 0);

               messageQueue.write(ts.serialize(), g_type);
            }
         }
         break;

         default:
         {
         }
         break;
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
   sa_chld.sa_handler = node::SignalHandler;

   sigaction(SIGCHLD, &sa_chld, NULL);
}

void Node::buildNeighborhood()
{
   m_log.write("Building neighborhood:");
   for(int adjacencyX = -1; adjacencyX <= 1; adjacencyX += 2)
   {
      Neighbor neighbor((m_x + adjacencyX) % 4, m_y);
      m_neighborList.push_back(neighbor);
      m_log.write(std::to_string(neighbor.x) + "," + std::to_string(neighbor.y));
   }

   for(int adjacencyY = -1; adjacencyY <= 1; adjacencyY += 2)
   {
      Neighbor neighbor(m_x, (m_y + adjacencyY) % 4);
      m_neighborList.push_back(neighbor);
      m_log.write(std::to_string(neighbor.x) + "," + std::to_string(neighbor.y));
   }
}

void Node::execute()
{
   while(true)
   {
      m_log.write("Reading from message queue");
      std::string message;
      if(!m_messageQueue.read(message, m_processType))
      {
         continue;
      }

      m_log.write("Message received: ");

      switch(Utils::getProtocolId(message))
      {
         case IProtocol::ExecuteProgramPostponed:
         {
            ExecuteProgramPostponedProtocol epp;
            epp.parse(message);
            m_log.write(epp.fancy());
            treat(epp);
         }
         break;

         case IProtocol::NotifyScheduler:
         {
            NotifySchedulerProtocol ns;
            ns.parse(message);
            m_log.write(ns.fancy());
            treat(ns);
         }
         break;

         case IProtocol::Timestamp:
         {
            TimestampProtocol ts;
            ts.parse(message);
            m_log.write(ts.fancy());
            treat(ts);
         }
         break;

         default:
         {
            m_log.write("Message unknown: " + message);
         }
         break;
      }
   }
}

void Node::treat(const ExecuteProgramPostponedProtocol& epp)
{
   if(epp.getDestinationNode() == m_id)
   {
      m_log.write("Received execution: " + epp.fancy());

      m_ns.setNodeId(m_id);
      m_ns.setDelay(epp.getDelay());
      m_ns.setProgramName(epp.getProgramName());
      m_ns.setBeginTime(std::time(nullptr));
      m_ns.setSubmissionTime(epp.getSubmissionTime());
      m_ns.setPID(getpid());
      
      pid_t pid;

      pid = fork();

      if(pid == -1)
      {
         return;
      }

      if(pid == 0)
      {
         const auto path = "./" + epp.getProgramName();
         const auto arg = std::to_string(m_id);

         _exit(execl(path.c_str(), arg.c_str(), NULL));
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
      m_log.write("Sending notification to scheduler: " + ns.fancy());
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
      m_ns.setSuccess(ts.getSuccess());
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
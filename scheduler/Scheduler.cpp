#include "Scheduler.h"

#include "Node.h"
#include "Utils.h"

#include <iostream>

#include <unistd.h>

Scheduler::Scheduler()
   : m_messageQueue(MessageQueue::MainQueueKey)
   , m_nodeMap()
   , m_pendingExecutionList()
   , m_shutdown(false)
{
}

Scheduler::~Scheduler()
{
}

bool Scheduler::createNodes()
{
   pid_t pid;
   for(int nodeId = 0; nodeId < 16; ++nodeId)
   {
      pid = fork();

      if(pid == -1)
      {
         return false;
      }

      if(pid == 0)
      {
         Node node(nodeId);
         node.execute();

         _exit(0);
      }
      else
      {
         m_nodeMap.insert(std::make_pair(nodeId, Node::Free));
         // @TODO: Not working on OSX
         wait();
      }
   }

   return true;
}

int Scheduler::execute()
{
   if(!createNodes())
   {
      return 1;
   }

   while(true)
   {
      std::string message;
      if(!m_messageQueue.read(message, MessageQueue::SchedulerId))
      {
         std::cout << "[Scheduler] Failed to read message\n";
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

         case IProtocol::Shutdown:
         {
            ShutdownProtocol sd;
            treat(sd);
         }
         break;

         default:
         {
         }
         break;
      }

      if(m_shutdown)
      {
         bool canShutdown = true;
         for(const auto& node : m_nodeMap)
         {
            if(node.second == Node::Busy)
            {
               canShutdown = false;
               break;
            }
         }

         if(canShutdown)
         {
            printStatistics();
            break;
         }
      }
   }

   return 0;
}

// Send message to all nodes via node zero
void Scheduler::treat(ExecuteProgramPostponedProtocol& epp)
{
   sleep(epp.getDelay());
   for(auto& node : m_nodeMap)
   {
      epp.setDestinationNode(node.first);

      if(node.second == Node::Busy)
      {
         m_pendingExecutionList.push_back(epp);
         continue;
      }

      executeProgramPostponed(epp);
   }
}


void Scheduler::treat(NotifySchedulerProtocol& ns)
{
   m_nodeMap[ns.getNodeId()] = Node::Free;

   const auto makespan = ns.getEndTime() - ns.getBeginTime();

   std::string message;
   message.append("job=" + std::to_string(ns.getNodeId()) + ", ");
   message.append("arquivo=" + ns.getProgramName() + ", ");
   message.append("delay=" + std::to_string(ns.getDelay()) + ", ");
   message.append("makespan=" + std::to_string(makespan));

   std::cout << message << "\n";

   for(auto& pendingExecution : m_pendingExecutionList)
   {
      if(pendingExecution.getDestinationNode() == ns.getNodeId())
      {
         executeProgramPostponed(pendingExecution);
      }
   }
}

void Scheduler::treat(ShutdownProtocol& sd)
{
   m_shutdown = true;

}

void Scheduler::executeProgramPostponed(ExecuteProgramPostponedProtocol& epp)
{
   if(m_shutdown)
   {
      std::cout << epp.getProgramName() << " will not be executed for node " +
         std::to_string(epp.getDestinationNode()) + ". System is shuting down\n";
      return;
   }

   // Write to node zero
   if(m_messageQueue.write(epp.serialize(), MessageQueue::SchedulerId + 1))
   {
      m_nodeMap[epp.getDestinationNode()] = Node::Busy;
   }
}

void Scheduler::printStatistics()
{
   // @TODO

   std::cout << "Statistics\n";
}

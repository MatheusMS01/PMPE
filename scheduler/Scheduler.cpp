#include "Scheduler.h"

#include "Node.h"
#include "Utils.h"

#include <iostream>

#include <unistd.h>
#include <signal.h>
#include <ctime>

Scheduler::Scheduler()
   : m_messageQueue(MessageQueue::MainQueueKey)
   , m_nodeMap()
   , m_pendingExecutionList()
   , m_childPIDList()
   , m_executionLogList()
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
         m_childPIDList.push_back(pid);
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
               std::cout << "Node " << node.first << "is still busy\n";
               canShutdown = false;
               break; // for
            }
         }

         if(canShutdown)
         {
            for(auto childPID : m_childPIDList)
            {
               kill(childPID, SIGTERM);
            }

            printStatistics();

            m_messageQueue.remove();

            break; // while
         }
      }
   }

   return 0;
}

// Send message to all nodes via node zero
void Scheduler::treat(ExecuteProgramPostponedProtocol& epp)
{
   if(m_shutdown)
   {
      m_pendingExecutionList.push_back(epp);
      return;
   }
   epp.setSubmittalTime(time(NULL));
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
   m_executionLogList.push_back(ns);
   m_nodeMap[ns.getNodeId()] = Node::Free;

   const auto makespan = ns.getEndTime() - ns.getBeginTime();

   std::string message;
   message.append("job=" + std::to_string(ns.getNodeId()) + ", ");
   message.append("arquivo=" + ns.getProgramName() + ", ");
   message.append("delay=" + std::to_string(ns.getDelay()) +
                  " segundo" + (ns.getDelay() > 1 ? "s" : "") +  ", ");

   message.append("makespan=" + std::to_string(makespan) + 
                  " segundo" + (makespan > 1 ? "s" : ""));

   std::cout << message << "\n";

   for(size_t it = 0; it < m_pendingExecutionList.size(); ++it)
   {
      if(m_pendingExecutionList.at(it).getDestinationNode() == ns.getNodeId())
      {
         executeProgramPostponed(m_pendingExecutionList.at(it));
         m_pendingExecutionList.erase(m_pendingExecutionList.begin() + it);
         break;
      }
   }
}

void Scheduler::treat(ShutdownProtocol& sd)
{
   m_shutdown = true;

}

void Scheduler::executeProgramPostponed(ExecuteProgramPostponedProtocol& epp)
{
   // Write to node zero
   if(m_messageQueue.write(epp.serialize(), MessageQueue::SchedulerId + 1))
   {
      m_nodeMap[epp.getDestinationNode()] = Node::Busy;
   }
}

void Scheduler::printStatistics()
{
   std::string statistics;
   statistics.append("\n\n\n-----------------------------Statistics-------------------------\n");
   statistics.append("PID\tProgram Name\tSubmittal Time\tBegin Time\tEnd Time\n");
   statistics.append("----------------------------------------------------------------\n");
   for(auto& executionLog : m_executionLogList)
   {
      statistics.append(std::to_string(executionLog.getPID()));
      statistics.append("\t");

      statistics.append(executionLog.getProgramName());
      statistics.append("\t");

      auto submittalTime = executionLog.getSubmittalTime();
      struct tm *submittalTimeInfo = localtime(&submittalTime);
      statistics.append(std::to_string(submittalTimeInfo->tm_hour) + ":");
      statistics.append(std::to_string(submittalTimeInfo->tm_min) + ":");
      statistics.append(std::to_string(submittalTimeInfo->tm_sec) + "\t\t");

      auto beginTime = executionLog.getBeginTime();
      struct tm *beginTimeInfo = localtime(&beginTime);
      statistics.append(std::to_string(beginTimeInfo->tm_hour) + ":");
      statistics.append(std::to_string(beginTimeInfo->tm_min) + ":");
      statistics.append(std::to_string(beginTimeInfo->tm_sec) + "\t\t");

      auto endTime = executionLog.getEndTime();
      struct tm *endTimeInfo = localtime(&endTime);
      statistics.append(std::to_string(endTimeInfo->tm_hour) + ":");
      statistics.append(std::to_string(endTimeInfo->tm_min) + ":");
      statistics.append(std::to_string(endTimeInfo->tm_sec) + "\n");
   }

   std::cout << statistics << "\n";
}

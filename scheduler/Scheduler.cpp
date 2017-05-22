#include "Scheduler.h"

#include "Node.h"
#include "Utils.h"

#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctime>
#include <cstring>

namespace scheduler
{
void SignalHandler(int signal)
{
   switch(signal)
   {
      case SIGALRM:
      {
         MessageQueue messageQueue(MessageQueue::MainQueueKey);

         AlarmProtocol al;

         messageQueue.write(al.serialize(), MessageQueue::SchedulerId);
      }
      break;

      default:
      {
      }
      break;
   }
}
}

Scheduler::Scheduler()
   : m_messageQueue(MessageQueue::MainQueueKey)
   , m_nodeMap()
   , m_pendingExecutionList()
   , m_childPIDList()
   , m_executionLogList()
   , m_shutdown(false)
   , m_sequentialNumber(0)
   , m_log("Scheduler")
{
   // Set action for SIGALRM
   struct sigaction sa_alrm;

   memset(&sa_alrm, 0, sizeof(sa_alrm));
   sa_alrm.sa_handler = scheduler::SignalHandler;

   sigaction(SIGALRM, &sa_alrm, NULL);
}

bool Scheduler::createNodes()
{
   pid_t pid;
   for(unsigned int nodeId = 0; nodeId < 16; ++nodeId)
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
         m_log.write("Created node " + std::to_string(nodeId) + " with PID " + std::to_string(pid));

         m_childPIDList.push_back(pid);
         m_nodeMap.insert(std::make_pair(nodeId, Node::Free));
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
      m_log.write("Reading from message queue");
      std::string message;
      if(!m_messageQueue.read(message, MessageQueue::SchedulerId))
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
         m_log.write(epp.pretty());
         treat(epp);
      }
      break;

      case IProtocol::NotifyScheduler:
      {
         NotifySchedulerProtocol ns;
         ns.parse(message);
         m_log.write(ns.pretty());
         treat(ns);
      }
      break;

      case IProtocol::Shutdown:
      {
         ShutdownProtocol sd;
         m_log.write(sd.pretty());
         treat(sd);
      }
      break;

      case IProtocol::Alarm:
      {
         AlarmProtocol al;
         m_log.write(al.pretty());
         treat(al);
      }
      break;

      default:
      {
         m_log.write("Message unknown: " + message);
      }
      break;

      }

      if(m_shutdown)
      {
         const auto nodeItr = find_if(m_nodeMap.begin(), m_nodeMap.end(),
            [] (const std::pair<int, int>& itr)
            {
               return itr.second == Node::Busy;
            });

         const auto canShutdown = (nodeItr == m_nodeMap.end());

         if(canShutdown)
         {
            m_log.write("Shutting down!");
            // Children reaping
            for(const auto childPID : m_childPIDList)
            {
               m_log.write("\tReaping child " + std::to_string(childPID));

               // Send signal
               kill(childPID, SIGTERM);

               // Wait for child signal
               int status;
               wait(&status);
            }

            // @TODO: Show programs that were not executed

            printStatistics();

            m_log.write("Removing Message Queue");
            m_messageQueue.remove();

            break; // while
         }
         else
         {
            m_log.write("Can't shutdown!");
            for(const auto node : m_nodeMap)
            {
               if(node.second == Node::Busy)
               {
                  m_log.write("\tNode " + std::to_string(node.first) + " is still busy");   
               }
            }
         }
      }
   }

   return 0;
}

void Scheduler::treat(ExecuteProgramPostponedProtocol& epp)
{
   if(m_shutdown)
   {
      return;
   }

   unsigned int alarmTime;

   const auto currentTime = std::time(nullptr);

   // Time when alarm is supposed to stop
   const auto endTime = static_cast<unsigned int>(epp.getSubmissionTime() + epp.getDelay());

   if(endTime <= currentTime)
   {
      // Delay has already passed; no need to wait
      alarmTime = 0;
   }
   else
   {
      // Wait remaining time
      alarmTime = endTime - currentTime;
   }

   if(alarmTime == 0)
   {
      if(epp.getDestinationNode() == -1)
      {
         // User sent epp with delay = 0
         for(const auto& node : m_nodeMap)
         {
            // All nodes should execute
            epp.setDestinationNode(node.first);
            if(node.second == Node::Busy)
            {
               // Node is busy. Push it to pending execution list
               m_log.write("Node " + std::to_string(node.first) + " is busy");

               epp.setSequentialNumber(m_sequentialNumber++);
               m_pendingExecutionList.push_back(epp);
               continue; // for
            }

            executeProgramPostponed(epp);
         }
      }
      else
      {
         // Alarm timed out
         if(m_nodeMap[epp.getDestinationNode()] == Node::Busy)
         {
            // Node is still busy. Do nothing
            m_log.write("Node " + std::to_string(epp.getDestinationNode()) + " is busy! Do nothing");
            return;
         }

         executeProgramPostponed(epp);
      }
   }
   else
   {
      // User sent epp with delay > 0
      m_log.write("Setting alarm to " + std::to_string(alarmTime) + " second" + (alarmTime > 1 ? "s" : ""));

      const auto remaining = alarm(alarmTime);

      if(remaining < alarmTime && remaining != 0)
      {
         // If previous delay will finish before new one, stick to what was remaining
         m_log.write("Resetting to faster time out " + std::to_string(remaining) + " second" + (remaining > 1 ? "s" : ""));
         alarm(remaining);
      }

      if(epp.getSequentialNumber() != -1)
      {
         // This epp was already generated before
         return;
      }

      // This is a new epp
      for(const auto& node : m_nodeMap)
      {
         // All nodes should receive it
         epp.setDestinationNode(node.first);
         epp.setSequentialNumber(m_sequentialNumber++);
         m_pendingExecutionList.push_back(epp);
      }
   }
}

void Scheduler::treat(const NotifySchedulerProtocol& ns)
{
   m_log.write(std::to_string(ns.getNodeId()) + " is now free");
   m_nodeMap[ns.getNodeId()] = Node::Free;

   if(ns.getSuccess())
   {
      m_executionLogList.push_back(ns);

      const auto makespan = ns.getEndTime() - ns.getBeginTime();

      std::string message;
      message.append("job=" + std::to_string(ns.getNodeId()) + ", ");

      message.append("arquivo=" + ns.getProgramName() + ", ");

      message.append("delay=" + std::to_string(ns.getDelay()) +
                     " segundo" + (ns.getDelay() > 1 ? "s" : "") +  ", ");

      message.append("makespan=" + std::to_string(makespan) + 
                     " segundo" + (makespan > 1 ? "s" : ""));

      std::cout << message << "\n";
   }
   else
   {
      std::cout << "Node " << ns.getNodeId() << " failed to execute " << ns.getProgramName() << "\n";
   }

   const auto eppItr = find_if(m_pendingExecutionList.begin(), m_pendingExecutionList.end(), 
      [&] (const ExecuteProgramPostponedProtocol& itr)
      {
         return itr.getDestinationNode() == ns.getNodeId();
      });

   if(eppItr != m_pendingExecutionList.end())
   {
      m_log.write(std::to_string(ns.getNodeId()) + " has a pending execution");
      // Found a pending execution for this node
      treat(*eppItr);
   }
}

void Scheduler::treat(const ShutdownProtocol& sd)
{
   m_shutdown = true;
}

void Scheduler::treat(const AlarmProtocol& al)
{
   // This copy is needed because treat() will remove from m_pendingExecutionList
   const auto pendingExecutionList = m_pendingExecutionList;

   // Treat every pending execution
   for(auto pendingExecution : pendingExecutionList)
   {
      if(m_messageQueue.getBufferSize() + m_messageQueue.getCurrentBytes() >= m_messageQueue.getLenght())
      {
         return;
      }

      treat(pendingExecution);
   }
}

void Scheduler::executeProgramPostponed(const ExecuteProgramPostponedProtocol& epp)
{
   m_log.write("Sending execution: " + epp.pretty());
   // Write to node zero
   if(m_messageQueue.write(epp.serialize(), MessageQueue::NodeZeroId))
   {
      m_log.write("Node " + std::to_string(epp.getDestinationNode()) + " is now busy");
      m_nodeMap[epp.getDestinationNode()] = Node::Busy;

      // Remove from pending execution list(if there is) and set node state to busy
      const auto eppItr = find_if(m_pendingExecutionList.begin(), m_pendingExecutionList.end(), 
         [&] (const ExecuteProgramPostponedProtocol& itr)
         {
            return itr.getSequentialNumber() == epp.getSequentialNumber();
         });

      if(eppItr != m_pendingExecutionList.end())
      {
         m_pendingExecutionList.erase(eppItr);
      }
   }
   else
   {
      m_log.write("Failed to send execute!");
   }
}

void Scheduler::printStatistics()
{
   std::string statistics;
   statistics.append("\n-----------------------------Statistics-------------------------\n");
   statistics.append("PID\tProgram Name\tSubmission Time\tBegin Time\tEnd Time\n");
   statistics.append("----------------------------------------------------------------\n");
   for(const auto& executionLog : m_executionLogList)
   {
      statistics.append(std::to_string(executionLog.getPID()));
      statistics.append("\t");

      statistics.append(executionLog.getProgramName());
      statistics.append("\t");

      const auto submissionTime = executionLog.getSubmissionTime();
      const auto *submissionTimeInfo = localtime(&submissionTime);
      statistics.append((submissionTimeInfo->tm_hour < 10 ? "0" : "") + std::to_string(submissionTimeInfo->tm_hour) + ":");
      statistics.append((submissionTimeInfo->tm_min < 10 ? "0" : "") + std::to_string(submissionTimeInfo->tm_min) + ":");
      statistics.append((submissionTimeInfo->tm_sec < 10 ? "0" : "") + std::to_string(submissionTimeInfo->tm_sec) + "\t");

      const auto beginTime = executionLog.getBeginTime();
      const auto *beginTimeInfo = localtime(&beginTime);
      statistics.append((beginTimeInfo->tm_hour < 10 ? "0" : "") + std::to_string(beginTimeInfo->tm_hour) + ":");
      statistics.append((beginTimeInfo->tm_min < 10 ? "0" : "") + std::to_string(beginTimeInfo->tm_min) + ":");
      statistics.append((beginTimeInfo->tm_sec < 10 ? "0" : "") + std::to_string(beginTimeInfo->tm_sec) + "\t");

      const auto endTime = executionLog.getEndTime();
      const auto *endTimeInfo = localtime(&endTime);
      statistics.append((endTimeInfo->tm_hour < 10 ? "0" : "") + std::to_string(endTimeInfo->tm_hour) + ":");
      statistics.append((endTimeInfo->tm_min < 10 ? "0" : "") + std::to_string(endTimeInfo->tm_min) + ":");
      statistics.append((endTimeInfo->tm_sec < 10 ? "0" : "") + std::to_string(endTimeInfo->tm_sec) + "\n");
   }

   std::cout << statistics << "\n";

   m_log.write(statistics);
}

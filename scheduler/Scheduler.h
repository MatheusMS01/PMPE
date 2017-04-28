#pragma once

#include "MessageQueue.h"

#include "ExecuteProgramPostponedProtocol.h"
#include "NotifySchedulerProtocol.h"

#include <map>
#include <list>

class Scheduler
{
public:
   Scheduler();
   ~Scheduler();

   int execute();

private:
   MessageQueue m_messageQueue;
   std::map<int, int> m_nodeMap;

   bool createNodes();

   void treat(ExecuteProgramPostponedProtocol& eep);
   void treat(NotifySchedulerProtocol& ns);
};
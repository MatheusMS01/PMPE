#pragma once

#include "MessageQueue.h"
#include "ExecuteProgramPostponedProtocol.h"
#include "NotifySchedulerProtocol.h"
#include "TimestampProtocol.h"
#include <vector>
#include "Log.h"

class Node
{
public:
   enum State : int
   {
      Free = 0,
      Busy = 1,
   };

   explicit Node(const int id);
   ~Node() = default;

   void execute();
private:
   struct Neighbor
   {
      Neighbor(unsigned int x, unsigned int y)
         : x(x)
         , y(y)
      {
         id = x + (y * 4);
         processType = id + MessageQueue::SchedulerId + 1;
      }
      int id;
      unsigned int processType;
      unsigned int x;
      unsigned int y;
   };
   int m_id;
   unsigned int m_processType;
   MessageQueue m_messageQueue;
   unsigned int m_x;
   unsigned int m_y;
   std::vector<Neighbor> m_neighborList;
   bool m_waitingTimestamp;
   NotifySchedulerProtocol m_ns;
   Log m_log;

   void buildNeighborhood();
   void treat(const ExecuteProgramPostponedProtocol& epp);
   void treat(const NotifySchedulerProtocol& ns);
   void treat(const TimestampProtocol& ts);
   void route(const std::string& pdu, const int destinationNode);
};
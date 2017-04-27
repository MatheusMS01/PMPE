#pragma once

#include "MessageQueue.h"

class Node
{
public:
   enum State : int
   {
      Free = 0,
      Busy = 1,
   };

   Node(int id);
   ~Node();

   void execute();
private:
   int m_id;
   unsigned int m_processType;
   MessageQueue m_messageQueue;
};
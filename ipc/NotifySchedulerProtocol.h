#pragma once

#include "IProtocol.h"

class NotifySchedulerProtocol : public IProtocol
{
public:
   enum ParameterId : unsigned int
   {
      NodeId           = 0,
      BeginTime        = 1,
      EndTime          = 2,
      Delay            = 3,
      ProgramName      = 4,
      ParametersNumber = ProgramName + 1, 
   };

   NotifySchedulerProtocol();

   int getNodeId();
   void setNodeId(int nodeId);

   long int getBeginTime();
   void setBeginTime(const long int beginTime);

   long int getEndTime();
   void setEndTime(const long int endTime);

   void setDelay(int delay);
   int getDelay();

   void setProgramName(const std::string& programName);
   std::string& getProgramName();
};
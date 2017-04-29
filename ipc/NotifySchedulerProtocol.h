#pragma once

#include "IProtocol.h"

class NotifySchedulerProtocol : public IProtocol
{
public:
   enum ParameterId : unsigned int
   {
      NodeId           = 0,
      SubmittalTime    = 1,
      BeginTime        = 2,
      EndTime          = 3,
      Delay            = 4,
      ProgramName      = 5,
      PID              = 6,
      ParametersNumber = PID + 1, 
   };

   NotifySchedulerProtocol();

   int getNodeId();
   void setNodeId(int nodeId);

   long int getSubmittalTime();
   void setSubmittalTime(const long int submittalTime);

   long int getBeginTime();
   void setBeginTime(const long int beginTime);

   long int getEndTime();
   void setEndTime(const long int endTime);

   void setDelay(int delay);
   int getDelay();

   void setProgramName(const std::string& programName);
   std::string& getProgramName();

   void setPID(const pid_t pid);
   pid_t getPID();
};
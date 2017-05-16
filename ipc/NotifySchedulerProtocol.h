#pragma once

#include <sys/types.h>
#include "IProtocol.h"

class NotifySchedulerProtocol : public IProtocol
{
public:
   enum ParameterId : unsigned int
   {
      NodeId           = 0,
      SubmissionTime    = 1,
      BeginTime        = 2,
      EndTime          = 3,
      Delay            = 4,
      ProgramName      = 5,
      PID              = 6,
      ParametersNumber = PID + 1, 
   };

   NotifySchedulerProtocol();

   int getNodeId() const;
   void setNodeId(const int nodeId);

   long int getSubmissionTime() const;
   void setSubmissionTime(const long int submissionTime);

   long int getBeginTime() const;
   void setBeginTime(const long int beginTime);

   long int getEndTime() const;
   void setEndTime(const long int endTime);

   int getDelay() const;
   void setDelay(const int delay);

   std::string getProgramName() const;
   void setProgramName(const std::string& programName);

   pid_t getPID() const;
   void setPID(const pid_t pid);

   std::string fancy() const;
};
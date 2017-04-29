#pragma once

#include "IProtocol.h"

class ExecuteProgramPostponedProtocol : public IProtocol
{
public:
   enum ParameterId : unsigned int
   {
      Delay            = 0,
      ProgramName      = 1,
      DestinationNode  = 2,
      SubmittalTime    = 3,
      ParametersNumber = SubmittalTime + 1, 
   };

   ExecuteProgramPostponedProtocol();

   bool parse(const std::string& pdu);

   void setDelay(int delay);
   int getDelay();

   void setProgramName(const std::string& programName);
   std::string& getProgramName();

   void setDestinationNode(int destinationNode);
   int getDestinationNode();

   void setSubmittalTime(const long int submittalTime);
   long int getSubmittalTime();
};
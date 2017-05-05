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

   void setDelay(const int delay);
   int getDelay() const;

   void setProgramName(const std::string& programName);
   std::string getProgramName() const;

   void setDestinationNode(const int destinationNode);
   int getDestinationNode() const;

   void setSubmittalTime(const long int submittalTime);
   long int getSubmittalTime() const;
};
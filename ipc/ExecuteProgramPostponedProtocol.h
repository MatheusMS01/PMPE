#pragma once

#include "IProtocol.h"
#include "Field.h"

class ExecuteProgramPostponedProtocol : public IProtocol
{
public:
   enum ParameterId : unsigned int
   {
      SequentialNumber = 0,
      Delay            = 1,
      ProgramName      = 2,
      DestinationNode  = 3,
      SubmissionTime   = 4,
      ParametersNumber = SubmissionTime + 1, 
   };
   
   FIELD( SequentialNumber, uint32_t);
   FIELD( Delay, uint32_t);
   FIELD( ProgramName, std::string);
   FIELD( DestinationNode, uint8_t);
   FIELD( SubmissionTime, uint32_t);

   ExecuteProgramPostponedProtocol();

   bool parse(const std::string& pdu);

   void setDelay(const int delay);
   int getDelay() const;

   void setProgramName(const std::string& programName);
   std::string getProgramName() const;

   void setDestinationNode(const int destinationNode);
   int getDestinationNode() const;

   void setSubmissionTime(const long int submissionTime);
   long int getSubmissionTime() const;

   void setSequentialNumber(const int sequentialNumber);
   int getSequentialNumber() const;

   std::string pretty() const;
};

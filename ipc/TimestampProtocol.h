#pragma once

#include "IProtocol.h"

class TimestampProtocol : public IProtocol
{
public:
   enum ParameterId : unsigned int
   {
      Timestamp        = 0,
      Success          = 1,
      ParametersNumber = Success + 1, 
   };

   TimestampProtocol();

   long int getTimestamp() const;
   void setTimestamp(const long int timestamp);

   bool getSuccess() const;
   void setSuccess(const bool success);

   std::string fancy() const;
};
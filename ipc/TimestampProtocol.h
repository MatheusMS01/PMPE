#pragma once

#include "IProtocol.h"

class TimestampProtocol : public IProtocol
{
public:
   enum ParameterId : unsigned int
   {
      Timestamp        = 0,
      ParametersNumber = Timestamp + 1, 
   };

   TimestampProtocol();

   long int getTimestamp();
   void setTimestamp(const long int timestamp);
};
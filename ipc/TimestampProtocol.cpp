#include "TimestampProtocol.h"

TimestampProtocol::TimestampProtocol()
   : IProtocol(IProtocol::Timestamp, ParametersNumber)
{
}

void TimestampProtocol::setTimestamp(const long int timestamp)
{
   m_parameterList.at(Timestamp) = std::to_string(timestamp);
}

long int TimestampProtocol::getTimestamp() const
{
   try
   {
      return std::stol(m_parameterList.at(Timestamp));
   }
   catch(...)
   {
      return -1;
   }
}
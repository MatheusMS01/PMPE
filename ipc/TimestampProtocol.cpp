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

bool TimestampProtocol::getSuccess() const
{
   if(m_parameterList.at(Success) == "1")
   {
      return true;
   }
   else
   {
      return false;
   }
}

void TimestampProtocol::setSuccess(const bool success)
{
   m_parameterList.at(Success) = std::to_string(success);
}

std::string TimestampProtocol::fancy() const
{
   std::string message;

   message.append("Timestamp\n");
   message.append("\tTimestamp: " + m_parameterList.at(Timestamp));

   return message;
}
#pragma once

#include <string>
#include <vector>

class IProtocol
{
public:
   enum ProtocolId : unsigned int
   {
      ExecuteProgramPostponed = 0,
      NotifyScheduler         = 1,
      Shutdown                = 2,
      Timestamp               = 3,
      Alarm                   = 4,
   };

   using ParameterList = std::vector<std::string>;

   virtual std::string serialize() const;
   virtual bool parse(const std::string& pdu);
   virtual std::string fancy() const = 0;

protected:
   int m_id;
   unsigned int m_parametersNumber;
   ParameterList m_parameterList;
   
   IProtocol(const int id, const unsigned int parametersNumber);
};

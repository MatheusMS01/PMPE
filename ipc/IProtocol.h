#pragma once

#include <string>
#include <vector>

class IProtocol
{
public:
   enum ProtocolId : int
   {
      ExecuteProgramPostponed = 0,
      NotifyScheduler         = 1,
      Shutdown                = 2,
   };

   using ParameterList = std::vector<std::string>;

   virtual std::string serialize();
   virtual bool parse(const std::string& pdu);

protected:
   int m_id;
   unsigned int m_parametersNumber;
   ParameterList m_parameterList;
   
   IProtocol(int id, unsigned int parametersNumber);
};

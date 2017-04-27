#pragma once

#include <string>
#include <vector>

class IProtocol
{
public:
   enum ProtocolId : int
   {
      ExecuteProgramPostponed = 0,
      UpdateState             = 1,
      Shutdown                = 2,
   };

   using ContentList = std::vector<std::string>;

   virtual std::string serialize();
   virtual bool parse(const std::string& pdu);

protected:
   int m_id;
   int m_parametersNumber;
   ContentList m_contentList;
   
   IProtocol(int id, int parametersNumber);
};

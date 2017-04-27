#include "IProtocol.h"

#include <algorithm>

IProtocol::IProtocol(int id, int parametersNumber)
   : m_id(id)
   , m_parametersNumber(parametersNumber)
{
   m_contentList.resize(parametersNumber);
}

std::string IProtocol::serialize()
{
   std::string pdu;

   pdu.append(";" + std::to_string(m_id) + ";");
   for(const auto& content : m_contentList)
   {
      pdu.append(content + ";");
   }

   return pdu;
}

bool IProtocol::parse(const std::string& pdu)
{
   // Deduce number of parameters by counting number of ';' occurrences
   if(std::count(pdu.begin(), pdu.end(), ';') - 2 != m_parametersNumber)
   {
      // Invalid Pdu
      return false;
   }

   m_contentList.clear();
   m_contentList.resize(m_parametersNumber);

   std::string buffer;
   bool isProtocolId = true;
   int contentId = 0;
   for(const auto byte : pdu)
   {
      if(byte == ';')
      {
         if(buffer.empty())
         {
            continue;
         }

         // Don't insert protocol id in
         // content list
         if(!isProtocolId)
         {
            m_contentList.at(contentId) = buffer;
            ++contentId;
         }
         else
         {
            if(std::stoi(buffer) != m_id)
            {
               // Wrong protocol id
               return false;
            }
         }
         
         isProtocolId = false;

         buffer.clear();
         continue;
      }

      buffer += byte;
   }

   return true;
}
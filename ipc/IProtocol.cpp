#include "IProtocol.h"

#include "Utils.h"

#include <algorithm>

IProtocol::IProtocol(const int id, const unsigned int parametersNumber)
   : m_id(id)
   , m_parametersNumber(parametersNumber)
{
   m_parameterList.resize(parametersNumber);
}

std::string IProtocol::serialize() const
{
   std::string pdu;

   pdu.append(";" + std::to_string(m_id) + ";");
   for(const auto& parameter : m_parameterList)
   {
      pdu.append(parameter + ";");
   }

   return pdu;
}

bool IProtocol::parse(const std::string& pdu)
{
   auto parameterList = Utils::getPduContent(pdu);

   if(parameterList.size() < m_parametersNumber + 1)
   {
      return false;
   }

   if(parameterList.at(0) != std::to_string(m_id))
   {
      return false;
   }

   parameterList.erase(parameterList.begin());

   m_parameterList = parameterList;

   return true;
}
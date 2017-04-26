#include "Protocol.h"

// If pdu returns empty, buildPdu failed
void Protocol::buildPdu(const int protocolId,
                        std::string& pdu,
                        const ParameterList& parameterList)
{
   pdu.clear();

   switch(protocolId)
   {
      case ExecuteProgramPostponed:
      {
         if(parameterList.size() < EEPExpectedParameters)
         {
            return;
         }

         pdu.append(";" + std::to_string(protocolId) + ";");
         for(short index = 0; index < EEPExpectedParameters; ++index)
         {
            pdu.append(parameterList.at(index) + ";");
         }
      }
      break;

      case UpdateAvailability:
      {
         if(parameterList.size() < UAExpectedParameters)
         {
            return;
         }

         pdu.append(";" + std::to_string(protocolId) + ";");
         for(short index = 0; index < UAExpectedParameters; ++index)
         {
            pdu.append(";" + parameterList.at(index) + ";");
         }
      }
      break;

      default:
      {
      }
      break;
   }
}

// If parameterList returns empty, parsePdu failed
void Protocol::parsePdu(const std::string& pdu, ParameterList& parameterList)
{
   auto contentList = getPduContent(pdu);

   if(contentList.empty())
   {
      return;
   }

   switch(std::stoi(contentList.at(0)))
   {
      case ExecuteProgramPostponed:
      {
         if(contentList.size() - 1 < EEPExpectedParameters)
         {
            return;
         }

         for(short index = 1; index <= EEPExpectedParameters; ++index)
         {
            parameterList.push_back(contentList.at(index));
         }
      }
      break;

      case UpdateAvailability:
      {
         if(contentList.size() - 1 < UAExpectedParameters)
         {
            return;
         }

         for(short index = 1; index <= UAExpectedParameters; ++index)
         {
            parameterList.push_back(contentList.at(index));
         }
      }
      break;

      default:
      {
      }
      break;
   }
}

std::vector<std::string> Protocol::getPduContent(const std::string& pdu)
{
   std::vector<std::string> contentList;
   std::string buffer;
   for(const auto character : pdu)
   {
      if(character == ';')
      {
         if(!buffer.empty())
         {
            contentList.push_back(buffer);
            buffer.clear();
         }
         continue;
      }

      buffer += character;
   }

   return contentList;
}
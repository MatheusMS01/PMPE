#include "Utils.h"

int Utils::getProtocolId(const std::string& pdu)
{
   if(pdu.empty())
   {
      return -1;
   }

   auto contentList = getPduContent(pdu);

   return std::stoi(contentList.at(0));
}

std::vector<std::string> Utils::getPduContent(const std::string& pdu)
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
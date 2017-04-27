#include "Utils.h"
#include <cmath>

int Utils::getProtocolId(const std::string& pdu)
{
   if(pdu.empty())
   {
      return -1;
   }

   auto contentList = getPduContent(pdu);

   return std::stoi(contentList.at(0));
}

unsigned int Utils::distanceBetweenNodes(int nodeId1, int nodeId2)
{
   int x1 = nodeId1 % 4;
   int y1 = nodeId1 / 4;
   int x2 = nodeId2 % 4;
   int y2 = nodeId2 / 4;

   const auto distance = static_cast<unsigned int>(std::abs(static_cast<double>(x2 - x1)) + std::abs(static_cast<double>(y2 - y1)));

   return distance;
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

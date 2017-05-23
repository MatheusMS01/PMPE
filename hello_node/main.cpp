#include <iostream>
#include <unistd.h>

int main(int argc, char * argv[])
{
   sleep(10);
   std::cout << "[" << argv[0] << "] Executei no nodo " << argv[1] << "\n";

   return 0;
}
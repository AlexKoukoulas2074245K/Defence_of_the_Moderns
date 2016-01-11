/* ============================
   Author:       Alex Koukoulas
   Date:         9/1/2016
   File:         main.cpp
   Description: 
   ============================ */

#include <iostream>
#include <string>

typedef unsigned int uint32;

/* ===============================================
   Function: getNthFibonacci
   @param uin32 num: input 
   @return uint32: the fibonacci number Fib(input)
   =============================================== */
uint32 getNthFibonacci(const uint32 num)
{
    if (num == 0) return 0;
    if (num == 1) return 1;

    return getNthFibonacci(num - 1) +
           getNthFibonacci(num - 2);
}

int main() 
{
    std::string line;
    while (std::getline(std::cin, line)) 
    {
        std::cout << getNthFibonacci(std::stoi(line)) << std::endl;
    }
}

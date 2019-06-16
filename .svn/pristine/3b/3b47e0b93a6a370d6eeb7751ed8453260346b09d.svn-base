#include<iostream>
#include "PlayGame.h"
#include <chrono>

int main(int argc, char** argv)
{
   std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
   std::cout << "Main:Before Play Start" << std::endl;

   PlayGame* timeToWin = new PlayGame("");
   timeToWin->Play();
   delete timeToWin;

   std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
   std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds> (t2 - t1);
   std::cout << "Play has end and took milliseconds: " << duration.count() << std::endl;

   //system("pause");
   return 0;
}


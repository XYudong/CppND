#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"

#include <iostream>
#include <string>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

int main() {
  System system;
  NCursesDisplay::Display(system);

  // while (1) {
  //   auto vec = system.Processes();
  //   // std::cout << "length: " << vec.size() <<std::endl;
  //   for (int i = 0; i < 10; ++i) {
  //     int sec = vec[i].UpTime() % 60;
  //     std::cout << std::to_string(sec) << " ";
  //     if (sec > 60) {
  //       std::cout << sec << " " << vec[i].UpTime() << std::endl;
  //     }
      
  //   }
  //   std::cout << std::endl;
  //   std::this_thread::sleep_for (std::chrono::seconds(2));
  // }
}
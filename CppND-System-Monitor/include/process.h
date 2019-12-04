#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using std::string;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, string cmd);
  int Pid() const;                               // TODO: See src/process.cpp
  std::string User() const;                      // TODO: See src/process.cpp
  std::string Command() const;                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  // "const X& x" is equivalent to "X const& x", the later one is better
  
  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string command_;
  float cpu_utilization_;
};

#endif
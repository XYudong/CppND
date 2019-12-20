#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, string cmd) : pid_(pid), command_(cmd) {}

// Done: Return this process's ID
int Process::Pid() const { return pid_; }

// Done: Update and Return this process's CPU utilization
float Process::CpuUtilization() { 
  string line;
  string temp;
  long utime, stime, cutime, cstime;

  std::ifstream file_stream(LinuxParser::kProcDirectory + std::to_string(pid_) + LinuxParser::kStatFilename);
  if (file_stream.is_open()) {
    if (std::getline(file_stream, line)) {
      std::istringstream line_stream(line);
      for (int i = 1; i < 14; ++i) {
        // jump over the first 13 items
        if (!(line_stream >> temp)) {
          return 0;
        }
      }

      // ref: https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
      line_stream >> utime >> stime >> cutime >> cstime;
      long total_clicks = utime + stime + cutime + cstime;
      long seconds = LinuxParser::UpTime(pid_);

      // ratio [0, 1]
      float cpu_usage = (total_clicks / sysconf(_SC_CLK_TCK)) / static_cast<float>(seconds);

      // update
      cpu_utilization_ = cpu_usage;
      return cpu_usage;   
    }
  } else {
    cpu_utilization_ = 0;
  }

  return 0;
}

// TODO: Return the command that generated this process
string Process::Command() const { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() const { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {   
  // for sorting in descending order
  return this->cpu_utilization_ > a.cpu_utilization_;
}
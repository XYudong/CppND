#include "processor.h"

#include <string>
#include <fstream>
#include <sstream>

#include "linux_parser.h"

using std::string;

Processor::Processor() {
  // initialize class member attributes using aggregate CPU information at the moment
  string line;
  string cpu_name = "";
  std::ifstream file_stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (file_stream.is_open()) {
    if (std::getline(file_stream, line)) {
      std::istringstream line_stream(line);
      line_stream >> cpu_name;
      if (cpu_name == "cpu") {
        line_stream >> init_user_ >> init_nice_ >> init_system_ >> init_idle_ >> init_iowait_
         >> init_irq_ >> init_softirq_ >> init_steal_ >> init_guest_ >> init_guest_nice_;

        total_init_idle_ = init_idle_ + init_iowait_;
        total_init_nonidle_ = init_user_ + init_nice_ + init_system_ + init_irq_ + init_softirq_ + init_steal_;
      }
    }
  }
}

// Done: Return the aggregate CPU utilization
float Processor::Utilization() { 
  // simplified version, i.e. relative to the moment the object is initialized
  int user, nice, system, idle, iowait,
   irq, softirq, steal, guest, guest_nice;
  string line;
  string cpu_name;

  std::ifstream file_stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (file_stream.is_open()) {
    if (std::getline(file_stream, line)) {
      std::istringstream line_stream(line);
      
      if (line_stream >> cpu_name && cpu_name == "cpu") {
        line_stream >> user >> nice >> system >> idle >> iowait
         >> irq >> softirq >> steal >> guest >> guest_nice;

        // reference: https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
        int init_total = total_init_nonidle_ + total_init_idle_;
        int total_idle = idle + iowait;
        int total = total_idle + user + nice + system + irq + softirq + steal;

        int diff_total = total - init_total;
        int diff_idle = total_idle - total_init_idle_;

        return (diff_total - diff_idle) / static_cast<float>(diff_total);
      }
    }
  }
  
  return 0.0; 
}
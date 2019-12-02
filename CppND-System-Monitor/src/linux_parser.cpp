#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "linux_parser.h"
#include "format.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, temp, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> temp >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  int mem_total = -1;
  int mem_available = -1;
  string line, key;

  std::ifstream file_stream(kProcDirectory + kMeminfoFilename);
  if (file_stream.is_open()) {
    while (std::getline(file_stream, line)) {
      std::istringstream line_stream(line);
      line_stream >> key;
      if (key == "MemTotal:") {
        line_stream >> mem_total;
      } else if (key == "MemAvailable:") {
        line_stream >> mem_available;
      }

      if (mem_total >= 0 && mem_available >= 0) {
        return (mem_total - mem_available) / static_cast<float>(mem_total);
      }
    }
  } else {
    std::cout << "Failed to open: " + kProcDirectory + kMeminfoFilename << std::endl;
  }
  return 0.0;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream file_stream(kProcDirectory + kUptimeFilename);
  string line;
  long seconds = 0;
  if (file_stream.is_open()) {
    std::getline(file_stream, line);
    std::istringstream line_stream(line);
    line_stream >> seconds;
  } else {
    std::cout << "Failed to open: " + kProcDirectory + kUptimeFilename << std::endl;
  }
  return seconds;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int num = 0;
  string line;
  string key;

  std::ifstream file_stream(kProcDirectory + kStatFilename);
  if (file_stream.is_open()) {
    while (std::getline(file_stream, line)) {
      std::stringstream line_stream(line);
      if(line_stream >> key && key == "processes") {
        line_stream >> num;
        return num;
      }
    }
  }
  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int num = 0;
  string line;
  string key;

  std::ifstream file_stream(kProcDirectory + kStatFilename);
  if (file_stream.is_open()) {
    while (std::getline(file_stream, line)) {
      std::stringstream line_stream(line);
      if(line_stream >> key && key == "procs_running") {
        line_stream >> num;
        return num;
      }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream file_stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (file_stream.is_open()) {
    if (std::getline(file_stream, line)) {
      return line;
    }
  }
  return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  long value;
  std::ifstream file_stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (file_stream.is_open()) {
    while (std::getline(file_stream, line)) {
      std::stringstream line_stream(line);
      if (line_stream >> key && key == "VmSize:") {
        if (line_stream >> value) {
          return std::to_string(value / 1024);
        }
      }
    }
  }
  return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key, value;
  std::ifstream file_stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (file_stream.is_open()) {
    while (std::getline(file_stream, line)) {
      std::stringstream line_stream(line);
      if (line_stream >> key && key == "Uid:") {
        if (line_stream >> value) {
          return value;
        }
      }
    }
  }
  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line;
  string user, x, uid, temp;
  string target_uid = Uid(pid);

  std::ifstream file_stream(kPasswordPath);
  if (file_stream.is_open()) {
    while (std::getline(file_stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::stringstream line_stream(line);
      if (line_stream >> user >> x >> uid && uid == target_uid) {
        return user;
      }
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  string temp;
  long clock_ticks;
  std::ifstream file_stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (file_stream.is_open()) {
    if (std::getline(file_stream, line)) {
      std::istringstream line_stream(line);
      for (int i = 1; i < 22; ++i) {
        // jump over the first 21 items
        if (!(line_stream >> temp)) {
          return 0;
        }
      }

      // TODO: bug, unchanged clock_ticks ...
      if (line_stream >> clock_ticks) {
        return clock_ticks;
        // return clock_ticks / sysconf(_SC_CLK_TCK);
      }
    }
  }
  return 0;
}

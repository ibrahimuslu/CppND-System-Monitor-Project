#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stoi;
string parseMultiLine(string varName, string streamName){
  string line;
  string key;
  string value;
  std::ifstream filestream(streamName);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if(line.find("=")!= string::npos){
        std::replace(line.begin(), line.end(), ' ', '_');
        std::replace(line.begin(), line.end(), '=', ' ');
        std::replace(line.begin(), line.end(), '"', ' ');
      }
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == varName) {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return "0";
}
// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  return parseMultiLine("PRETTY_NAME", kOSPath);
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  return 1-(stof(parseMultiLine("MemAvailable:", kProcDirectory + kMeminfoFilename)) / stof(parseMultiLine("MemTotal:", kProcDirectory + kMeminfoFilename))) ;
 }

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime, idletime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return uptime;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long user,nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  }
  return user+nice+system+idle+iowait+irq+softirq+steal+guest+guest_nice;
 }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; } // unused 

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  long user,nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  }
  return user+nice+system+irq+softirq+steal+guest+guest_nice;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long user,nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  }
  return idle+iowait;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpuu;
  string utilizated = to_string( LinuxParser::ActiveJiffies()/ LinuxParser::Jiffies());
  cpuu.push_back(utilizated);
  return cpuu; 
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return stoi(parseMultiLine("processes", kProcDirectory + kStatFilename));
 }

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return stoi(parseMultiLine("procs_running", kProcDirectory + kStatFilename));
 }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string cmd;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cmd;
  }
  return cmd;
 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  int value;
  string varName = "VmSize";
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if(line.find(":")!= string::npos){
        std::replace(line.begin(), line.end(), ' ', '\0');
        std::replace(line.begin(), line.end(), ':', ' ');
        std::replace(line.begin(), line.end(), '"', '\0');
      }
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == varName) {
          return to_string(value/1024);
        }
      }
    }
  }
  return "0";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value;
  string varName = "Uid";
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if(line.find(":")!= string::npos){
        std::replace(line.begin(), line.end(), ' ', '\0');
        std::replace(line.begin(), line.end(), ':', ' ');
        std::replace(line.begin(), line.end(), '"', '\0');
      }
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == varName) {
          return value;
        }
      }
    }
  }
  return "0";
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line;
  string name;
  string x;
  string uid;
  string ouid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if(line.find(":")!= string::npos){
        std::replace(line.begin(), line.end(), ' ', '\0');
        std::replace(line.begin(), line.end(), ':', ' ');
        std::replace(line.begin(), line.end(), '"', '\0');
      }
      std::istringstream linestream(line);
      while (linestream >> name >> x >> uid) {
        if (uid == ouid) {
          return name;
        }
      }
    }
  }
  return "0";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long int LinuxParser::UpTime(int pid) { 
  long int starttime;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    std::istream_iterator<string> stats(linestream),end;
    vector<string>statss(stats,end);
    starttime = std::stol(statss[21])/sysconf(_SC_CLK_TCK);
  }
  return starttime;
 
}
float LinuxParser::CpuUtilization(int pid) { 
  float starttime, total_time, seconds, utilized;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    std::istream_iterator<string> stats(linestream),end;
    vector<string>statss(stats,end);
    starttime = std::stol(statss[21]);
    total_time = std::stol(statss[13]) + std::stol(statss[14]) + std::stol(statss[15]) + std::stol(statss[16]);
    seconds = UpTime() - (starttime / sysconf(_SC_CLK_TCK));
    utilized = (total_time / sysconf(_SC_CLK_TCK))/seconds;
    return utilized;
  }
  return 0;
 
}
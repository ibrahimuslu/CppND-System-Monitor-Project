#include "processor.h"
#include "linux_parser.h"
#include <string>
using std::string;
using std::stof;
// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float acc = 0.0f;
     for(string u : LinuxParser::CpuUtilization()){
         acc+=stof(u);
     }
      return acc/LinuxParser::CpuUtilization().size();
}
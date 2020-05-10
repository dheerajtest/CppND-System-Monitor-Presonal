#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid){pid_=pid;}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { 
  string line, key;
  long utime, stime, cutime, cstime, starttime, uptime, Hz, sec,total;
  int cnt = 1;
  float cpu_usg;
  string str = to_string(pid_);
  std::ifstream filestream(LinuxParser::kProcDirectory + str + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        switch (cnt) {
        case 14:
          utime = std::stol(key);break;
        case 15:
          stime = std::stol(key);break;
        case 16:
          cutime = std::stol(key);break;
        case 17:
          cstime = std::stol(key);break;
        case 22:
          starttime = std::stol(key);break;
        }
        cnt++;
      }
    }
  }
  uptime = LinuxParser::UpTime();
  Hz = sysconf(_SC_CLK_TCK);
  total = utime + stime + cutime + cstime;
  sec = uptime - (long)((float)starttime / (float)Hz);
  cpu_usg = (((float)total / (float)Hz) / (float)sec);
  return cpu_usg;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  return (a.CpuUtilization() < this->CpuUtilization());
}
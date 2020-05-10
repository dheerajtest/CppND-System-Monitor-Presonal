#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  string line;
  string key;
  string value;
  int t1=0,t2=0;//to break loop
  float MemTot,MemFre;
  std::ifstream filestr(kProcDirectory + kMeminfoFilename);
  if (filestr.is_open()) {
    while (std::getline(filestr, line)){
      std::istringstream linestr(line);
      while(linestr >>key >>value){
        if (key == "MemTotal:") {
          MemTot = stof(value);
          t1=1;
        }
        else if (key == "MemFree:") {
          MemFre = stof(value);
          t2=1;
        } 
      }
      if(t1==1 && t2==1){break;}
    }
  }
  return MemTot-MemFre;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line, key;
  std::ifstream filestr(kProcDirectory + kUptimeFilename);
  if (filestr.is_open()) {
    std::getline(filestr, line);
    std::istringstream linestr(line);
    linestr >> key;
  }
  return stol(key); 
}

// TODO: Read and return CPU utilization
string LinuxParser::CpuUtilization() { 
  string line;
  float PercentCpu;
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  std::ifstream filestr(kProcDirectory + kStatFilename);
  if (filestr.is_open()) {
    static long prevTotal = 0, prevIdle = 0;
    long Total, Idle;
    float total_diff, idle_diff;
    std::getline(filestr, line);
    std::istringstream linestr(line);
    linestr >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>softirq >> steal >> guest >> guest_nice;
    Total = stol(user) + stol(nice) + stol(system) + stol(idle) + stol(iowait) + stol(irq) +stol(softirq) + stol(steal) + stol(guest) + stol(guest_nice);
    Idle = stol(idle) + stol(iowait);
    total_diff = (float)(Total - prevTotal);
    idle_diff = (float)(Idle - prevIdle);
    PercentCpu = ((total_diff - idle_diff) / total_diff);
    prevTotal = Total;
    prevIdle = Idle;
  }
  return to_string(PercentCpu);
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  int tot_proc;
  std::ifstream filestr(kProcDirectory + kStatFilename);
  if (filestr.is_open()) {
    while (std::getline(filestr, line)) {
      std::istringstream linestr(line);
      linestr >> key >> value;
      if (key == "processes") {
        tot_proc = std::stoi(value);
      }
    }
  }
  return tot_proc;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  int run_proc;
  std::ifstream filestr(kProcDirectory + kStatFilename);
  if (filestr.is_open()) {
    while (std::getline(filestr, line)) {
      std::istringstream linestr(line);
      linestr >> key >> value;
      if (key == "procs_running") {
        run_proc = std::stoi(value);
      }
    }
  }
  return run_proc;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line, key, str;
  str = to_string(pid);
  std::ifstream filestr(kProcDirectory + str + kCmdlineFilename);
  if (filestr.is_open()) {
    std::getline(filestr, line);
    std::istringstream linestr(line);
    linestr >> key;
  }
  return key;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key, value, MB;
  double KB;
  string str = to_string(pid);
  std::ifstream filestr(kProcDirectory + str + kStatusFilename);
  if (filestr.is_open()) {
    while (std::getline(filestr, line)) {
      std::istringstream linestr(line);
      linestr >> key >> value;
      if (key == "VmSize:") {
        KB = std::stod(value);
      }
    }
    MB = std::to_string((int)(KB * 0.0009765625));
  }
  return MB;
 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, value, UID;
  string str = to_string(pid);
  std::ifstream filestr(kProcDirectory + str + kStatusFilename);
  if (filestr.is_open()) {
    while (std::getline(filestr, line)) {
      std::istringstream linestr(line);
      linestr >> key >> value;
      if (key == "Uid:") {
        UID = value;
      }
    }
  }
  return UID;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line, user, x, uid_, res;
  string UID = LinuxParser::Uid(pid);
  std::ifstream file(kPasswordPath);
  if (file.is_open()) {
    while (std::getline(file, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stream(line);
      while (stream >> user >> x >> uid_) {
        if (uid_ == UID)
          res = user;
      }
    }
  }
  return res;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, key;
  int cnt = 1;
  long uptime;
  string str = to_string(pid);
  std::ifstream filestr(kProcDirectory + str + kStatFilename);
  if (filestr.is_open()) {
    while (std::getline(filestr, line)) {
      std::istringstream linestr(line);
      while (linestr >> key) {
        if (cnt == 22) {
          uptime = std::stol(key);
        }
        cnt++;
      }
    }
  }
  uptime /= sysconf(_SC_CLK_TCK);
  return uptime;
}
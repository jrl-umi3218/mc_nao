#pragma once

#include <mc_rbdyn/RobotModule.h>
#include <mc_rbdyn_urdf/urdf.h>
#include <mc_robots/api.h>

#include <mc_rtc/logging.h>

namespace mc_robots
{
struct MC_ROBOTS_DLLAPI NAOCommonRobotModule : public mc_rbdyn::RobotModule
{
 public:
  NAOCommonRobotModule();

 protected:
  void readUrdf(const std::string& robotName, const std::vector<std::string>& filteredLinks);
  std::map<std::string, std::vector<double>> halfSittingPose(const rbd::MultiBody& mb) const;
  std::vector<std::map<std::string, std::vector<double>>> nominalBounds(const mc_rbdyn_urdf::Limits& limits) const;
  std::map<std::string, std::pair<std::string, std::string>> getConvexHull(const std::map<std::string, std::pair<std::string, std::string>>& files) const;
  std::map<std::string, std::pair<std::string, std::string>> stdCollisionsFiles(const rbd::MultiBody& mb) const;
  virtual const std::map<std::string, std::pair<std::string, std::string>>& convexHull() const;
  virtual const std::vector<std::map<std::string, std::vector<double>>>& bounds() const;
  virtual const std::map<std::string, std::vector<double>>& stance() const;

 public:
  std::vector<std::string> virtualLinks;
  std::vector<std::string> gripperLinks;
  std::map<std::string, std::vector<double>> halfSitting;
  mc_rbdyn_urdf::Limits limits;
  std::vector<std::string> filteredLinks;
};

struct MC_ROBOTS_DLLAPI NAONoHandRobotModule : public NAOCommonRobotModule
{
 public:
  NAONoHandRobotModule();
};

struct MC_ROBOTS_DLLAPI NAOWithHandRobotModule : public NAOCommonRobotModule
{
 public:
  NAOWithHandRobotModule();
};
}

/*TODO Provide a different constructor to allow WithHand/NoHand instantation */
extern "C" {
ROBOT_MODULE_API std::vector<std::string> MC_RTC_ROBOT_MODULE()
{
  return {"NAO", "NAONoHand"};
}
ROBOT_MODULE_API void destroy(mc_rbdyn::RobotModule* ptr) { delete ptr; }
ROBOT_MODULE_API mc_rbdyn::RobotModule* create(const std::string& n)
{
  if (n == "NAO")
  {
    return new mc_robots::NAOWithHandRobotModule();
  }
  else if (n == "NAONoHand")
  {
    return new mc_robots::NAONoHandRobotModule();
  }
  else
  {
    LOG_ERROR("NAO module Cannot create an object of type " << n)
    return nullptr;
  }
}
}

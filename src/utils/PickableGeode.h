#pragma once

#include <osg/Geode>

#include <functional>
namespace ConanQTOSGDemo {
class PickableGeode : public osg::Geode {
 public:
  void picked();
  void setCallback(const std::function<void()>& callback) {
    m_callback = callback;
  }

 private:
  std::function<void()> m_callback;
};
}  // namespace ConanQTOSGDemo

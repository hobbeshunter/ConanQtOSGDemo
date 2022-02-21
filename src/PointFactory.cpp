#include "PointFactory.h"

namespace ConanQTOSGDemo {

PointFactory::~PointFactory() {
  m_shouldExit = true;
  m_runConditionVariable.notify_all();
  if (m_factoryThread && m_factoryThread->joinable()) {
    m_factoryThread->join();
  }
}

void PointFactory::start() {
  m_factoryThread.reset(new std::thread([&]() {
    while (!m_shouldExit) {
      waitIfPaused();

      if (m_createPoints) {
        const Eigen::Vector3d point = Eigen::Vector3d::Random();
        emit pointCreated(point.normalized());
      } else {
        emit pointRemoved();
      }
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }));
}

void PointFactory::waitIfPaused() {
  if (!m_run) {
    std::unique_lock<decltype(m_runMutex)> lock(m_runMutex);
    m_runConditionVariable.wait(lock);
  }
}

void PointFactory::setRun(bool run) {
  m_run = run;

  if (m_run) {
    m_runConditionVariable.notify_one();
  }
}

void PointFactory::setCreatePoints(bool createPoints) {
  m_createPoints = createPoints;
}

}  // namespace ConanQTOSGDemo

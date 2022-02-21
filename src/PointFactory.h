#pragma once

#include <Eigen/Dense>
#include <QObject>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <thread>

namespace ConanQTOSGDemo {

class PointFactory : public QObject {
  Q_OBJECT

 public:
  ~PointFactory() override;
  void start();

 public slots:
  void setRun(bool run);
  void setCreatePoints(bool createPoints);

 signals:
  void pointCreated(const Eigen::Vector3d&) const;
  void pointRemoved() const;

 private:
  void waitIfPaused();

  std::atomic<bool> m_shouldExit = false;
  bool m_run = true;
  std::condition_variable m_runConditionVariable;
  std::mutex m_runMutex;
  std::atomic<bool> m_createPoints = true;
  std::shared_ptr<std::thread> m_factoryThread;
};
}  // namespace ConanQTOSGDemo
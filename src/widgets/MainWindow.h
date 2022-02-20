#pragma once

#include <QMainWindow>

#include <memory>

class QItemSelectionModel;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace ConanQTOSGDemo {
class PointFactory;
class QPointCloudModel;
class QPointCloudView;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *m_ui;
  std::unique_ptr<PointFactory> m_pointFactory;
  std::unique_ptr<QWidget> m_osgWidget;
  std::unique_ptr<QPointCloudModel> m_pointCloudModel;
  std::unique_ptr<QItemSelectionModel> m_pointCloudSelectionModel;
  std::unique_ptr<QPointCloudView> m_pointCloudView;
};

}  // namespace ConanQTOSGDemo

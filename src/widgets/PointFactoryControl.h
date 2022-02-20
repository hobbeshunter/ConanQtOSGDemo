#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class PointFactoryControl;
}
QT_END_NAMESPACE

namespace ConanQTOSGDemo {
class PointFactory;

class PointFactoryControl : public QWidget {
  Q_OBJECT

 public:
  explicit PointFactoryControl(QWidget* parent = nullptr);
  ~PointFactoryControl() override;

  void setFactory(PointFactory* pointFactory);

 protected slots:
  void pauseButtonClicked(bool checked);
  void resumeButtonClicked(bool checked);
  void createPointsRadioToggled(bool checked);

 signals:
  void runStateChanged(bool) const;
  void createPointsChanged(bool) const;

 private:
  void updatePauseResumeButtonState();
  Ui::PointFactoryControl* m_ui;
  bool m_run = true;
  PointFactory* m_pointFactory = nullptr;
};

}  // namespace ConanQTOSGDemo

#include "widgets/PointFactoryControl.h"

#include "../../ui/ui_PointFactoryControl.h"
#include "PointFactory.h"

namespace ConanQTOSGDemo {

PointFactoryControl::PointFactoryControl(QWidget* parent)
    : QWidget(parent), m_ui(new Ui::PointFactoryControl) {
  m_ui->setupUi(this);

  connect(m_ui->pauseButton, SIGNAL(clicked(bool)), this,
          SLOT(pauseButtonClicked(bool)));
  connect(m_ui->resumeButton, SIGNAL(clicked(bool)), this,
          SLOT(resumeButtonClicked(bool)));
  connect(m_ui->createPointsRadioButton, SIGNAL(toggled(bool)), this,
          SLOT(createPointsRadioToggled(bool)));
}

PointFactoryControl::~PointFactoryControl() { delete m_ui; }

void PointFactoryControl::setFactory(PointFactory* pointFactory) {
  Q_ASSERT(pointFactory);

  if (m_pointFactory) {
    disconnect(this, SIGNAL(runStateChanged(bool)), m_pointFactory,
               SLOT(setRun(bool)));
    disconnect(this, SIGNAL(createPointsChanged(bool)), m_pointFactory,
               SLOT(setCreatePoints(bool)));
  }

  m_pointFactory = pointFactory;

  connect(this, SIGNAL(runStateChanged(bool)), m_pointFactory,
          SLOT(setRun(bool)));
  connect(this, SIGNAL(createPointsChanged(bool)), m_pointFactory,
          SLOT(setCreatePoints(bool)));
}

void PointFactoryControl::pauseButtonClicked(bool checked) {
  if (checked) {
    auto prevRun = m_run;
    m_run = false;

    if (m_run != prevRun) {
      updatePauseResumeButtonState();
      emit runStateChanged(m_run);
    }
  }
}

void PointFactoryControl::resumeButtonClicked(bool checked) {
  if (checked) {
    auto prevRun = m_run;
    m_run = true;

    if (m_run != prevRun) {
      updatePauseResumeButtonState();
      emit runStateChanged(m_run);
    }
  }
}

void PointFactoryControl::updatePauseResumeButtonState() {
  m_ui->pauseButton->setChecked(!m_run);
  m_ui->resumeButton->setChecked(m_run);
}

void PointFactoryControl::createPointsRadioToggled(bool checked) {
  emit createPointsChanged(checked);
}

}  // namespace ConanQTOSGDemo

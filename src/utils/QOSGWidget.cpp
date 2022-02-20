#include "QOSGWidget.h"

#include "QPickHandler.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPoint>
#include <osg/Camera>
#include <osg/Material>
#include <osgGA/EventQueue>
#include <osgGA/TrackballManipulator>

#include "utils/ConversionsOSGQT.h"

namespace ConanQTOSGDemo {

QOSGWidget::QOSGWidget(QWidget* parent)
    : QOpenGLWidget(parent),
      m_graphicsWindow(new osgViewer::GraphicsWindowEmbedded(
          this->x(), this->y(), this->width(), this->height())),
      m_viewer(new osgViewer::Viewer),
      m_rootGroup(new osg::Group) {
  m_rootGroup->setCullingActive(false);
  m_viewer->setSceneData(m_rootGroup);

  auto* camera = m_viewer->getCamera();
  camera->setViewport(0, 0, this->width(), this->height());
  camera->setClearColor(osg::Vec4(0.9f, 0.9f, 1.f, 1.f));
  float aspectRatio =
      static_cast<float>(this->width()) / static_cast<float>(this->height());
  camera->setProjectionMatrixAsPerspective(50.f, aspectRatio, .1f, 1000.f);
  camera->setGraphicsContext(m_graphicsWindow);

  auto* manipulator = new osgGA::OrbitManipulator;
  m_viewer->setCameraManipulator(manipulator);
  manipulator->setAllowThrow(false);
  manipulator->setVerticalAxisFixed(true);
  const osg::Vec3d eye(0.0, 0.0, 2.0);
  const osg::Vec3d center(0.0, 0.0, 0.0);
  const osg::Vec3d up(1., 0.0, 0.0);
  manipulator->setTransformation(eye, center, up);

  m_viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
  m_viewer->addEventHandler(new QPickHandler());

  m_viewer->realize();

  this->setMinimumSize(300, 300);
}

void QOSGWidget::paintGL() { m_viewer->frame(); }

void QOSGWidget::resizeGL(int width, int height) {
  const int scaledWidth =
      width * static_cast<int>(window()->devicePixelRatio());
  const int scaledHeight =
      height * static_cast<int>(window()->devicePixelRatio());

  m_graphicsWindow->resized(0, 0, scaledWidth, scaledHeight);
  getEventQueue()->windowResize(0, 0, scaledWidth, scaledHeight);
  osg::Camera* camera = m_viewer->getCamera();
  camera->setViewport(0, 0, this->width(), this->height());
}

void QOSGWidget::initializeGL() {
  // osg::Geode* geode = dynamic_cast<osg::Geode*>(m_viewer->getSceneData());
  // osg::StateSet* stateSet = geode->getOrCreateStateSet();
  // osg::Material* material = new osg::Material;
  // material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
  // stateSet->setAttributeAndModes(material, osg::StateAttribute::ON);
  // stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
}

void QOSGWidget::mouseMoveEvent(QMouseEvent* const event) {
  const auto scaledMousePosition =
      scaleMousePositionByScreenResolution(event->pos());
  getEventQueue()->mouseMotion(scaledMousePosition.first,
                               scaledMousePosition.second);

  event->accept();
}

void QOSGWidget::mousePressEvent(QMouseEvent* const event) {
  const auto osgButton = qtButtonToOSGCode(event->button());

  if (osgButton > 0) {
    const auto scaledMousePosition =
        scaleMousePositionByScreenResolution(event->pos());
    getEventQueue()->mouseButtonPress(scaledMousePosition.first,
                                      scaledMousePosition.second, osgButton);

    event->accept();
  }
}

void QOSGWidget::mouseReleaseEvent(QMouseEvent* const event) {
  const auto osgButton = qtButtonToOSGCode(event->button());

  if (osgButton > 0) {
    const auto scaledMousePosition =
        scaleMousePositionByScreenResolution(event->pos());
    getEventQueue()->mouseButtonRelease(scaledMousePosition.first,
                                        scaledMousePosition.second, osgButton);

    event->accept();
  }
}

void QOSGWidget::wheelEvent(QWheelEvent* const event) {
  const QPoint numDegrees = event->angleDelta();

  const osgGA::GUIEventAdapter::ScrollingMotion motion =
      numDegrees.y() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP
                         : osgGA::GUIEventAdapter::SCROLL_DOWN;

  getEventQueue()->mouseScroll(motion);
  event->accept();
}

bool QOSGWidget::event(QEvent* const event) {
  bool handled = QOpenGLWidget::event(event);
  update();
  return handled;
}

osgGA::EventQueue* QOSGWidget::getEventQueue() {
  osgGA::EventQueue* eventQueue = m_graphicsWindow->getEventQueue();
  return eventQueue;
}

[[nodiscard]] std::pair<float, float>
QOSGWidget::scaleMousePositionByScreenResolution(
    const QPoint& mouseMotion) const {
  return {static_cast<float>(mouseMotion.x() * window()->devicePixelRatio()),
          static_cast<float>(mouseMotion.y() * window()->devicePixelRatio())};
}

void QOSGWidget::addChildNode(osg::Node* nodeToAppend) {
  Q_ASSERT(m_rootGroup);
  m_rootGroup->addChild(nodeToAppend);
}

void QOSGWidget::resizeEvent(QResizeEvent* event) {
  QOpenGLWidget::resizeEvent(event);

  for (auto* child : findChildren<QWidget*>()) {
    child->resize(size());
  }
}

}  // namespace ConanQTOSGDemo

#pragma once

#include <QOpenGLWidget>
#include <osg/Group>
#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/Viewer>

#include <utility>

class QMouseEvent;
class QEvent;
class QPoint;
namespace osg {
class Node;
}  // namespace osg

namespace osgGA {
class EventQueue;
}  // namespace osgGA

namespace ConanQTOSGDemo {

class QOSGWidget : public QOpenGLWidget {
  Q_OBJECT

 public:
  explicit QOSGWidget(QWidget* parent = nullptr);

  void addChildNode(osg::Node* nodeToAppend);

 protected:
  void paintGL() override;

  void resizeGL(int width, int height) override;

  void initializeGL() override;

  void mouseMoveEvent(QMouseEvent* event) override;

  void mousePressEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void wheelEvent(QWheelEvent* event) override;

  bool event(QEvent* event) override;

 protected:
  void resizeEvent(QResizeEvent* event) override;
  [[nodiscard]] osgGA::EventQueue* getEventQueue();
  [[nodiscard]] std::pair<float, float> scaleMousePositionByScreenResolution(
      const QPoint& mouseMotion) const;

  osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_graphicsWindow;
  osg::ref_ptr<osgViewer::Viewer> m_viewer;
  osg::ref_ptr<osg::Group> m_rootGroup;
};
}  // namespace ConanQTOSGDemo

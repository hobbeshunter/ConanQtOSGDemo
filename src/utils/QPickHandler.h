#pragma once

#include <osgGA/GUIEventHandler>

namespace osgViewer {
class View;
}  // namespace osgViewer

namespace ConanQTOSGDemo {
class QPickHandler : public osgGA::GUIEventHandler {
 public:
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
  bool handle(const osgGA::GUIEventAdapter& ea,
              osgGA::GUIActionAdapter& aa) override;
#pragma GCC diagnostic pop
  virtual bool pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea);
};
}  // namespace ConanQTOSGDemo

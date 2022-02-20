#include "QPickHandler.h"

#include "PickableGeode.h"

#include <osgViewer/View>

#include <iostream>

namespace ConanQTOSGDemo {

bool QPickHandler::handle(const osgGA::GUIEventAdapter& ea,
                          osgGA::GUIActionAdapter& aa) {
  auto* view = dynamic_cast<osgViewer::View*>(&aa);
  if (!view) {
    return false;
  }

  switch (ea.getEventType()) {
    case (osgGA::GUIEventAdapter::PUSH): {
      return pick(view, ea);
    }
    default:
      return false;
  }
}

bool QPickHandler::pick(osgViewer::View* view,
                        const osgGA::GUIEventAdapter& ea) {
  constexpr auto intersectionBoxHalfWidth = 0.01;
  osg::ref_ptr<osgUtil::PolytopeIntersector> intersector =
      new osgUtil::PolytopeIntersector(
          osgUtil::Intersector::PROJECTION,
          ea.getXnormalized() - intersectionBoxHalfWidth,
          ea.getYnormalized() - intersectionBoxHalfWidth,
          ea.getXnormalized() + intersectionBoxHalfWidth,
          ea.getYnormalized() + intersectionBoxHalfWidth);

  intersector->setIntersectionLimit(osgUtil::Intersector::LIMIT_NEAREST);
  osgUtil::IntersectionVisitor iv(intersector);
  view->getCamera()->accept(iv);

  if (!intersector->containsIntersections()) {
    return false;
  }

  const auto intersections = intersector->getIntersections();
  bool handled = false;
  for (auto hitr = intersections.cbegin(); hitr != intersections.cend(); ++hitr) {
    if (hitr->nodePath.empty()) {
      continue;
    }

    auto pickableGeode = dynamic_cast<PickableGeode*>(hitr->nodePath.back());
    if(pickableGeode) {
      pickableGeode->picked();
      handled = true;
    }
  }

  return handled;
}

}  // namespace ConanQTOSGDemo

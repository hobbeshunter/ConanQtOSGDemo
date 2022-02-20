#include "QAbstractOSGView.h"

#include "utils/QOSGWidget.h"

#include <QAbstractItemModel>
#include <QMouseEvent>
#include <QScrollBar>
#include <osg/Geode>
#include <osg/Geometry>

#include <iostream>

namespace ConanQTOSGDemo {

QAbstractOSGView::QAbstractOSGView(QWidget *parent)
    : QAbstractItemView(parent), m_rootGroup(new osg::Group) {
  setAutoFillBackground(false);
  setStyleSheet("background-color: rgba(0,0,0,0)");
  setFrameStyle(QFrame::NoFrame);
  this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  QOSGWidget *osgWidget = dynamic_cast<QOSGWidget *>(parent);
  Q_ASSERT(osgWidget);

  osgWidget->addChildNode(m_rootGroup);
}

QModelIndex QAbstractOSGView::indexAt(const QPoint &point) const {
  Q_UNUSED(point);

  return QModelIndex();
}

QRect QAbstractOSGView::visualRect(const QModelIndex &index) const {
  Q_UNUSED(index);
  return QRect();
}

void QAbstractOSGView::scrollTo(const QModelIndex &index, ScrollHint hint) {
  Q_UNUSED(index);
  Q_UNUSED(hint);
  // TODO
}

void QAbstractOSGView::setModel(QAbstractItemModel *model) {
  Q_ASSERT(model);
  if (model == this->model()) {
    return;
  }

  QAbstractItemView::setModel(model);

  m_rootGroup->removeChild(0, m_rootGroup->getNumChildren());
}

void QAbstractOSGView::setSelectionModel(QItemSelectionModel *selectionModel) {
  Q_ASSERT(selectionModel);
  if (selectionModel == this->selectionModel()) {
    return;
  }

  QAbstractItemView::setSelectionModel(selectionModel);
}

void QAbstractOSGView::dataChanged(const QModelIndex &topLeft,
                                   const QModelIndex &bottomRight,
                                   const QList<int> &roles) {
  QAbstractItemView::dataChanged(topLeft, bottomRight, roles);
}

void QAbstractOSGView::rowsRemoved(const QModelIndex &parent, int start,
                                   int end) {
  Q_UNUSED(parent);
  m_rootGroup->removeChildren(static_cast<unsigned int>(start),
                              static_cast<unsigned int>(end - start) + 1);
}

QModelIndex QAbstractOSGView::moveCursor(CursorAction cursorAction,
                                         Qt::KeyboardModifiers modifiers) {
  Q_UNUSED(cursorAction);
  Q_UNUSED(modifiers)
  // TODO
  return QModelIndex();
}

int QAbstractOSGView::horizontalOffset() const { return 0; }

int QAbstractOSGView::verticalOffset() const { return 0; }

bool QAbstractOSGView::isIndexHidden(const QModelIndex &index) const {
  Q_UNUSED(index);
  return false;
}

void QAbstractOSGView::setSelection(
    const QRect &rect, QItemSelectionModel::SelectionFlags command) {
  Q_UNUSED(rect);
  Q_UNUSED(command);
  // TODO
}

QRegion QAbstractOSGView::visualRegionForSelection(
    const QItemSelection &selection) const {
  Q_UNUSED(selection);
  return QRegion();
}


void QAbstractOSGView::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
}

void QAbstractOSGView::mouseMoveEvent(QMouseEvent *const event) {
  event->ignore();
}

void QAbstractOSGView::mousePressEvent(QMouseEvent *const event) {
  event->ignore();
}

void QAbstractOSGView::mouseReleaseEvent(QMouseEvent *const event) {
  event->ignore();
}

void QAbstractOSGView::mouseDoubleClickEvent(QMouseEvent *event) {
  event->ignore();
}

void QAbstractOSGView::wheelEvent(QWheelEvent *event) { event->ignore(); }

}  // namespace ConanQTOSGDemo

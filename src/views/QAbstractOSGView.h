#pragma once

#include <QtWidgets/qtwidgetsglobal.h>
#include <QAbstractItemView>
#include <osg/Group>
#include <osg/ref_ptr>

namespace osg {
class Node;
}

namespace ConanQTOSGDemo {

class QAbstractOSGView : public QAbstractItemView {
  Q_OBJECT

 public:
  explicit QAbstractOSGView(QWidget *parent = nullptr);

  [[nodiscard]] QModelIndex indexAt(const QPoint &point) const override;
  [[nodiscard]] QRect visualRect(const QModelIndex &index) const override;
  // NOLINTNEXTLINE(google-default-arguments)
  void scrollTo(const QModelIndex &index,
                ScrollHint hint = EnsureVisible) override;
  void setModel(QAbstractItemModel *model) override;
  void setSelectionModel(QItemSelectionModel *selectionModel) override;

 protected:
  void paintEvent(QPaintEvent *event) override;
  void mouseMoveEvent(QMouseEvent *const event) override;
  void mousePressEvent(QMouseEvent *const event) override;
  void mouseReleaseEvent(QMouseEvent *const event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

 protected slots:
  // NOLINTNEXTLINE(google-default-arguments)
  void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                   const QList<int> &roles = QList<int>()) override;
  void rowsRemoved(const QModelIndex &parent, int start, int end);

 protected:
  QModelIndex moveCursor(CursorAction cursorAction,
                         Qt::KeyboardModifiers modifiers) override;
  [[nodiscard]] int horizontalOffset() const override;
  [[nodiscard]] int verticalOffset() const override;
  [[nodiscard]] bool isIndexHidden(const QModelIndex &index) const override;
  void setSelection(const QRect &rect,
                    QItemSelectionModel::SelectionFlags command) override;
  [[nodiscard]] QRegion visualRegionForSelection(
      const QItemSelection &selection) const override;

  Q_DISABLE_COPY(QAbstractOSGView)

  osg::ref_ptr<osg::Group> m_rootGroup;
};

}  // namespace ConanQTOSGDemo

#pragma once

#include "views/QAbstractOSGView.h"

namespace osg {
class Node;
}  // namespace osg

namespace ConanQTOSGDemo {

class QPointCloudView : public QAbstractOSGView {
  Q_OBJECT

 public:
  explicit QPointCloudView(QWidget *parent = nullptr);

  void setModel(QAbstractItemModel *model) override;
  void setSelectionModel(QItemSelectionModel *selectionModel) override;

 protected slots:
  void rowsInserted(const QModelIndex &parent, int start, int end) override;
  void selectionChanged(const QItemSelection &selected,
                        const QItemSelection &deselected) override;

 signals:
  void pointSelected(const QItemSelection &selection,
                     QItemSelectionModel::SelectionFlags command) const;

 private:
  Q_DISABLE_COPY(QPointCloudView)

  void createGeodes();
  void createGeode(int row);
};

}  // namespace ConanQTOSGDemo

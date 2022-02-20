#include "models/QPointCloudModel.h"

#include <iostream>

namespace ConanQTOSGDemo {

QPointCloudModel::QPointCloudModel(QObject *parent)
    : QAbstractTableModel(parent) {}

QPointCloudModel::~QPointCloudModel() = default;

// NOLINTNEXTLINE(google-default-arguments)
int QPointCloudModel::rowCount(const QModelIndex &) const {
  return static_cast<int>(m_points.size());
}

// NOLINTNEXTLINE(google-default-arguments)
int QPointCloudModel::columnCount(const QModelIndex &) const { return 4; }

QVariant QPointCloudModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole && index.row() < rowCount()) {
    if (index.column() == 0) {
      return m_points.at(static_cast<size_t>(index.row())).first;
    } else if (index.column() >= indexX && index.column() <= indexZ) {
      return m_points.at(static_cast<size_t>(index.row()))
          .second(index.column() - 1);
    }
  }
  return QVariant();
}

QVariant QPointCloudModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return QString("ID");
      case 1:
        return QString("x");
      case 2:
        return QString("y");
      case 3:
        return QString("z");
    }
  }
  return QVariant();
}

bool QPointCloudModel::insertRow(int row, const QModelIndex &parent,
                                 const Eigen::Vector3d &newRow) {
  if (row > rowCount()) {
    return false;
  }

  beginInsertRows(parent, row, row);
  m_points.insert(m_points.begin() + row, std::make_pair(++m_highestId, newRow));
  endInsertRows();
  return true;
}

bool QPointCloudModel::insertRows(int row, int count,
                                  const QModelIndex &parent) {
  if (row > rowCount()) {
    return false;
  }

  beginInsertRows(parent, row, row + count - 1);
  for (int idx = 1; idx <= count; idx++) {
    m_points.insert(m_points.begin() + row + idx - 1,
                    std::make_pair(++m_highestId, Eigen::Vector3d::Zero()));
  }
  endInsertRows();
  return true;
}

bool QPointCloudModel::removeRows(int row, int count,
                                  const QModelIndex &parent) {
  if (row + count - 1 >= rowCount()) {
    return false;
  }

  beginRemoveRows(parent, row, row + count - 1);
  m_points.erase(m_points.begin() + row, m_points.begin() + row + count);
  endRemoveRows();
  return true;
}

void QPointCloudModel::removeFirstRow() { removeRow(0); }
void QPointCloudModel::appendRow(const Eigen::Vector3d &newRow) {
  insertRow(rowCount(), QModelIndex(), newRow);
}

}  // namespace ConanQTOSGDemo

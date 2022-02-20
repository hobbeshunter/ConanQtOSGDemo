#pragma once

#include <Eigen/Dense>
#include <QAbstractTableModel>

#include "utils/EigenUtils.h"

namespace ConanQTOSGDemo {

class QPointCloudModel : public QAbstractTableModel {
 public:
  using index_t = int;
  static constexpr int indexId = 0;
  static constexpr int indexX = 1;
  static constexpr int indexY = 2;
  static constexpr int indexZ = 3;

  Q_OBJECT

 public:
  explicit QPointCloudModel(QObject *parent = nullptr);

  ~QPointCloudModel() override;

  // NOLINTNEXTLINE(google-default-arguments)
  [[nodiscard]] int rowCount(
      const QModelIndex &parent = QModelIndex()) const override;

  // NOLINTNEXTLINE(google-default-arguments)
  [[nodiscard]] int columnCount(
      const QModelIndex &parent = QModelIndex()) const override;

  [[nodiscard]] QVariant data(const QModelIndex &index,
                              int role) const override;

  [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation,
                                    int role) const override;

  bool insertRow(int row, const QModelIndex &parent = QModelIndex(),
                 const Eigen::Vector3d &newRow = Eigen::Vector3d::Zero());
  bool insertRows(int row, int count, const QModelIndex &parent) override;
  bool removeRows(int row, int count, const QModelIndex &parent) override;

 public slots:
  void removeFirstRow();
  void appendRow(const Eigen::Vector3d &newRow);

 protected:
  Eigen::aligned_vector<std::pair<index_t, Eigen::Vector3d>> m_points;
  index_t m_highestId = 0;
};

}  // namespace ConanQTOSGDemo

#include "QPointCloudView.h"

#include "models/QPointCloudModel.h"
#include "utils/PickableGeode.h"
#include "utils/QOSGWidget.h"

#include <QAbstractItemModel>
#include <QMouseEvent>
#include <QScrollBar>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/Point>
#include <osg/Vec3>

#include <iostream>

namespace ConanQTOSGDemo {
namespace {
class Point : public PickableGeode {
 public:
  using GetIndex = std::function<unsigned int(const osg::Node *)>;

  Point(const QPointCloudView &pointcloudView,
        const GetIndex& getIndex, float x,
        float y, float z, bool isSelected = false) :
      m_getIndex(getIndex) {
    setCallback([&pointcloudView, this]() -> void {
      auto selectedRow = this->m_getIndex(this);
      emit pointcloudView.pointSelected(
          QItemSelection(
              pointcloudView.model()->index(static_cast<int>(selectedRow),
                                            QPointCloudModel::indexId),
              pointcloudView.model()->index(static_cast<int>(selectedRow),
                                            QPointCloudModel::indexZ)),
          QItemSelectionModel::ClearAndSelect);
    });

    auto *pointsGeom = new osg::Geometry();

    auto *vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(x, y, z));

    pointsGeom->setVertexArray(vertices);

    auto *normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
    pointsGeom->setNormalArray(normals, osg::Array::BIND_OVERALL);

    pointsGeom->addPrimitiveSet(new osg::DrawArrays(
        osg::PrimitiveSet::POINTS, 0, static_cast<GLsizei>(vertices->size())));

    pointsGeom->getOrCreateStateSet()->setAttribute(new osg::Point(6.0f),
                                                    osg::StateAttribute::ON);

    addDrawable(pointsGeom);

    if (isSelected) {
      select();
    } else {
      unselect();
    }

    setCullingActive(false);
  }

  void select() {
    getDrawable(0)->asGeometry()->setColorArray(m_blueArray,
                                                osg::Array::BIND_OVERALL);
  }

  void unselect() {
    getDrawable(0)->asGeometry()->setColorArray(m_brownArray,
                                                osg::Array::BIND_OVERALL);
  }

 private:
  static const inline osg::ref_ptr<osg::Vec4Array> m_brownArray = ([]() {
    auto array = new osg::Vec4Array;
    array->push_back(osg::Vec4(0.6f, 0.4f, 0.0f, 1.0f));
    return array;
  })();
  static const inline osg::ref_ptr<osg::Vec4Array> m_blueArray = ([]() {
    auto array = new osg::Vec4Array;
    array->push_back(osg::Vec4(0.1f, 0.1f, 0.8f, 1.0f));
    return array;
  })();
  GetIndex m_getIndex;
};
}  // namespace

QPointCloudView::QPointCloudView(QWidget *parent) : QAbstractOSGView(parent) {}

void QPointCloudView::setModel(QAbstractItemModel *model) {
  Q_ASSERT(model);
  if (model == this->model()) {
    return;
  }
  // let's disconnect from the old model
  if (this->model()) {
    disconnect(this->model(), &QAbstractItemModel::rowsRemoved, this,
               &QPointCloudView::rowsRemoved);
  }

  QAbstractOSGView::setModel(model);

  if (this->model()) {  // and connect to the new one
    connect(this->model(), &QAbstractItemModel::rowsRemoved, this,
            &QPointCloudView::rowsRemoved);
  }

  createGeodes();
}

void QPointCloudView::setSelectionModel(QItemSelectionModel *selectionModel) {
  Q_ASSERT(selectionModel);
  if (selectionModel == this->selectionModel()) {
    return;
  }

  if (this->selectionModel()) {
    connect(this,
            SIGNAL(pointSelected(QItemSelection,
                                 QItemSelectionModel::SelectionFlags)),
            this->selectionModel(),
            SLOT(select(QItemSelection, QItemSelectionModel::SelectionFlags)));
  }

  QAbstractOSGView::setSelectionModel(selectionModel);

  if (this->selectionModel()) {
    connect(this,
            SIGNAL(pointSelected(QItemSelection,
                                 QItemSelectionModel::SelectionFlags)),
            this->selectionModel(),
            SLOT(select(QItemSelection, QItemSelectionModel::SelectionFlags)));
  }
}

void QPointCloudView::rowsInserted(const QModelIndex &parent, int start,
                                   int end) {
  Q_UNUSED(parent);
  for (auto row = start; row <= end; ++row) {
    createGeode(row);
  }
}

void QPointCloudView::selectionChanged(const QItemSelection &selected,
                                       const QItemSelection &deselected) {
  for (const auto deselectIdx : deselected.indexes()) {
    auto point = dynamic_cast<Point *>(
        m_rootGroup->getChild(static_cast<unsigned int>(deselectIdx.row())));
    if (point) {
      point->unselect();
    }
  }

  for (const auto selectIdx : selected.indexes()) {
    auto point = dynamic_cast<Point *>(
        m_rootGroup->getChild(static_cast<unsigned int>(selectIdx.row())));
    if (point) {
      point->select();
    }
  }
}

void QPointCloudView::createGeodes() {
  for (auto row = 0; row < model()->rowCount(rootIndex()); ++row) {
    createGeode(row);
  }
}

void QPointCloudView::createGeode(int row) {
  QModelIndex indexX =
      model()->index(row, QPointCloudModel::indexX, rootIndex());
  QModelIndex indexY =
      model()->index(row, QPointCloudModel::indexY, rootIndex());
  QModelIndex indexZ =
      model()->index(row, QPointCloudModel::indexZ, rootIndex());

  auto *point = new Point(
      *this,
      [this](const osg::Node *node) {
        return this->m_rootGroup->getChildIndex(node);
      },
      model()->data(indexX).toFloat(), model()->data(indexY).toFloat(),
      model()->data(indexZ).toFloat(),
      this->selectionModel()->isRowSelected(row));

  m_rootGroup->insertChild(static_cast<unsigned int>(row), point);
}

}  // namespace ConanQTOSGDemo

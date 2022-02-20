#include "MainWindow.h"

#include "PointFactory.h"

#include "../../ui/ui_MainWindow.h"
#include "models/QPointCloudModel.h"
#include "utils/QOSGWidget.h"
#include "views/QPointCloudView.h"

namespace ConanQTOSGDemo {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_ui(new Ui::MainWindow),
      m_pointFactory(new PointFactory),
      m_osgWidget(new ConanQTOSGDemo::QOSGWidget(this)),
      m_pointCloudModel(new QPointCloudModel),
      m_pointCloudSelectionModel(
          new QItemSelectionModel(m_pointCloudModel.get())),
      m_pointCloudView(new QPointCloudView(m_osgWidget.get())) {
  m_ui->setupUi(this);

  setCentralWidget(m_osgWidget.get());

  m_ui->tableView->setModel(m_pointCloudModel.get());
  m_ui->tableView->setSelectionModel(m_pointCloudSelectionModel.get());

  m_pointCloudView->setModel(m_pointCloudModel.get());
  m_pointCloudView->setSelectionModel(m_pointCloudSelectionModel.get());

  connect(m_pointFactory.get(), &PointFactory::pointCreated,
          m_pointCloudModel.get(), &QPointCloudModel::appendRow);
  connect(m_pointFactory.get(), &PointFactory::pointRemoved,
          m_pointCloudModel.get(), &QPointCloudModel::removeFirstRow);
  m_ui->pointFactoryControl->setFactory(m_pointFactory.get());

  m_pointFactory->start();
}

MainWindow::~MainWindow() { delete m_ui; }
}  // namespace ConanQTOSGDemo

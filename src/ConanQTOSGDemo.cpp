#include "widgets/MainWindow.h"

#include <QApplication>

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  ConanQTOSGDemo::MainWindow mainWindow;
  mainWindow.show();
  return app.exec();
}

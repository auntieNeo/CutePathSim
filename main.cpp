#include <QtGui>

#include "mainWindow.h"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  CutePathSim::MainWindow mainWindow;
  mainWindow.show();

  return app.exec();
}

#include <QtGui>

#include "mainWindow.h"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  CutePathSim::MainWindow mainWindow;
  mainWindow.show();

  return app.exec();
}

/**
 * \mainpage CutePathSim Documentation
 * \section intro_section Introduction
 * CutePathSim is a graphical, object oriented simulation of the data path in a digital system.
 *
 * \section basics_section Basics
 * Something something something.
 */

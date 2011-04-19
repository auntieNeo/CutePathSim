CONFIG += debug
message($$CONFIG)
VERSION = 0.0.1
HEADERS += assembler.h common.h component.h componentDataModel.h componentGraph.h componentGraphScene.h componentGraphView.h edge.h graphLayoutManager.h inputDialogs.h interface.h mainWindow.h
SOURCES += assembler.cpp common.cpp component.cpp componentDataModel.cpp componentGraph.cpp componentGraphScene.cpp componentGraphView.cpp edge.cpp graphLayoutManager.cpp inputDialogs.cpp interface.cpp main.cpp mainWindow.cpp
include(components/components.pri)
FORMS += interfacesWidget.ui mainWindow.ui stringDialog.ui
#TRANSLATIONS += cutePathSim_ja.ts
QT += opengl
unix {
  TARGET = cutepathsim
  target.path = /usr/local/bin
  LIBS += -lgvc -lgraph -lcgraph
}
win32 {
  TARGET = CutePathSim
}
macx {
  TARGET = CutePathSim
  release {
    QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
    CONFIG += x86 ppc
  }
} 
INSTALLS += target
documentation {
  QT_DOCUMENTATION_DIR = /usr/share/doc/qt/html
  system(doxytag -t qt.tag $$QT_DOCUMENTATION_DIR )
  system(doxygen)
  system(cd ./documentation/html && ./installdox -l qt.tag@http://doc.trolltech.com/$$QT_MAJOR_VERSION.$$QT_MINOR_VERSION/)
}

CONFIG += debug
message($$CONFIG)
VERSION = 0.0.1
HEADERS += common.h component.h componentGraph.h componentGraphScene.h componentGraphView.h edge.h interface.h mainWindow.h adder.h 32_bit_adder.cpp
SOURCES += component.cpp componentGraph.cpp componentGraphScene.cpp componentGraphView.cpp edge.cpp interface.cpp main.cpp mainWindow.cpp adder.cpp 32_bit_adder.cpp
include(components/components.pri)
FORMS += mainWindow.ui
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

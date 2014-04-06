#include <QtGui/QApplication>
#include <QtGui/QGtkStyle>

#include "UIMain.hpp"

int main(int argc, char* argv[])
{
   using namespace qTuner;
   QApplication::setGraphicsSystem("raster");
   QApplication application(argc,argv);
   #ifndef _WIN32
   application.setStyle(new QGtkStyle());
   #endif // _WIN32
   UIMain ui;
   ui.show();
   return application.exec();
}

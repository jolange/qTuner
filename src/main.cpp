#include <QApplication>
#include <QStyleFactory>

#include "UIMain.hpp"

int main(int argc, char* argv[])
{
   QCoreApplication::setOrganizationName("io.github.jolange");
   QCoreApplication::setOrganizationDomain("jolange.github.io");
   QCoreApplication::setApplicationName("qTuner");
   QApplication application(argc,argv);
   #ifndef _WIN32
   application.setStyle(QStyleFactory::create("GTK+"));
   #endif // _WIN32
   qTuner::UIMain ui;
   ui.show();
   return application.exec();
}

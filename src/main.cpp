#include <QApplication>
#include <QStyleFactory>

#include "UIMain.hpp"

int main(int argc, char* argv[])
{
   using namespace qTuner;
   QCoreApplication::setOrganizationName("io.github.johannes-lange");
   QCoreApplication::setOrganizationDomain("johannes-lange.github.io");
   QCoreApplication::setApplicationName("qTuner");
   QApplication application(argc,argv);
   #ifndef _WIN32
   application.setStyle(QStyleFactory::create("GTK+"));
   #endif // _WIN32
   UIMain ui;
   ui.show();
   return application.exec();
}

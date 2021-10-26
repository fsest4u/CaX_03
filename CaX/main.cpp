#include <QApplication>
//#include <QStyleFactory>

#include "widget/mainwindow.h"
#include "util/caxtranslate.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QCoreApplication::setOrganizationName(TR_ORGANIZATION);

	QApplication a(argc, argv);
//	const auto fusion = QStyleFactory::create( "Fusion" );
//	QApplication::setStyle( fusion );

	MainWindow w;
	w.show();
	return a.exec();
}

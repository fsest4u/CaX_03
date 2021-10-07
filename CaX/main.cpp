#include "widget/mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QCoreApplication::setOrganizationName("NOVATRON");

	QApplication a(argc, argv);
//	const auto fusion = QStyleFactory::create( "Fusion" );
//	QApplication::setStyle( fusion );

	MainWindow w;
	w.show();
	return a.exec();
}

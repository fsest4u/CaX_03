#include <QApplication>
#include <QSharedMemory>
//#include <QStyleFactory>

#include "widget/mainwindow.h"
#include "util/caxtranslate.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QCoreApplication::setOrganizationName(TR_ORGANIZATION);

	QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);

	QApplication a(argc, argv);
//	const auto fusion = QStyleFactory::create( "Fusion" );
//	QApplication::setStyle( fusion );

	MainWindow w;
	QSharedMemory shared(TR_ORGANIZATION);
	if(!shared.create(512, QSharedMemory::ReadWrite))
	{
//		QMessageBox::information(&w,QObject::tr("타이틀"),QObject::tr("메세지"),QMessageBox::Ok);
		exit(0);
	}
	w.show();
	return a.exec();
}

#include <QMovie>
#include <QScreen>

#include "loadingdialog.h"
#include "ui_loadingdialog.h"

LoadingDialog::LoadingDialog(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::LoadingDialog)
{
	ui->setupUi(this);

	Initialize();
}

LoadingDialog::~LoadingDialog()
{
	delete ui;
}

bool LoadingDialog::eventFilter(QObject *object, QEvent *event)
{
	if (object == ui->frame)
	{
		return true;
	}
	return QObject::eventFilter(object, event);
}

void LoadingDialog::Initialize()
{
	QWidget *widget = QApplication::activeWindow();
	if (widget)
	{
		QRect rect = widget->geometry();
		rect.setY(rect.y() - 20);
		rect.setHeight(rect.height() + 20);
		setGeometry(rect);

	//	setAttribute(Qt::WA_TranslucentBackground);
		setWindowFlags(Qt::FramelessWindowHint);
		setWindowOpacity(0.5);

		QMovie *movie = new QMovie(":/resource/loading.gif");
		movie->setScaledSize(QSize(200, 200));
		ui->label->setMovie(movie);
		movie->start();

	}
}

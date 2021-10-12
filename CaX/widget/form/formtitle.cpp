#include <QMouseEvent>

#include "formtitle.h"
#include "ui_formtitle.h"

FormTitle::FormTitle(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormTitle)
{
	ui->setupUi(this);

	ui->labelTitle->installEventFilter(this);
	ui->labelSubtitle->installEventFilter(this);
}

FormTitle::~FormTitle()
{
	delete ui;
}

QString FormTitle::GetTitle()
{
	return ui->labelTitle->text();
}

void FormTitle::SetTitle(QString title)
{
	ui->labelTitle->setText(title);
}

void FormTitle::SetTitleFontSize(int size)
{
	QString style = QString("QLabel{font-size: %1pt;color: rgb(84, 84, 84);}").arg(size);
	ui->labelTitle->setStyleSheet(style);
}

QString FormTitle::GetSubtitle()
{
	return ui->labelSubtitle->text();
}

void FormTitle::SetSubtitle(QString subtitle)
{
	ui->labelSubtitle->setText(subtitle);
}

void FormTitle::SetSubtitleFontSize(int size)
{
	QString style = QString("QLabel{font-size: %1pt;color: rgb(84, 84, 84);}").arg(size);
	ui->labelSubtitle->setStyleSheet(style);
}

bool FormTitle::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelTitle)
		{
			emit SigTitle();
		}
		else if (object == ui->labelSubtitle)
		{
			emit SigSubtitle();
		}
	}

	return QObject::eventFilter(object, event);
}

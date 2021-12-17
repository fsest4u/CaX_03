#include <QMouseEvent>

#include "formtitle.h"
#include "ui_formtitle.h"

#include "util/log.h"

FormTitle::FormTitle(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormTitle)
{
	ui->setupUi(this);

	ui->labelTitle->installEventFilter(this);
	ui->labelSubtitle->installEventFilter(this);

	ui->labelTitle->activateWindow();

	ui->labelTitle->hide();
	ui->labelSubtitle->hide();
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
	if (!title.isEmpty())
	{
		ui->labelTitle->show();
		ui->labelTitle->setText(title);
	}
}

void FormTitle::SetTitleFont(int size, QString color, QString weight)
{
	QString style = QString("QLabel{font-family: \"Segoe UI\";font-size: %1pt;color: #%2;font-weight: %3;}").arg(size).arg(color).arg(weight);
	ui->labelTitle->setStyleSheet(style);
}

QString FormTitle::GetSubtitle()
{
	return ui->labelSubtitle->text();
}

void FormTitle::SetSubtitle(QString subtitle)
{
	if (!subtitle.isEmpty())
	{
		ui->labelSubtitle->show();
		ui->labelSubtitle->setText(subtitle);
	}
}

void FormTitle::SetSubtitleFont(int size, QString color, QString weight)
{
	QString style = QString("QLabel{font-family: \"Segoe UI\";font-size: %1pt;color: #%2;font-weight: %3;}").arg(size).arg(color).arg(weight);
	ui->labelSubtitle->setStyleSheet(style);
}

bool FormTitle::eventFilter(QObject *object, QEvent *event)
{
//	LogDebug("type [%d]", event->type());
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
	else if (event->type() == QMouseEvent::Enter)
	{
		if (object == ui->labelTitle)
		{
		}
		else if (object == ui->labelSubtitle)
		{
		}
	}
	else if (event->type() == QMouseEvent::Leave)
	{
		if (object == ui->labelTitle)
		{
		}
		else if (object == ui->labelSubtitle)
		{
		}
	}

	return QObject::eventFilter(object, event);
}

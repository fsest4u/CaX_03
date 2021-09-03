#include <QMouseEvent>

#include "menuiconeditor.h"
#include "ui_menuiconeditor.h"

#include "util/log.h"

MenuIconEditor::MenuIconEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MenuIconEditor)
{
	ui->setupUi(this);

	m_ID = -1;
	m_CoverArt.clear();

	ui->labelCoverArt->installEventFilter(this);
	ui->labelTitle->installEventFilter(this);
}

MenuIconEditor::~MenuIconEditor()
{
	delete ui;
}

int MenuIconEditor::GetID()
{
	return m_ID;
}

void MenuIconEditor::SetID(int nID)
{
	m_ID = nID;
}

QString MenuIconEditor::GetCoverArt()
{
	return m_CoverArt;
}

void MenuIconEditor::SetCoverArt(const QString &value)
{
	if (value.isEmpty())
		return;

	m_CoverArt = value;

	QImage image;
	if (image.load(m_CoverArt))
	{
//		painter->drawImage(coverRect, image);
		QPixmap pixmap = QPixmap::fromImage(image);
		ui->labelCoverArt->setPixmap(pixmap.scaled(ui->labelCoverArt->width()
												   , ui->labelCoverArt->height()
												   , Qt::KeepAspectRatio));
	}

}

QString MenuIconEditor::GetTitle()
{
	return ui->labelTitle->text();
}

void MenuIconEditor::SetTitle(const QString &value)
{
	ui->labelTitle->setText(value);
}

bool MenuIconEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelCoverArt)
		{
			emit SigClickCoverArt(m_ID);
		}
		else if (object == ui->labelTitle)
		{
			emit SigClickTitle(m_ID);
		}
	}
	return QObject::eventFilter(object, event);
}

#include <QMouseEvent>

#include "iconserviceeditor.h"
#include "ui_iconserviceeditor.h"

#include "util/log.h"

IconServiceEditor::IconServiceEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::IconServiceEditor)
{
	ui->setupUi(this);

	m_ID = -1;
	m_Type = -1;
	m_CoverArt.clear();

	ui->labelCoverArt->installEventFilter(this);
	ui->labelTitle->installEventFilter(this);
}

IconServiceEditor::~IconServiceEditor()
{
	delete ui;
}

int IconServiceEditor::GetID()
{
	return m_ID;
}

void IconServiceEditor::SetID(int nID)
{
	m_ID = nID;
}

int IconServiceEditor::GetType() const
{
	return m_Type;
}

void IconServiceEditor::SetType(int Type)
{
	m_Type = Type;
}

QString IconServiceEditor::GetCoverArt()
{
	return m_CoverArt;
}

void IconServiceEditor::SetCoverArt(const QString &value)
{
	m_CoverArt = value;

	if (m_CoverArt.isEmpty())
	{
		m_CoverArt = ":/resource/Icon-playbar-volume-160.png";
	}

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

QString IconServiceEditor::GetTitle()
{
	return ui->labelTitle->text();
}

void IconServiceEditor::SetTitle(const QString &value)
{
	ui->labelTitle->setText(value);
}

QString IconServiceEditor::GetSubtitle()
{
	return ui->labelSubtitle->text();
}

void IconServiceEditor::SetSubtitle(const QString &value)
{
	ui->labelSubtitle->setText(value);
}

QString IconServiceEditor::GetRawData() const
{
	return m_RawData;
}

void IconServiceEditor::SetRawData(const QString &RawData)
{
	m_RawData = RawData;
}

bool IconServiceEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelCoverArt)
		{
			emit SigClickCoverArt(m_Type, m_RawData);
		}
		else if (object == ui->labelTitle)
		{
			emit SigClickTitle(m_Type);
		}
	}
	return QObject::eventFilter(object, event);
}

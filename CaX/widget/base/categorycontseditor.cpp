#include <QFile>
#include <QMouseEvent>

#include "categorycontseditor.h"
#include "ui_categorycontseditor.h"

#include "util/log.h"

CategoryContsEditor::CategoryContsEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CategoryContsEditor)
{
	ui->setupUi(this);

	m_CoverArt.clear();
	m_ID = -1;

	ui->labelCoverArt->installEventFilter(this);
	ui->labelTitle->installEventFilter(this);
	ui->labelSubTitle->installEventFilter(this);
	ui->labelCount->installEventFilter(this);

}

CategoryContsEditor::~CategoryContsEditor()
{
	delete ui;
}

int CategoryContsEditor::GetID()
{
	return m_ID;
}

void CategoryContsEditor::SetID(int nID)
{
	m_ID = nID;
}

QString CategoryContsEditor::GetCoverArt()
{
	return m_CoverArt;
}

void CategoryContsEditor::SetCoverArt(const QString &value)
{
	m_CoverArt = value;

	bool bFoundImage = false;
	if (QFile::exists(m_CoverArt))
	{
		QImage image;
		if (image.load(m_CoverArt))
		{
//			painter->drawImage(coverRect, image);
			QPixmap pixmap = QPixmap::fromImage(image);
			ui->labelCoverArt->setPixmap(pixmap.scaled(ui->labelCoverArt->width()
													   , ui->labelCoverArt->height()
													   , Qt::KeepAspectRatio));
			bFoundImage = true;
		}
	}

	if (!bFoundImage)
	{
//		painter->drawImage(coverRect, m_Image);
	}
}

QString CategoryContsEditor::GetTitle()
{
	return ui->labelTitle->text();
}

void CategoryContsEditor::SetTitle(const QString &value)
{
	ui->labelTitle->setText(value);
}

QString CategoryContsEditor::GetSubtitle()
{
	return ui->labelSubTitle->text();

}

void CategoryContsEditor::SetSubtitle(const QString &value)
{
	ui->labelSubTitle->setText(value);
}

QString CategoryContsEditor::GetCount()
{
	return ui->labelCount->text();

}

void CategoryContsEditor::SetCount(const QString &value)
{
	ui->labelCount->setText(value);
}

bool CategoryContsEditor::eventFilter(QObject *object, QEvent *event)
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
		else if (object == ui->labelSubTitle)
		{
			emit SigClickSubtitle(m_ID);
		}
		else if (object == ui->labelCount)
		{
			emit SigClickCount(m_ID);
		}
	}
	return QObject::eventFilter(object, event);
}

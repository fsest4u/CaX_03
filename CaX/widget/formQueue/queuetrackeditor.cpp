#include <QMouseEvent>

#include "queuetrackeditor.h"
#include "ui_queuetrackeditor.h"

QueueTrackEditor::QueueTrackEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::QueueTrackEditor)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

QueueTrackEditor::~QueueTrackEditor()
{
	delete ui;
}

int QueueTrackEditor::GetID() const
{
	return m_ID;
}

void QueueTrackEditor::SetID(int ID)
{
	m_ID = ID;
}

QString QueueTrackEditor::GetTitle() const
{
	return ui->labelTitle->text();
}

void QueueTrackEditor::SetTitle(const QString &title)
{
	ui->labelTitle->setText(title);
}

QString QueueTrackEditor::GetTime() const
{
	return ui->labelTime->text();
}

void QueueTrackEditor::SetTime(const QString &time)
{
	ui->labelTime->setText(time);
}

QString QueueTrackEditor::GetArtist() const
{
	return ui->labelArtist->text();
}

void QueueTrackEditor::SetArtist(const QString &artist)
{
	ui->labelArtist->setText(artist);
}

int QueueTrackEditor::GetIndex() const
{
	return m_Index;
}

void QueueTrackEditor::SetIndex(int Index)
{
	m_Index = Index;
}

bool QueueTrackEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (((QMouseEvent*)event)->button() == Qt::LeftButton)
		{
			if (object == ui->labelPlay)
			{
				emit SigClickPlay(m_Index);
			}
			else if (object == ui->labelTitle)
			{
				emit SigClickTitle(m_Index);
			}
			else if (object == ui->labelTime)
			{
				emit SigClickTime(m_Index);
			}
			else if (object == ui->labelArtist)
			{
				emit SigClickArtist(m_Index);
			}
		}
	}

	return QObject::eventFilter(object, event);
}

void QueueTrackEditor::ConnectSigToSlot()
{

}

void QueueTrackEditor::Initialize()
{
	ui->labelPlay->installEventFilter(this);
	ui->labelTitle->installEventFilter(this);
	ui->labelTime->installEventFilter(this);
	ui->labelArtist->installEventFilter(this);

	m_ID = -1;
}

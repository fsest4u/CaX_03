#include <QMouseEvent>

#include "listserviceeditor.h"
#include "ui_listserviceeditor.h"

#include "util/log.h"

#define ICON_DIR		"dir"
#define ICON_SEARCH		"search"
#define ICON_REC_LIST	"rec_list"


ListServiceEditor::ListServiceEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ListServiceEditor)
{
	ui->setupUi(this);

	m_nType = -1;
	m_Icon.clear();
	m_Art.clear();
	m_RawData.clear();

	ui->labelImageIcon->installEventFilter(this);
	ui->labelTitle->installEventFilter(this);

	ui->labelBottom->hide();
	ui->labelDuration->hide();
}

ListServiceEditor::~ListServiceEditor()
{
	delete ui;
}

QString ListServiceEditor::GetID() const
{
	return m_StrID;
}

void ListServiceEditor::SetID(QString strID)
{
	m_StrID = strID;
}

int ListServiceEditor::GetType() const
{
	return m_nType;
}

void ListServiceEditor::SetType(int nType)
{
	m_nType = nType;
}

QString ListServiceEditor::GetTitle() const
{
	return ui->labelTitle->text();
}

void ListServiceEditor::SetTitle(const QString &Title)
{
	ui->labelTitle->setText(Title);
}

QString ListServiceEditor::GetBottom() const
{
	return ui->labelBottom->text();
}

void ListServiceEditor::SetBottom(const QString &Bottom)
{
	ui->labelBottom->show();
	ui->labelBottom->setText(Bottom);
}

QString ListServiceEditor::GetDuration() const
{
	return ui->labelDuration->text();
}

void ListServiceEditor::SetDuration(const QString &Duration)
{
	ui->labelDuration->show();
	ui->labelDuration->setText(Duration);
}

QString ListServiceEditor::GetIcon() const
{
	return m_Icon;
}

void ListServiceEditor::SetIcon(const QString &Icon)
{
	if (Icon.isEmpty() || !m_Art.isEmpty())
		return;

	m_Icon = Icon;

	QString filename = "";
	if (!m_Icon.compare(ICON_SEARCH))
	{
		filename = ":/resource/Icon-playbar-volume-160.png";
	}
	else if (!m_Icon.compare(ICON_REC_LIST))
	{
		filename = ":/resource/Icon-playbar-volume-160.png";
	}
	else
	{
		filename = ":/resource/Icon-playbar-volume-160.png";
	}

	QImage image;
	if (image.load(filename))
	{
//		painter->drawImage(coverRect, image);
		QPixmap pixmap = QPixmap::fromImage(image);
		ui->labelImageIcon->setPixmap(pixmap.scaled(ui->labelImageIcon->width()
												   , ui->labelImageIcon->height()
												   , Qt::KeepAspectRatio));
	}
}

QString ListServiceEditor::GetArt() const
{
	return m_Art;
}

void ListServiceEditor::SetArt(const QString &Art)
{
	m_Art = Art;

	bool bFoundImage = false;
	if (QFile::exists(m_Art))
	{
		QImage image;
		if (image.load(m_Art))
		{
//			painter->drawImage(coverRect, image);
			QPixmap pixmap = QPixmap::fromImage(image);
			ui->labelImageIcon->setPixmap(pixmap.scaled(ui->labelImageIcon->width()
													   , ui->labelImageIcon->height()
													   , Qt::KeepAspectRatio));
			bFoundImage = true;
		}
	}

	if (!bFoundImage)
	{
//		painter->drawImage(coverRect, m_Image);
	}
}

QString ListServiceEditor::GetRawData() const
{
	return m_RawData;
}

void ListServiceEditor::SetRawData(const QString &RawData)
{
	m_RawData = RawData;
}

bool ListServiceEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelImageIcon)
		{
			emit SigClickIcon(m_RawData);
		}
		else if (object == ui->labelTitle)
		{
			emit SigClickTitle(m_RawData);
		}
	}

	return QObject::eventFilter(object, event);
}


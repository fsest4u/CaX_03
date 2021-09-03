#include <QMouseEvent>

#include "menulisteditor.h"
#include "ui_menulisteditor.h"

#include "util/log.h"

#define ICON_DIR		"dir"
#define ICON_SEARCH		"search"
#define ICON_REC_LIST	"rec_list"


MenuListEditor::MenuListEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MenuListEditor)
{
	ui->setupUi(this);

	m_nType = -1;
	m_Icon.clear();
	m_Art.clear();
	m_RawData.clear();

	ui->labelImageIcon->installEventFilter(this);
	ui->labelTitle->installEventFilter(this);
}

MenuListEditor::~MenuListEditor()
{
	delete ui;
}

int MenuListEditor::GetType() const
{
	return m_nType;
}

void MenuListEditor::SetType(int nType)
{
	m_nType = nType;
}

QString MenuListEditor::GetTitle() const
{
	return ui->labelTitle->text();
}

void MenuListEditor::SetTitle(const QString &Title)
{
	ui->labelTitle->setText(Title);
}

QString MenuListEditor::GetIcon() const
{
	return m_Icon;
}

void MenuListEditor::SetIcon(const QString &Icon)
{
	if (Icon.isEmpty() || !m_Art.isEmpty())
		return;

	m_Icon = Icon;

	QString filename = "";
	if (!m_Icon.compare(ICON_SEARCH))
	{
		filename = ":/resource/baseline_search_black_24dp.png";
	}
	else if (!m_Icon.compare(ICON_REC_LIST))
	{
		filename = ":/resource/baseline_search_black_24dp.png";
	}
	else
	{
		filename = ":/resource/baseline_search_black_24dp.png";
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

QString MenuListEditor::GetArt() const
{
	return m_Art;
}

void MenuListEditor::SetArt(const QString &Art)
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

QString MenuListEditor::GetRawData() const
{
	return m_RawData;
}

void MenuListEditor::SetRawData(const QString &RawData)
{
	m_RawData = RawData;
}

bool MenuListEditor::eventFilter(QObject *object, QEvent *event)
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


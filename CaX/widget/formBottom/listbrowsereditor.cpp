#include <QMouseEvent>

#include "listbrowsereditor.h"
#include "ui_listbrowsereditor.h"

#include "util/log.h"

#include "widget/form/formcoverart.h"

#define ICON_DIR		"dir"
#define ICON_SEARCH		"search"
#define ICON_REC_LIST	"rec_list"


ListBrowserEditor::ListBrowserEditor(QWidget *parent) :
	QWidget(parent),
	m_pFormCoverArt(new FormCoverArt(this)),
	ui(new Ui::ListBrowserEditor)
{
	ui->setupUi(this);

	m_nType = -1;
	m_RawData.clear();

	ConnectSigToSlot();
}

ListBrowserEditor::~ListBrowserEditor()
{
	if (m_pFormCoverArt)
	{
		delete m_pFormCoverArt;
		m_pFormCoverArt = nullptr;
	}

	delete ui;
}

QString ListBrowserEditor::GetID() const
{
	return m_StrID;
}

void ListBrowserEditor::SetID(QString strID)
{
	m_StrID = strID;
}

int ListBrowserEditor::GetType() const
{
	return m_nType;
}

void ListBrowserEditor::SetType(int nType)
{
	m_nType = nType;
}

QString ListBrowserEditor::GetTitle() const
{
	return ui->labelTitle->text();
}

void ListBrowserEditor::SetTitle(const QString &title)
{
	ui->labelTitle->setText(title);
}

QString ListBrowserEditor::GetSubtitle() const
{
	return ui->labelSubtitle->text();
}

void ListBrowserEditor::SetSubtitle(const QString &subtitle)
{
	ui->labelSubtitle->show();
	ui->labelSubtitle->setText(subtitle);
}

QString ListBrowserEditor::GetDuration() const
{
	return ui->labelDuration->text();
}

void ListBrowserEditor::SetDuration(const QString &duration)
{
	ui->labelDuration->show();
	ui->labelDuration->setText(duration);
}

QString ListBrowserEditor::GetFilesize() const
{
	return ui->labelFilesize->text();
}

void ListBrowserEditor::SetFileSize(const QString &filesize)
{
	ui->labelFilesize->show();
	ui->labelFilesize->setText(filesize);
}

QString ListBrowserEditor::GetRawData() const
{
	return m_RawData;
}

void ListBrowserEditor::SetRawData(const QString &rawData)
{
	m_RawData = rawData;
}

FormCoverArt *ListBrowserEditor::GetFormCoverArt() const
{
	return m_pFormCoverArt;
}

bool ListBrowserEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelTitle)
		{
			emit SigClickTitle(m_nType, m_RawData);
		}
	}

	return QObject::eventFilter(object, event);
}

void ListBrowserEditor::ConnectSigToSlot()
{
	ui->labelTitle->installEventFilter(this);

	ui->labelSubtitle->hide();
	ui->labelDuration->hide();
	ui->labelFilesize->hide();

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);

}

#include <QMouseEvent>

#include "listserviceeditor.h"
#include "ui_listserviceeditor.h"

#include "widget/form/formcoverart.h"

#include "util/log.h"

#define ICON_DIR		"dir"
#define ICON_SEARCH		"search"
#define ICON_REC_LIST	"rec_list"


ListServiceEditor::ListServiceEditor(QWidget *parent) :
	QWidget(parent),
	m_pFormCoverArt(new FormCoverArt(this)),
	ui(new Ui::ListServiceEditor)
{
	ui->setupUi(this);

	m_nType = -1;
//	m_Icon.clear();
//	m_Art.clear();
	m_RawData.clear();

	ConnectSigToSlot();

}

ListServiceEditor::~ListServiceEditor()
{
	if (m_pFormCoverArt)
	{
		delete m_pFormCoverArt;
		m_pFormCoverArt = nullptr;
	}

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

QString ListServiceEditor::GetSubtitle() const
{
	return ui->labelSubtitle->text();
}

void ListServiceEditor::SetSubtitle(const QString &Bottom)
{
	ui->labelSubtitle->show();
	ui->labelSubtitle->setText(Bottom);
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


QString ListServiceEditor::GetRawData() const
{
	return m_RawData;
}

void ListServiceEditor::SetRawData(const QString &RawData)
{
	m_RawData = RawData;
}

FormCoverArt *ListServiceEditor::GetFormCoverArt() const
{
	return m_pFormCoverArt;
}

bool ListServiceEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelTitle)
		{
			emit SigClickTitle(m_RawData);
		}
	}

	return QObject::eventFilter(object, event);
}

void ListServiceEditor::ConnectSigToSlot()
{
	ui->labelTitle->installEventFilter(this);
	ui->labelSubtitle->hide();
	ui->labelDuration->hide();

	ui->gridLayoutFormCoverArt->addWidget(m_pFormCoverArt);
}


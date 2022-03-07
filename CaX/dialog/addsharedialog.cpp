#include "addsharedialog.h"
#include "ui_addsharedialog.h"

#include "manager/browsermanager.h"

AddShareDialog::AddShareDialog(QWidget *parent) :
	QDialog(parent),
	m_pMgr(new BrowserManager),
	ui(new Ui::AddShareDialog)
{
	ui->setupUi(this);

	ConnectSigToSlot();
	Initialize();
}

AddShareDialog::~AddShareDialog()
{
	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = nullptr;
	}

	delete ui;
}

CJsonNode AddShareDialog::GetNodeInfo()
{
	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_WORK_GROUP, ui->cbWorkGroup->currentText());
	node.Add(KEY_SERVER, ui->cbServer->currentText());
	node.Add(KEY_IP_ADDR, qvariant_cast<QString>(ui->cbServer->currentData()));
	node.Add(KEY_SHARE, ui->cbShare->currentText());
	node.Add(KEY_PATH, ui->labelPath->text());
	node.Add(KEY_USER, ui->lineEditUsername->text());
	node.Add(KEY_PASSWORD, ui->lineEditPassword->text());
	node.Add(KEY_NICKNAME, ui->lineEditNickname->text());

	return node;
}

void AddShareDialog::SetNodeInfo(CJsonNode node)
{
	ui->cbWorkGroup->setCurrentText(node.GetString(KEY_WORK_GROUP));
	ui->cbServer->setCurrentText(node.GetString(KEY_SERVER));
	ui->cbShare->setCurrentText(node.GetString(KEY_SHARE));

	ui->lineEditUsername->setText(node.GetString(KEY_USER));
	ui->lineEditPassword->setText(node.GetString(KEY_PASSWORD));
	ui->lineEditNickname->setText(node.GetString(KEY_NICKNAME));

}

void AddShareDialog::SetInfo(QString addr, int EventID)
{
	m_pMgr->SetAddr(addr);
	m_EventID = EventID;

	m_pMgr->RequestSMBWorkGroup(m_EventID);
}

void AddShareDialog::SlotRespWorkGroup(QStringList list)
{
	ui->cbWorkGroup->clear();

	if (list.count() > 0)
	{
		for (int i = 0; i < list.count(); i++)
		{
			ui->cbWorkGroup->addItem(list.at(i));
		}
	}
}

void AddShareDialog::SlotRespServer(QList<CJsonNode> list)
{
	ui->cbServer->clear();

	foreach (CJsonNode node, list)
	{
		ui->cbServer->addItem(node.GetString(KEY_SERVER), node.GetString(KEY_IP));
	}
}

void AddShareDialog::SlotRespSMBShare(QStringList list)
{
	ui->cbShare->clear();

	if (list.count() > 0)
	{
		for (int i = 0; i < list.count(); i++)
		{
			ui->cbShare->addItem(list.at(i));
		}
	}
}

void AddShareDialog::SlotSelectWorkGroup(const QString &title)
{
	m_pMgr->RequestSMBServer(m_EventID, title);
}

void AddShareDialog::SlotSelectServer(int index)
{
	Q_UNUSED(index)

	QString ipAddr = qvariant_cast<QString>(ui->cbServer->currentData());
	ui->labelPath->setText(ipAddr);

	CJsonNode node(JSON_OBJECT);
	node.Add(KEY_ADD, true);
	node.Add(KEY_WORK_GROUP, ui->cbWorkGroup->currentText());
	node.Add(KEY_SERVER, ui->cbServer->currentText());
	node.Add(KEY_IP_ADDR, ipAddr);
	node.Add(KEY_USER, ui->lineEditUsername->text());
	node.Add(KEY_PASSWORD, ui->lineEditPassword->text());

	m_pMgr->RequestSMBShare(m_EventID, node);

}

void AddShareDialog::SlotSelectShare(int index)
{
	Q_UNUSED(index)

	QString ipAddr = qvariant_cast<QString>(ui->cbServer->currentData());
	QString share = qvariant_cast<QString>(ui->cbShare->currentData());
	ipAddr = ipAddr + "\\" + share;

	ui->labelPath->setText(ipAddr);
}

void AddShareDialog::ConnectSigToSlot()
{
	connect(ui->cbWorkGroup, SIGNAL(currentTextChanged(const QString&)), this, SLOT(SlotSelectWorkGroup(const QString&)));
	connect(ui->cbServer, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotSelectServer(int)));
	connect(ui->cbShare, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotSelectShare(int)));

	connect(m_pMgr, SIGNAL(SigRespSMBWorkGroup(QStringList)), this, SLOT(SlotRespWorkGroup(QStringList)));
	connect(m_pMgr, SIGNAL(SigRespSMBServer(QList<CJsonNode>)), this, SLOT(SlotRespServer(QList<CJsonNode>)));
	connect(m_pMgr, SIGNAL(SigRespSMBShare(QStringList)), this, SLOT(SlotRespSMBShare(QStringList)));

}

void AddShareDialog::Initialize()
{
	m_EventID = -1;
}

#include <QUrl>
#include <QWebEngineProfile>

#include "webengineviewdialog.h"
#include "ui_webengineviewdialog.h"

#include "requestinterceptor.h"

#include "util/log.h"

WebEngineViewDialog::WebEngineViewDialog(QWidget *parent, QString url) :
	QDialog(parent),
	ui(new Ui::WebEngineViewDialog)
{
	ui->setupUi(this);

	m_WebView = new QWebEngineView(this);
	m_WebView->setUrl(QUrl(url));
//	m_WebInterceptor = new RequestInterceptor(m_WebView);
//	m_WebView->page()->profile()->setRequestInterceptor(m_WebInterceptor);
	m_WebView->show();
	m_WebView->resize(this->width(), this->height());

	connect(m_WebView, SIGNAL(loadFinished(bool)), this, SLOT(SlotLoadFinished(bool)));
	connect(this, SIGNAL(html(QString)), this, SLOT(SlotHandleHTML(QString)));

	m_bLogin = false;

}

WebEngineViewDialog::~WebEngineViewDialog()
{
	delete ui;

//	if (m_WebInterceptor)
//	{
//		delete m_WebInterceptor;
//		m_WebInterceptor = nullptr;
//	}

	if (m_WebView)
	{
		delete m_WebView;
		m_WebView = nullptr;
	}

}

void WebEngineViewDialog::SlotLoadFinished(bool ok)
{
	m_WebView->page()->toHtml([this](const QString& result) mutable {emit html(result);});
}

void WebEngineViewDialog::SlotHandleHTML(QString html)
{
	LogDebug("load finished [%s]", html.toUtf8().data());
	if (html.contains("[\"success\""))
	{
		m_bLogin = true;
		accept();
	}

}

void WebEngineViewDialog::resizeEvent(QResizeEvent *)
{
	m_WebView->resize(this->width(), this->height());
}

bool WebEngineViewDialog::GetLogin() const
{
	return m_bLogin;
}

void WebEngineViewDialog::SetLogin(bool bLogin)
{
	m_bLogin = bLogin;
}


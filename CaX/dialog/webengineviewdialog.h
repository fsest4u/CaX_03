#ifndef WEBENGINEVIEWDIALOG_H
#define WEBENGINEVIEWDIALOG_H

#include <QDialog>
#include <QtWebEngineWidgets/QWebEngineView>

namespace Ui {
class WebEngineViewDialog;
}

//class RequestInterceptor;

class WebEngineViewDialog : public QDialog
{
	Q_OBJECT

public:
	explicit WebEngineViewDialog(QWidget *parent = nullptr, QString url = "");
	~WebEngineViewDialog();

	bool GetLogin() const;
	void SetLogin(bool bLogin);

signals:

	void html(QString);

private slots:

	void SlotLoadFinished(bool ok);
	void SlotHandleHTML(QString html);

private:

	void resizeEvent(QResizeEvent *) override;


	QWebEngineView		*m_WebView;
//	RequestInterceptor	*m_WebInterceptor;

	bool				m_bLogin;


	Ui::WebEngineViewDialog *ui;
};

#endif // WEBENGINEVIEWDIALOG_H

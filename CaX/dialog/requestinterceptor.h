#ifndef REQUESTINTERCEPTOR_H
#define REQUESTINTERCEPTOR_H

// no use
// no use
// no use
// no use

//#include <QWebEngineUrlRequestInterceptor>
//#include <QDebug>

//class RequestInterceptor : public QWebEngineUrlRequestInterceptor
//{
//public:
//	explicit RequestInterceptor(QObject * parent = Q_NULLPTR) : QWebEngineUrlRequestInterceptor(parent) {}
//	virtual void interceptRequest(QWebEngineUrlRequestInfo & info) override;

//};

//void RequestInterceptor::interceptRequest(QWebEngineUrlRequestInfo & info)
//{
//	// Intercepting the requested URL
//	QUrl url = info.requestUrl();
//	qDebug() << "Request URL: " << url;

//	// Optionally redirect the request URL but it only works for requests
//	// without payload data such as GET ones
////	info.redirect(QUrl("https://www.google.com"));
//	if (!info.requestUrl().host().contains("qt"))
//	{
//		qDebug() << "blocking" << info.requestUrl();
////		info.block(true);
//	}

//	// Set HTTP header
//	QByteArray httpHeaderName = "SomeHeaderName";
//	QByteArray httpHeaderValue = "SomeHeaderValue";
//	info.setHttpHeader(httpHeaderName, httpHeaderValue);
//}

#endif // REQUESTINTERCEPTOR_H

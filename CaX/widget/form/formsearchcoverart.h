#ifndef FORMSEARCHCOVERART_H
#define FORMSEARCHCOVERART_H

#include <QWidget>

namespace Ui {
class FormSearchCoverArt;
}

class FormSearchCoverArt : public QWidget
{
	Q_OBJECT

public:
	explicit FormSearchCoverArt(QWidget *parent = nullptr);
	~FormSearchCoverArt();

	QByteArray GetCoverArt() const;
	void SetCoverArt(const QByteArray &CoverArt);
//	QString GetCoverArt() const;
//	void SetCoverArt(const QString &CoverArt);

signals:

	void SigCoverArt();

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:

	QByteArray m_CoverArt;
//	QString m_CoverArt;

	Ui::FormSearchCoverArt *ui;
};

#endif // FORMSEARCHCOVERART_H

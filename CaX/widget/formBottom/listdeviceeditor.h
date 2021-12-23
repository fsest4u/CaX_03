#ifndef LISTDEVICEEDITOR_H
#define LISTDEVICEEDITOR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class ListDeviceEditor;
}

class ListDeviceEditor : public QWidget
{
	Q_OBJECT

public:
	explicit ListDeviceEditor(QWidget *parent = nullptr);
	~ListDeviceEditor();

	QString GetMac() const;
	void SetMac(const QString &Mac);

	QString GetAddr() const;
	void SetAddr(const QString &Addr);

	QString GetVal() const;
	void SetVal(const QString &Val);

	QString GetDev() const;
	void SetDev(const QString &Dev);

	void SetLabelCancelShow(bool bShow);

signals:

	void SigClickPlus();
	void SigClickDevice(QString mac, QString addr, QString val, QString dev);
	void SigClickCancel(QString mac, QString addr, QString val, QString dev);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:

	QString m_Mac;
	QString m_Addr;
	QString m_Val;
	QString m_Dev;

	Ui::ListDeviceEditor *ui;
};

#endif // LISTDEVICEEDITOR_H

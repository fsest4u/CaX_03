#ifndef LISTSETUPEDITOR_H
#define LISTSETUPEDITOR_H

#include <QWidget>

namespace Ui {
class ListSetupEditor;
}

class ListSetupEditor : public QWidget
{
	Q_OBJECT

public:
	explicit ListSetupEditor(QWidget *parent = nullptr);
	~ListSetupEditor();

	QString GetStrID() const;
	void SetStrID(const QString &StrID);

	QString GetTitle() const;
	void SetTitle(const QString &title);

signals:

	void SigClickTitle(QString strID);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:

	void ConnectSigToSlot();

	QString		m_StrID;

	Ui::ListSetupEditor *ui;
};

#endif // LISTSETUPEDITOR_H

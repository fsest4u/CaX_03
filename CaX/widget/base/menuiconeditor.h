#ifndef MENUICONEDITOR_H
#define MENUICONEDITOR_H

#include <QWidget>

namespace Ui {
class MenuIconEditor;
}

class MenuIconEditor : public QWidget
{
	Q_OBJECT

public:
	explicit MenuIconEditor(QWidget *parent = nullptr);
	~MenuIconEditor();

	int GetID();
	void SetID(int nID);

	QString GetCoverArt();
	void SetCoverArt(const QString &value);

	QString GetTitle();
	void SetTitle(const QString &value);

signals:

	void SigClickCoverArt(int nID);
	void SigClickTitle(int nID);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:

	int		m_ID;
	QString m_CoverArt;

	Ui::MenuIconEditor *ui;
};

#endif // MENUICONEDITOR_H

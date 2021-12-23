#ifndef LISTSETUPEDITOR_H
#define LISTSETUPEDITOR_H

#include <QWidget>
#include <QMenu>

#include "util/CJsonNode.h"

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

	int GetIndex() const;
	void SetIndex(int Index);

	void ClearMenu();
	void SetMenu(QMap<QString, CJsonNode> map);


signals:

	void SigClickTitle(QString strID, int index);
	void SigMenuAction(QString strID, QString json);
	void SigSubMenuAction(QString value, QString json);

protected:

//	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotClickTitle();
	void SlotMenuAction(QAction* action);
	void SlotSubMenuAction(QAction* action);

private:

	void ConnectSigToSlot();
	void Initialize();

	void AddAction();
	void AddMenu();

	QString		m_StrID;
	int			m_Index;

	QMenu		*m_Menu;

	bool		m_bSubMenu;

	Ui::ListSetupEditor *ui;
};

#endif // LISTSETUPEDITOR_H

#ifndef LISTBROWSERDELEGATE_H
#define LISTBROWSERDELEGATE_H

#include <QStyledItemDelegate>

#include "util/CJsonNode.h"

class ListBrowserDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	ListBrowserDelegate();

	enum {
		LIST_BROWSER_ID = Qt::UserRole + 0,
		LIST_BROWSER_TYPE,
		LIST_BROWSER_COVER,
		LIST_BROWSER_TITLE,
		LIST_BROWSER_SUBTITLE,
		LIST_BROWSER_DURATION,
		LIST_BROWSER_FILESIZE,
		LIST_BROWSER_RAW,
		LIST_BROWSER_INDEX,
		LIST_BROWSER_SELECT,
		LIST_BROWSER_MAX
	};

	QMap<int, QString> GetOptionMenuMap() const;
	void SetOptionMenuMap(const QMap<int, QString> &OptionMenuMap);

	int GetService() const;
	void SetService(int Service);

signals:

	void SigSelectCheck(const QModelIndex &modelIndex);
	void SigSelectPlay(int type, CJsonNode node);
	void SigSelectTitle(int type, CJsonNode node);
	void SigMenu(int index, int type);
	void SigMenu(int index, int type, QString menuName);
	void SigMenuAction(QString path, int type, int menuID);
	void SigMenuAction(CJsonNode node, int type, int menuID);

private slots:

//	void SlotClickCoverArt(int index);
//	void SlotClickPlay(int type, CJsonNode node);
//	void SlotClickTitle(int type, CJsonNode node);
//	void SlotMenu(int index, int type);
//	void SlotMenu(int index, int type, QString menuName);
//	void SlotMenuAction(QString path, int type, int menuID);
//	void SlotMenuAction(CJsonNode node, int type, int menuID);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
//	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
//	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
//	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QMap<int, QString> m_OptionMenuMap;
	int m_Service;

};

#endif // LISTBROWSERDELEGATE_H

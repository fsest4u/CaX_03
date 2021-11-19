#ifndef LISTBROWSERDELEGATE_H
#define LISTBROWSERDELEGATE_H

#include <QStyledItemDelegate>

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
		LIST_BROWSER_SELECT,
		LIST_BROWSER_MAX
	};

	QMap<int, QString> GetOptionMenuMap() const;
	void SetOptionMenuMap(const QMap<int, QString> &OptionMenuMap);

signals:

	void SigSelectTitle(int nType, QString rawData);
	void SigMenu(int index, int nType);
	void SigMenuAction(int index, int menuID);

private slots:

	void SlotClickTitle(int nType, QString rawData);
	void SlotMenu(int index, int nType);
	void SlotMenuAction(int index, int menuID);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QMap<int, QString> m_OptionMenuMap;

};

#endif // LISTBROWSERDELEGATE_H

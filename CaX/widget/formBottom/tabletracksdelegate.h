#ifndef TABLETRACKSDELEGATE_H
#define TABLETRACKSDELEGATE_H

#include <QItemDelegate>
#include <QObject>
#include <QPushButton>
#include <QMenu>


class TableTracksDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	TableTracksDelegate();
	~TableTracksDelegate();


signals:


private slots:


private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

	void ConnectSigToSlot();
	void Initialize();

	QPushButton		*m_BtnSelect;
	QPushButton		*m_BtnCover;
	QPushButton		*m_BtnPlay;
	QPushButton		*m_BtnFavorite;
	QPushButton		*m_BtnMenu;

};

#endif // TABLETRACKSDELEGATE_H

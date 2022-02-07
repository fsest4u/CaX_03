#ifndef EDITTAGDELEGATE_H
#define EDITTAGDELEGATE_H

#include <QItemDelegate>
#include <QObject>
#include <QPushButton>

class EditTagDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	EditTagDelegate();
	~EditTagDelegate();

signals:

	void SigClickFavorite(const QModelIndex &index);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

	QPushButton *m_BtnFavorite;


};

#endif // EDITTAGDELEGATE_H

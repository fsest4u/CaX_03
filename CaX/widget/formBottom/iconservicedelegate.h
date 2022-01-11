#ifndef ICONSERVICEDELEGATE_H
#define ICONSERVICEDELEGATE_H

#include <QStyledItemDelegate>
#include <QListView>

class IconServiceDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	IconServiceDelegate();

	enum {
		ICON_SERVICE_ID = Qt::UserRole + 0,
		ICON_SERVICE_TYPE,
		ICON_SERVICE_COVER,
		ICON_SERVICE_MUTE,
		ICON_SERVICE_TITLE,
		ICON_SERVICE_SUBTITLE,
		ICON_SERVICE_RAW,
		ICON_SERVICE_INDEX,
		ICON_SERVICE_SELECT,
		ICON_SERVICE_MAX
	};

	int GetService() const;
	void SetService(int nService);

signals:

	void SigSelectCoverArt(int index);
	void SigSelectPlay(int index, bool muted);
	void SigSelectTitle(int nType);
	void SigSelectTitle(int nType, QString rawData);

private slots:

	void SlotClickCoverArt(int index);
	void SlotClickPlay(int index, bool muted);
	void SlotClickTitle(int nType, QString rawData);

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	int					m_Service;

};

#endif // ICONSERVICEDELEGATE_H

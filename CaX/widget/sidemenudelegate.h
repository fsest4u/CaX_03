#ifndef SIDEMENUDELEGATE_H
#define SIDEMENUDELEGATE_H

#include <QStyledItemDelegate>

#define SIDE_ROLE_NONE	100
#define SIDE_TYPE_NONE	200

class SideMenuDelegate : public QStyledItemDelegate
{
public:
	SideMenuDelegate();

	enum sidemenuRole {
		SIDE_ROLE_ID = Qt::UserRole + SIDE_ROLE_NONE,
		SIDE_ROLE_ICON,
		SIDE_ROLE_TITLE,
		SIDE_ROLE_MAX
	};

	enum sidemenuType {
		SIDE_MY_MUSIC = Qt::UserRole + SIDE_TYPE_NONE,
		SIDE_AUDIO_CD,
		SIDE_PLAYLIST,
		SIDE_BROWSER,
		SIDE_ISERVICE,
		SIDE_INPUT,
		SIDE_FM_RADIO,
		SIDE_DAB_RADIO,
		SIDE_GROUP_PLAY,
		SIDE_SETUP,
		SIDE_POWER_OFF,
		SIDE_MAX
	};

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // SIDEMENUDELEGATE_H

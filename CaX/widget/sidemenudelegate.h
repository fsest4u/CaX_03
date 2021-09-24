#ifndef SIDEMENUDELEGATE_H
#define SIDEMENUDELEGATE_H

#include <QStyledItemDelegate>

class SideMenuDelegate : public QStyledItemDelegate
{
public:
	SideMenuDelegate();

	enum {
		SIDEMENU_ID = Qt::UserRole + 0,
		SIDEMENU_ICON,
		SIDEMENU_TITLE,
		SIDEMENU_MAX
	};

	enum {
		SIDEMENU_SERVICE_MY_MUSIC = Qt::UserRole + 0,
		SIDEMENU_SERVICE_AUDIO_CD,
		SIDEMENU_SERVICE_PLAYLIST,
		SIDEMENU_SERVICE_BROWSER,
		SIDEMENU_SERVICE_ISERVICE,
		SIDEMENU_SERVICE_INPUT,
		SIDEMENU_SERVICE_FM_RADIO,
		SIDEMENU_SERVICE_DAB_RADIO,
		SIDEMENU_SERVICE_GROUP_PLAY,
		SIDEMENU_SERVICE_SETUP,
		SIDEMENU_SERVICE_POWER_OFF,
		SIDEMENU_SERVICE_MAX
	};

private:

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // SIDEMENUDELEGATE_H

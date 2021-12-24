#ifndef GROUPPLAYDIALOG_H
#define GROUPPLAYDIALOG_H

#include <QDialog>

#include "util/CJsonNode.h"

namespace Ui {
class GroupPlayDialog;
}

class GroupPlayDialog : public QDialog
{
	Q_OBJECT

public:
	explicit GroupPlayDialog(QWidget *parent = nullptr);
	~GroupPlayDialog();

	void SetData(CJsonNode node);
	void SetSelf(bool enable);

signals:

	void SigGroupPlay(QString addr, bool enable);
	void SigAutoJoin(QString addr, bool enable);
	void SigMute(QString addr, bool enable);
	void SigPlayStop(QString addr, bool enable);
	void SigPowerOff(QString addr, bool self);

	void SigVolumeSliderReleased(QString addr, int value);
	void SigChangedChannel(QString addr, int value);

private slots:

	void SlotGroupPlay(bool enable);
	void SlotAutoJoin(bool enable);
	void SlotMute(bool enable);
	void SlotPlayStop(bool enable);
	void SlotPowerOff(bool enable);

	void SlotVolumeSliderReleased();
	void SlotChangedChannel(int value);

private:

	enum {
		GROUP_PLAY_IDLE = 0,
		GROUP_PLAY_CLIENT,
		GROUP_PLAY_SERVER,
		GROUP_PLAY_MAX
	};

	void ConnectSigToSlot();
	void Initialize();

	QString		m_Addr;

	int			m_Status;
	bool		m_Self;
	QString		m_Server;

	Ui::GroupPlayDialog *ui;
};

#endif // GROUPPLAYDIALOG_H

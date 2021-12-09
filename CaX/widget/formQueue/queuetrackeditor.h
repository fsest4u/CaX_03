#ifndef QUEUETRACKEDITOR_H
#define QUEUETRACKEDITOR_H

#include <QWidget>

namespace Ui {
class QueueTrackEditor;
}

class QueueTrackEditor : public QWidget
{
	Q_OBJECT

public:
	explicit QueueTrackEditor(QWidget *parent = nullptr);
	~QueueTrackEditor();

	int GetID() const;
	void SetID(int ID);

	QString GetTitle() const;
	void SetTitle(const QString &title);

	QString GetTime() const;
	void SetTime(const QString &time);

	QString GetArtist() const;
	void SetArtist(const QString &artist);

	int GetIndex() const;
	void SetIndex(int Index);

signals:

	void SigClickPlay(int index);
	void SigClickTitle(int index);
	void SigClickTime(int index);
	void SigClickArtist(int index);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:

	void ConnectSigToSlot();
	void Initialize();

	int		m_ID;
	int		m_Index;

	Ui::QueueTrackEditor *ui;
};

#endif // QUEUETRACKEDITOR_H

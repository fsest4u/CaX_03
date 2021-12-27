#ifndef LISTSERVICEEDITOR_H
#define LISTSERVICEEDITOR_H

#include <QWidget>

class FormCoverArt;

namespace Ui {
class ListServiceEditor;
}

class ListServiceEditor : public QWidget
{
	Q_OBJECT

public:
	explicit ListServiceEditor(QWidget *parent = nullptr);
	~ListServiceEditor();

	QString GetID() const;
	void SetID(QString strID);

	int GetType() const;
	void SetType(int nType);

	QString GetTitle() const;
	void SetTitle(const QString &title);

	QString GetSubtitle() const;
	void SetSubtitle(const QString &subtitle);

	QString GetDuration() const;
	void SetDuration(const QString &duration);

	QString GetRawData() const;
	void SetRawData(const QString &rawData);

	FormCoverArt *GetFormCoverArt() const;

signals:

	void SigClickCoverArt(int index);
	void SigClickTitle(QString rawData);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotCoverArtLeft(int index);
	void SlotCoverArtRight(int index);

private:

	void ConnectSigToSlot();

	FormCoverArt	*m_pFormCoverArt;

	QString		m_StrID;
	int			m_nType;
	QString		m_RawData;

	Ui::ListServiceEditor *ui;
};

#endif // LISTSERVICEEDITOR_H

#ifndef ICONSERVICEEDITOR_H
#define ICONSERVICEEDITOR_H

#include <QWidget>

class FormCoverArt;
class FormTitle;

namespace Ui {
class IconServiceEditor;
}

class IconServiceEditor : public QWidget
{
	Q_OBJECT

public:
	explicit IconServiceEditor(QWidget *parent = nullptr);
	~IconServiceEditor();

	int GetID();
	void SetID(int nID);

	int GetType() const;
	void SetType(int Type);

	QString GetCoverArt();
	void SetCoverArt(const QString &coverArt);

	QString GetTitle();
	void SetTitle(const QString &title);

	QString GetSubtitle();
	void SetSubtitle(const QString &subtitle);

	QString GetRawData() const;
	void SetRawData(const QString &RawData);


signals:

	void SigClickCoverArt(int nType, QString rawData = "");
	void SigClickTitle(int nType);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotCoverArt();
	void SlotTitle();
	void SlotSubtitle();

private:

	void ConnectSigToSlot();

	FormCoverArt	*m_pFormCoverArt;
	FormTitle		*m_pFormTitle;

	int		m_ID;
	int		m_Type;
//	QString m_CoverArt;
	QString m_RawData;

	Ui::IconServiceEditor *ui;
};

#endif // ICONSERVICEEDITOR_H

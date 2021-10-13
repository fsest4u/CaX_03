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

	QString GetRawData() const;
	void SetRawData(const QString &RawData);

	FormCoverArt *GetFormCoverArt() const;
	FormTitle *GetFormTitle() const;


signals:

	void SigClickPlay(int nType);
	void SigClickTitle(int nType, QString rawData = "");


private slots:

	void SlotPlay();
	void SlotTitle();

private:

	void ConnectSigToSlot();

	FormCoverArt	*m_pFormCoverArt;
	FormTitle		*m_pFormTitle;

	int		m_ID;
	int		m_Type;
	QString m_RawData;

	Ui::IconServiceEditor *ui;
};

#endif // ICONSERVICEEDITOR_H

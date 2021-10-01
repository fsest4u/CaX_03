#ifndef ICONSERVICEEDITOR_H
#define ICONSERVICEEDITOR_H

#include <QWidget>

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
	void SetCoverArt(const QString &value);

	QString GetTitle();
	void SetTitle(const QString &value);

	QString GetSubtitle();
	void SetSubtitle(const QString &value);

	QString GetRawData() const;
	void SetRawData(const QString &RawData);


signals:

	void SigClickCoverArt(int nType, QString rawData = "");
	void SigClickTitle(int nType);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:

	int		m_ID;
	int		m_Type;
	QString m_CoverArt;
	QString m_RawData;

	Ui::IconServiceEditor *ui;
};

#endif // ICONSERVICEEDITOR_H

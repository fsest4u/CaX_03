#ifndef CATEGORYCONTSEDITOR_H
#define CATEGORYCONTSEDITOR_H

#include <QWidget>

namespace Ui {
class CategoryContsEditor;
}

class CategoryContsEditor : public QWidget
{
	Q_OBJECT

public:
	explicit CategoryContsEditor(QWidget *parent = nullptr);
	~CategoryContsEditor();

	int GetID();
	void SetID(int nID);

	QString GetCoverArt();
	void SetCoverArt(const QString &value);

	QString GetTitle();
	void SetTitle(const QString &value);

	QString GetSubtitle();
	void SetSubtitle(const QString &value);

	QString GetCount();
	void SetCount(const QString &value);

signals:

	void SigClickCoverArt(int nID);
	void SigClickTitle(int nID);
	void SigClickSubtitle(int nID);
	void SigClickCount(int nID);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:

	int		m_ID;
	QString m_CoverArt;

	Ui::CategoryContsEditor *ui;
};

#endif // CATEGORYCONTSEDITOR_H

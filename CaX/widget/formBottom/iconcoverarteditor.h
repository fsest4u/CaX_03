#ifndef ICONCOVERARTEDITOR_H
#define ICONCOVERARTEDITOR_H

#include <QWidget>

class FormSearchCoverArt;

namespace Ui {
class IconCoverArtEditor;
}

class IconCoverArtEditor : public QWidget
{
	Q_OBJECT

public:
	explicit IconCoverArtEditor(QWidget *parent = nullptr);
	~IconCoverArtEditor();

	FormSearchCoverArt *GetFormSearchCoverArt() const;

	int GetID() const;
	void SetID(int ID);

signals:

	void SigClickCoverArt(int id);

private slots:

	void SlotCoverArt();

private:

	FormSearchCoverArt *m_FormSearchCoverArt;

	int					m_ID;

	Ui::IconCoverArtEditor *ui;
};

#endif // ICONCOVERARTEDITOR_H

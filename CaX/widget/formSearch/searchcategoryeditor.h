#ifndef SEARCHCATEGORYEDITOR_H
#define SEARCHCATEGORYEDITOR_H

#include <QWidget>

class FormCoverArt;
class FormTitle;

namespace Ui {
class SearchCategoryEditor;
}

class SearchCategoryEditor : public QWidget
{
	Q_OBJECT

public:
	explicit SearchCategoryEditor(QWidget *parent = nullptr);
	~SearchCategoryEditor();

	int GetID() const;
	void SetID(int ID);

	FormCoverArt *GetFormCoverArt() const;
	FormTitle *GetFormTitle() const;

signals:

	void SigClickTitle(int id, QString coverArt);

private slots:

	void SlotTitle();

private:

	void ConnectSigToSlot();

	FormCoverArt	*m_pFormCoverArt;
	FormTitle		*m_pFormTitle;

	int m_ID;

	Ui::SearchCategoryEditor *ui;
};

#endif // SEARCHCATEGORYEDITOR_H

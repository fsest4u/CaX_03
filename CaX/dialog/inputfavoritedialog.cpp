#include "inputfavoritedialog.h"
#include "ui_inputfavoritedialog.h"

#include "util/log.h"

InputFavoriteDialog::InputFavoriteDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::InputFavoriteDialog)
{
	ui->setupUi(this);

	connect(ui->btnFavorite, SIGNAL(clicked()), this, SLOT(SlotClickFavorite()));
	SetFavorite(false);
}

InputFavoriteDialog::~InputFavoriteDialog()
{
	delete ui;
}

void InputFavoriteDialog::SlotClickFavorite()
{
	SetFavorite(!m_Favorite);
//	LogDebug("favorite [%d]", m_Favorite);
}

bool InputFavoriteDialog::GetFavorite() const
{
	return m_Favorite;
}

void InputFavoriteDialog::SetFavorite(bool Favorite)
{
	m_Favorite = Favorite;

	QString style;
	if (m_Favorite)
	{
		style = QString("	\
						QPushButton	\
						{	\
							border-image: url(\":/resource/play-btn12-like-u.png\");	\
						}");
	}
	else
	{
		style = QString("	\
						QPushButton	\
						{	\
							border-image: url(\":/resource/play-btn12-like-n.png\");	\
						}");
	}
	ui->btnFavorite->setStyleSheet(style);

}

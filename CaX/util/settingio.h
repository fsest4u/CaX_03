/************************************************************************
**
**  Copyright © 2021 NOVATRON. All right reserved.
**
**  This file is part of CaX
**
**  CaX is desktop application for Cocktail Audio Player.
**
*************************************************************************/

#pragma once

#include <QSettings>
#include <QObject>

class SettingIO : public QSettings
{
	Q_OBJECT
public:
	SettingIO();
};



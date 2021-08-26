/************************************************************************
**
**  Copyright © 2021 NOVATRON. All right reserved.
**
**  This file is part of CaX
**
**  CaX is desktop application for Cocktail Audio Player.
**
*************************************************************************/

#include <QStandardPaths>

#include "settingio.h"

#include "util/log.h"


SettingIO::SettingIO()
	: QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/cax.ini", QSettings::IniFormat)
{
//	LogDebug("[%s]", QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toUtf8().data());
}

/*
 * (c) 2019 Rechenraum GmbH (office@rechenraum.com)
 *
 * This file is part of gpsinfo (www.gpsinfo.org).
 *
 * gpsinfo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gpsinfo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gpsinfo. If not, see <http://www.gnu.org/licenses/>.
 */

//------------------------------------------------------------------------------

#include "gpsinfoMainDialog.h"
#include <QApplication>

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setOrganizationName("Rechenraum");
	QApplication::setOrganizationDomain("rechenraum.com");
	QApplication::setApplicationName("gpsinfo_create");

	gpsinfoMainDialog mainDialog;
	mainDialog.show();

	return a.exec();
}

//------------------------------------------------------------------------------

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

#include <iostream>

#include <QApplication>

#include "gdal.h"

#include "gpsinfoMainDialog.h"

//------------------------------------------------------------------------------

/*
 * gdal_translate "WMTS:file:///tmp/gpsinfo/gpsinfoWMTSCapabilities.xml" ./gpsinfo_wmts.xml -oF WMTS
 * CPL_DEBUG=ON  gdal_translate ./gpsinfo_wmts.xml test.tif
 */
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setOrganizationName("Rechenraum");
	QApplication::setOrganizationDomain("rechenraum.com");
	QApplication::setApplicationName("gpsinfo_create");

	gpsinfoMainDialog mainDialog;
	mainDialog.show();

    std::clog << std::endl << "Welcome to gpsinfo_create, part of gpsinfo (http://gpsinfo.org)!" << std::endl;
    std::clog << "Running with qt " << qVersion() << " and "
              << "gdal " << GDALVersionInfo("RELEASE_NAME") << "." << std::endl
              << std::endl;

	return a.exec();
}

//------------------------------------------------------------------------------

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

#ifndef TILEMATRIXSET_INFO_H
#define TILEMATRIXSET_INFO_H

//------------------------------------------------------------------------------

#include <vector>

#include <QPointF>

//------------------------------------------------------------------------------

/*! \brief Collects all information about the input file necessary for the
 *		XML file.
 *
 */
struct TileMatrixSetInfo
{
	/*! The input file's EPSG code. */
	int m_EPSG;

    /*! Struct describing a single TileMatrix entity. */
    struct TileMatrixInfo
    {
        /*! Size of a pixel in the units of the input file. */
        double m_pixelSize;

        /*! Coordinates of the upper left corner. */
        QPointF m_originUpperLeft;

        /*! Number of tiles in horizontal direction */
        int m_nrTilesX;

        /*! Number of tiles in vertical direction */
        int m_nrTilesY;
    };

    /*! All TileMatrix entities of this TileMatrixSet entity. */
    std::vector< TileMatrixInfo > m_tileMatrixInfos;
};

//------------------------------------------------------------------------------

#endif // TILEMATRIXSET_INFO_H

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

#ifndef GPSINFOMAINDIALOG_H
#define GPSINFOMAINDIALOG_H

//------------------------------------------------------------------------------

#include <QDialog>

#include "TileMatrixSetInfo.h"

//------------------------------------------------------------------------------

class GDALDataset;
class GDALRasterBand;

//------------------------------------------------------------------------------

namespace Ui {
	class gpsinfoMainDialog;
}

//------------------------------------------------------------------------------

class gpsinfoMainDialog : public QDialog
{
	Q_OBJECT

public:

	//--------------------------------------------------------------------------

	explicit gpsinfoMainDialog(QWidget *parent = nullptr);

	~gpsinfoMainDialog();

	//--------------------------------------------------------------------------

private slots:

	//--------------------------------------------------------------------------

	void on_pushButton_quit_clicked();

	void on_pushButton_create_clicked();

	//--------------------------------------------------------------------------

	void on_pushButton_input_clicked();

	void on_pushButton_output_clicked();

	//--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------

    bool writeTiles(TileMatrixSetInfo&);
    bool writeTiles(GDALDataset*,
                    GDALRasterBand*,
                    const int,
                    const int,
                    TileMatrixSetInfo::TileMatrixInfo&);

    bool writeASC(const int,
                  const int,
                  const double,
                  const double,
                  const double,
                  const double,
                  const std::vector< float >&,
                  const QString&,
                  const QString&,
                  const bool);

    QString titleAsDirectory() const;

	//--------------------------------------------------------------------------

    void reportError(const QString&);

    //--------------------------------------------------------------------------

	Ui::gpsinfoMainDialog *ui;

	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
};

//------------------------------------------------------------------------------

#endif // GPSINFOMAINDIALOG_H

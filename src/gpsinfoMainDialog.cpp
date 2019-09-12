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

#include <QFileDialog>
#include <QSettings>
#include <QtXml>

#include "gdal_priv.h"

#include "gpsinfoMainDialog.h"
#include "ui_gpsinfoMainDialog.h"

//------------------------------------------------------------------------------

gpsinfoMainDialog::gpsinfoMainDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::gpsinfoMainDialog)
{
	ui->setupUi(this);

	QSettings settings;
	m_filenameInput = settings.value("m_filenameInput", "").toString();
    ui->lineEdit_input->setText(m_filenameInput);
    m_directoryOutput = settings.value("m_directoryOutput", "").toString();
    ui->lineEdit_output->setText(m_directoryOutput);
    m_compress = settings.value("m_compress", false).toBool();
    ui->checkBox_compression->setChecked(m_compress);

    m_title = settings.value("m_title", "LAYER_NAME").toString();
    ui->lineEdit_title->setText(m_title);

    m_tileSizeX = settings.value("m_tileSizeX", 512).toInt();
    ui->spinBox_x->setValue(m_tileSizeX);
    m_tileSizeY = settings.value("m_tileSizeY", 512).toInt();
    ui->spinBox_y->setValue(m_tileSizeY);
}

//------------------------------------------------------------------------------

gpsinfoMainDialog::~gpsinfoMainDialog()
{
	delete ui;
}

//------------------------------------------------------------------------------

void gpsinfoMainDialog::on_pushButton_quit_clicked()
{
	QApplication::quit();
}

//------------------------------------------------------------------------------

void gpsinfoMainDialog::on_pushButton_input_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select Input File", m_filenameInput);
    if (filename.isNull())
    {
        return;
    }
    m_filenameInput = filename;
    QSettings().setValue("m_filenameInput", m_filenameInput);
    ui->lineEdit_input->setText(m_filenameInput);
}

//------------------------------------------------------------------------------

void gpsinfoMainDialog::on_pushButton_output_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this, "Select Output File", m_directoryOutput);
    if (directory.isNull())
    {
        return;
    }
    m_directoryOutput = directory;
    QSettings().setValue("m_directoryOutput", m_directoryOutput);
    ui->lineEdit_output->setText(m_directoryOutput);
}

//------------------------------------------------------------------------------

void gpsinfoMainDialog::reportError(const QString& message)
{
    std::cerr << "gpsinfo_create reported an error: '"
              << message.toStdString() << "'." << std::endl;
}

//------------------------------------------------------------------------------

void gpsinfoMainDialog::on_pushButton_create_clicked()
{
	ui->progressBar->setEnabled(true);

    m_compress = ui->checkBox_compression->isChecked();
    QSettings().setValue("m_compress", m_compress);

    m_title = ui->lineEdit_title->text();
    QSettings().setValue("m_title", m_title);

    m_tileSizeX = (unsigned int) ui->spinBox_x->value();
    QSettings().setValue("m_tileSizeX", m_tileSizeX);
    m_tileSizeY = (unsigned int) ui->spinBox_y->value();
    QSettings().setValue("m_tileSizeY", m_tileSizeY);

	if (!createTiles())
	{
		return;
	}

	return;


    const QString& description = ui->lineEdit_description->text();
    const QString& copyright = ui->lineEdit_copyright->text();
    const QString& url = ui->lineEdit_URL->text();

    const QString& name = ui->lineEdit_name->text();
    const QString& city = ui->lineEdit_city->text();
    const QString& country = ui->lineEdit_country->text();
    const QString& email = ui->lineEdit_email->text();


    QFile file(m_directoryOutput + "/gpsinfoWMTSCapabilities.xml");
    if (!file.open(QIODevice::WriteOnly| QIODevice::Text))
    {
        reportError("Failed to open " + file.fileName() + " for writing.");
        return;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("Capabilities");
    xml.writeAttribute("xmlns", "http://www.opengis.net/wmts/1.0");
    xml.writeAttribute("xmlns:ows", "http://www.opengis.net/ows/1.1");
    xml.writeAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
    xml.writeAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    xml.writeAttribute("xsi:schemaLocation", "http://www.opengis.net/wmts/1.0 http://schemas.opengis.net/wmts/1.0/wmtsGetCapabilities_response.xsd");
    xml.writeAttribute("version", "1.0.0");

    /*
     * ows:ServiceIdentification
     */

    xml.writeStartElement("ows:ServiceIdentification");
    xml.writeTextElement("ows:Title", m_title);
    xml.writeEndElement();

    /*
     * ows:ServiceProvider
     */

    xml.writeStartElement("ows:ServiceIdentification");
    xml.writeTextElement("ows:Title", m_title);
    xml.writeEndElement();

    /*
     * Contents - Layer
     */

    xml.writeStartElement("Contents");

    xml.writeStartElement("Layer");
    xml.writeTextElement("ows:Title", "Layer for " + m_title);
    xml.writeEndElement();

    /*
     * Contents - TileMatrixSet
     */

    xml.writeStartElement("TileMatrixSet");
    xml.writeTextElement("ows:Title", "Tiles for " + m_title);
    xml.writeEndElement();

    xml.writeEndElement();

    /*
     * ServiceMetadataURL
     */

    xml.writeStartElement("ServiceMetadataURL");
    xml.writeAttribute("xlink:href", url);
    xml.writeEndElement();

    xml.writeEndElement();
    xml.writeEndDocument();

    file.close();

	ui->progressBar->setEnabled(false);
}

//------------------------------------------------------------------------------

/*! \brief Split the input file into tiles
 *
 * \return True on success, false otherwise.
 */
bool gpsinfoMainDialog::createTiles()
{
	/*
	 * Open input file
	 */

	GDALAllRegister();

	GDALDataset *dataset = (GDALDataset *) GDALOpen(ui->lineEdit_input->text().toStdString().c_str(), GA_ReadOnly);
	if (!dataset)
	{
		reportError("Failed to open '" + ui->lineEdit_input->text() + "'.");
		return false;
	}

	if (dataset->GetRasterCount() < 1)
	{
		reportError("We require at least a single raster band in '" + ui->lineEdit_input->text() + "'.");
		GDALClose(dataset);
		return false;
	}

	GDALRasterBand *rasterBand = dataset->GetRasterBand(1);
	if (!rasterBand)
	{
		reportError("We failed to retrieve the first raster band in '" + ui->lineEdit_input->text() + "'.");
		GDALClose(dataset);
		return false;
	}

	/*
	 * Process input file
	 */

	const int nrXTotal = dataset->GetRasterXSize();
	const int nrYTotal = dataset->GetRasterYSize();
	double geoTransform[6];
	dataset->GetGeoTransform(geoTransform);
    /* The coordinates of the upper left corner of the upper left pixel (0,0) */
    const QPointF origin(geoTransform[0], geoTransform[1]);
	const QPointF pixelSize(geoTransform[1], geoTransform[5]);
    if (fabs(fabs(pixelSize.x()) - fabs(pixelSize.y())) > 1e-8)
    {
        reportError("We support raster data sets with square pixels only.");
        GDALClose(dataset);
        return false;
    }

	int hasNoDataValue;
    double noDataValue = rasterBand->GetNoDataValue(&hasNoDataValue);
	if (!hasNoDataValue)
	{
		noDataValue = -9999;
	}

    const int nrXTile = m_tileSizeX;
    const int nrYTile = m_tileSizeY;

	const int nrTilesX = ceil(((float) nrXTotal) / nrXTile);
	const int nrTilesY = ceil(((float) nrYTotal) / nrYTile);

	std::clog << "Importing " << nrXTotal << " x " << nrYTotal << " raster points "
			  << "into " << nrTilesX << " x " << nrTilesY << " tiles, "
			  << "each raster point of " << pixelSize.x() << " x " << pixelSize.y() << " pixel size with a "
			  << "no data value of '" << noDataValue << "'."
			  << std::endl;

	ui->progressBar->setRange(0, nrTilesX*nrTilesY-1);
	ui->progressBar->setValue(0);

	std::vector< float > data(nrXTile*nrYTile);
	for ( int col=0 ; col<nrTilesX ; ++col )
	{
        QString pathCol = QString(ui->lineEdit_output->text() + "/" + ui->lineEdit_title->text() + "/%1").arg(col);
        QDir().mkpath(pathCol);
        const double xllCorner = origin.x() + col*nrXTile*pixelSize.x();

		for ( int row=0 ; row<nrTilesY ; ++row )
		{
			ui->progressBar->setValue(col*nrTilesY + row);
			QApplication::processEvents();

            std::fill(data.begin(), data.end(), noDataValue);

			int sizeX = std::min(nrXTile, nrXTotal - (col+1)*nrXTile);
			int sizeY = std::min(nrYTile, nrYTotal - (row+1)*nrYTile);

            /* Stores data "in left to right, top to bottom pixel order". */
			CPLErr err = rasterBand->RasterIO(GF_Read,
											  col*nrXTile,
											  row*nrYTile,
											  sizeX,
											  sizeY,
											  data.data(),
                                              sizeX,
                                              sizeY,
											  GDT_Float32,
											  0,
                                              sizeof(float)*nrXTile,
											  nullptr);
			if (err != CE_None)
			{
				reportError("We failed to read data from '" + ui->lineEdit_input->text() + "'.");
				GDALClose(dataset);
				return false;
			}

            const double yllCorner = origin.y() + (row+1)*nrYTile*pixelSize.y();
            const QString filenameASC = QString(pathCol + "/%1.asc").arg(row);
            if (!writeASC(nrXTile,
                          nrYTile,
                          xllCorner,
                          yllCorner,
                          fabs(pixelSize.x()),
                          noDataValue,
                          data,
                          filenameASC,
                          ui->checkBox_compression->isChecked()))
            {
                reportError("We failed to write tile '" + filenameASC + "'.");
                GDALClose(dataset);
                return false;
            }
		}
	}


	/*
	 * Epilogue
	 */

	GDALClose(dataset);

	return true;
}

//------------------------------------------------------------------------------

bool gpsinfoMainDialog::writeASC(
        const int nrXTile,
        const int nrYTile,
        const double xllCorner,
        const double yllCorner,
        const double cellSize,
        const double noDataValue,
        const std::vector< float >& data,
        const QString& filenameASC,
        const bool compression)
{
    if (compression)
    {
        reportError("We do not yet support compression.");
        return false;
    }

    QString asc;
    QTextStream ascStream(&asc);
    ascStream << "NCOLS " << nrXTile << "\n"
              << "NROWS " << nrYTile << "\n"
              << "XLLCORNER " << xllCorner << "\n"
              << "YLLCORNER " << yllCorner << "\n"
              << "CELLSIZE " << cellSize << "\n"
              << "NODATA_VALUE " << noDataValue << "\n";

    for ( int row=0 ; row<nrYTile ; ++row )
    {
        for ( int col=0 ; col<nrXTile ; ++col )
        {
            ascStream << data[row*nrXTile + col] << " ";
        }
        ascStream << "\n";
    }

    QSaveFile file(compression ? filenameASC + ".zip" : filenameASC);
    if (!file.open(QIODevice::WriteOnly))
    {
        return false;
    }
    file.write(asc.toUtf8());
    file.commit();

    return true;
}

//------------------------------------------------------------------------------

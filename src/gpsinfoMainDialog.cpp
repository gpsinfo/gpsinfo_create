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

#include "ogr_spatialref.h"
#include "gdal_priv.h"

#include "gpsinfoMainDialog.h"
#include "ui_gpsinfoMainDialog.h"

//------------------------------------------------------------------------------

/*! \brief Constructor
 *
 * We realize the UI as QDialog.
 *
 * \param parent Parent widget
 */
gpsinfoMainDialog::gpsinfoMainDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::gpsinfoMainDialog)
{
	ui->setupUi(this);

    QSettings settings;
    ui->lineEdit_input->setText(settings.value("filenameInput", "").toString());
    ui->lineEdit_output->setText(settings.value("directoryOutput", "").toString());
    ui->combo_tileFormat->setCurrentIndex(settings.value("tileFormatIndex", 0).toInt());
    ui->checkbox_createOverviews->setChecked(settings.value("createOverviews", false).toBool());

    ui->lineEdit_title->setText(settings.value("title", "").toString());
    ui->lineEdit_description->setText(settings.value("description", "").toString());
    ui->lineEdit_copyright->setText(settings.value("copyright", "").toString());
    ui->lineEdit_URL->setText(settings.value("url", "").toString());

    ui->lineEdit_name->setText(settings.value("name", "").toString());
    ui->lineEdit_city->setText(settings.value("city", "").toString());
    ui->lineEdit_country->setText(settings.value("country", "").toString());
    ui->lineEdit_email->setText(settings.value("email", "").toString());

    ui->spinBox_x->setValue(settings.value("tileSizeX", 512).toInt());
    ui->spinBox_y->setValue(settings.value("tileSizeY", 512).toInt());
}

//------------------------------------------------------------------------------

/*! \brief Destructor
 */
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
    QSettings settings;
    QString filename = QFileDialog::getOpenFileName(this, "Select Input File", settings.value("filenameInput").toString());
    if (filename.isNull())
    {
        return;
    }
    settings.setValue("filenameInput", filename);
    ui->lineEdit_input->setText(filename);
}

//------------------------------------------------------------------------------

void gpsinfoMainDialog::on_pushButton_output_clicked()
{
    QSettings settings;
    QString directory = QFileDialog::getExistingDirectory(this, "Select Output Directory", settings.value("directoryOutput").toString());
    if (directory.isNull())
    {
        return;
    }
    settings.setValue("directoryOutput", directory);
    ui->lineEdit_output->setText(directory);
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

    /*
     * Write tiles
     */

	TileMatrixSetInfo tmsInfo;
    if (!writeTiles(tmsInfo))
    {
        return;
    }

    /*
     * Settings
     */

    QSettings settings;
	settings.setValue("filenameInput", ui->lineEdit_input->text());
	settings.setValue("directoryOutput", ui->lineEdit_output->text());
    settings.setValue("tileFormatIndex", ui->combo_tileFormat->currentIndex());
    settings.setValue("createOverviews", ui->checkbox_createOverviews->isChecked());

    settings.setValue("title", ui->lineEdit_title->text());
    settings.setValue("description", ui->lineEdit_description->text());
    settings.setValue("copyright", ui->lineEdit_copyright->text());
    settings.setValue("url", ui->lineEdit_URL->text());

    settings.setValue("name", ui->lineEdit_name->text());
    settings.setValue("city", ui->lineEdit_city->text());
    settings.setValue("country", ui->lineEdit_country->text());
    settings.setValue("email", ui->lineEdit_email->text());

    settings.setValue("tileSizeX", ui->spinBox_x->value());
    settings.setValue("tileSizeY", ui->spinBox_y->value());

    /*
     * XML file
     */

    QFile file(ui->lineEdit_output->text() + "/gpsinfoWMTSCapabilities.xml");
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
        xml.writeTextElement("ows:Title", ui->lineEdit_title->text());
        xml.writeStartElement("ows:Keywords");
            xml.writeTextElement("ows:Keyword", "gpsinfo");
        xml.writeEndElement();
        xml.writeTextElement("ows:ServiceType", "OGC WMTS");
        xml.writeTextElement("ows:ServiceTypeVersion", "2.0");
        xml.writeTextElement("ows:Fees", "none");
        xml.writeTextElement("ows:AccessConstraints", "https://creativecommons.org/licenses/by/4.0/");
    xml.writeEndElement();

    /*
     * ows:ServiceProvider
     */

    xml.writeStartElement("ows:ServiceProvider");
        xml.writeTextElement("ows:ProviderName", "Rechenraum GmbH");
        xml.writeStartElement("ows:ProviderSite");
            xml.writeAttribute("xlink:href", "https://www.rechenraum.com");
        xml.writeEndElement();
        xml.writeStartElement("ows:ServiceContact");
            xml.writeStartElement("ows:ContactInfo");
                xml.writeStartElement("ows:Address");
                    xml.writeTextElement("ows:City", "Vienna");
                    xml.writeTextElement("ows:Country", "Austria");
                    xml.writeTextElement("ows:ElectronicMailAddress", "office@rechenraum.com");
                xml.writeEndElement();
            xml.writeEndElement();
        xml.writeEndElement();
    xml.writeEndElement();

    /*
     * Contents
     */

    xml.writeStartElement("Contents");

    /*
     * Contents - Layer
     */

        xml.writeStartElement("Layer");
            xml.writeTextElement("ows:Title", ui->lineEdit_title->text());
            xml.writeTextElement("ows:Abstract", ui->lineEdit_description->text());
            xml.writeTextElement("ows:Identifier", ui->lineEdit_title->text());
			/* The Style element is mandatory, but useless for us. */
            xml.writeStartElement("Style");
                xml.writeAttribute("isDefault", "true");
            xml.writeEndElement();
			/* Let's use text/plain as mime-type for the ESRI ASCII Grid file
			 * format. For the compressed versions, we are going to use
			 * application/zip
			 */
            QString format, filenameEnding;
            switch (ui->combo_tileFormat->currentIndex())
            {
            case 0:
                /* https://github.com/opengeospatial/geotiff/issues/34 (Jun 19, 2019) */
                format = "image/tiff; application=geotiff";
                filenameEnding = ".tif";
                break;
            case 1:
                format = "text/plain";
                filenameEnding = ".asc";
                break;
            case 2:
                format = "application/zip";
                filenameEnding = ".asc.zip";
                break;
            default:
                reportError("Unknown or unsupported file format.");
                return;
            }

            xml.writeTextElement("Format", format);
			/* This must be a valid link to a TileMatrixSet defined below */
			xml.writeStartElement("TileMatrixSetLink");
                xml.writeTextElement("TileMatrixSet", ui->lineEdit_title->text());
            xml.writeEndElement();
            xml.writeStartElement("ResourceURL");
                xml.writeAttribute("format", format);
                xml.writeAttribute("resourceType", "tile");
                xml.writeAttribute("template", ui->lineEdit_URL->text() + "/" + titleAsDirectory() + "/{TileMatrix}/{TileCol}/{TileRow}" + filenameEnding);
            xml.writeEndElement();
        xml.writeEndElement();

    /*
     * Contents - TileMatrixSet
     */

        xml.writeStartElement("TileMatrixSet");
			xml.writeTextElement("ows:Title", ui->lineEdit_title->text());
			/* This is referenced from above */
			xml.writeTextElement("ows:Identifier", ui->lineEdit_title->text());
            std::cerr << "Writing hard-coded EPSG" << std::endl;
//			xml.writeTextElement("ows:SupportedCRS", QString("urn:ogc:def:crs:EPSG::%1").arg(tmsInfo.m_EPSG));
            xml.writeTextElement("ows:SupportedCRS", "urn:ogc:def:crs:EPSG::31287");
            for ( int i=0 ; i<static_cast< int >(tmsInfo.m_tileMatrixInfos.size()) ; ++i )
            {
                const auto& tmInfo = tmsInfo.m_tileMatrixInfos[i];
                xml.writeStartElement("TileMatrix");
                    xml.writeTextElement("ows:Identifier", "0");
                    /* See https://gis.stackexchange.com/a/315989:
                     *			CELLSIZE in meters = ScaleDenominator * 0.00028 * (factor CRS units to m)
                     * So a CELLSIZE of 10 in 31287 corresponds to
                     * 			ScaleDenominator = 10 / 0.00028 = 35714.285714
                     */
                    xml.writeTextElement("ScaleDenominator", QString("%1").arg(tmInfo.m_pixelSize / 0.00028, 0, 'e', 10));
                    xml.writeTextElement("TopLeftCorner", QString("%1 %2").arg(tmInfo.m_originUpperLeft.x(), 0, 'e', 10).arg(tmInfo.m_originUpperLeft.y(), 0, 'e', 10));
                    /* Each tile has this many columns, e.g. the asc file's NCOLS */
                    xml.writeTextElement("TileWidth", QString("%1").arg(ui->spinBox_x->value()));
                    /* Each tile has this many rows, e.g. the asc file's NROWS */
                    xml.writeTextElement("TileHeight", QString("%1").arg(ui->spinBox_y->value()));
                    /* We store MatrixHeight x MatrixWidth tiles, e.g. there are
                     * MatrixWidth subdirectories.
                     */
                    xml.writeTextElement("MatrixWidth", QString("%1").arg(tmInfo.m_nrTilesX));
                    /* Each subdirectory stores this many asc files. */
                    xml.writeTextElement("MatrixHeight", QString("%1").arg(tmInfo.m_nrTilesY));
                xml.writeEndElement();
            }
        xml.writeEndElement();

    xml.writeEndElement();

    /*
     * ServiceMetadataURL
     */

    xml.writeStartElement("ServiceMetadataURL");
        xml.writeAttribute("xlink:href", ui->lineEdit_URL->text());
    xml.writeEndElement();

    xml.writeEndElement();
    xml.writeEndDocument();

    file.close();

	ui->progressBar->setEnabled(false);
}

//------------------------------------------------------------------------------

/*! \brief Split the input file into tiles
 *
 * \param info (out) Information about the input file necessary for writing the
 *		XML file.
 *
 * \return True on success, false otherwise.
 */
bool gpsinfoMainDialog::writeTiles(TileMatrixSetInfo& info)
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

	OGRSpatialReference spatialReference(dataset->GetProjectionRef());
	info.m_EPSG = QString(spatialReference.GetAttrValue("AUTHORITY", 1)).toInt();

	/*
	 * Process input file
	 */

	const int nrXTotal = dataset->GetRasterXSize();
	const int nrYTotal = dataset->GetRasterYSize();
    const int nrXTile = ui->spinBox_x->value();
    const int nrYTile = ui->spinBox_y->value();

    /*
     * Build overviews (e.g. zoom levels)
     */

    /* The progress bar will not update as the application blocks. */
    ui->progressBar->setFormat(QString("Building Overviews ..."));
    ui->progressBar->setRange(0, 0);
    ui->progressBar->setValue(0);

    std::clog << "Build overviews (this may take a while, and no progress is shown) ... " << std::flush;

    /* At the coarsest zoom level, the data shall fit into a single tile. Hence
     * we solve for
     *      nrXTotal / 2^k <  nrXTile
     */
    const int maxZoomLevelX = ceil(log2(nrXTotal / nrXTile));
    const int maxZoomLevelY = ceil(log2(nrYTotal / nrYTile));
    const int maxZoomLevel = std::max(maxZoomLevelX, maxZoomLevelY);
    std::vector< int > decimationFactors(maxZoomLevel-1);
    for ( int i=0 ; i<static_cast< int >(decimationFactors.size()) ; ++i )
    {
        decimationFactors[i] = 1<<(i+1);
    }
    dataset->BuildOverviews("NEAREST",
                            decimationFactors.size(),
                            decimationFactors.data(),
                            0,
                            nullptr,
                            nullptr,
                            nullptr);

    std::clog << "DONE" << std::endl;

    info.m_tileMatrixInfos.resize(maxZoomLevel);
    for (int i=0 ; i<maxZoomLevel-1 ; ++i )
    {
        /* Overviews are sorted in decreasing size */
        writeTiles(dataset, rasterBand->GetOverview(maxZoomLevel-i-1), i, maxZoomLevel, info.m_tileMatrixInfos[i]);
        if (i == 2)
        {
            break;
        }
    }
//    writeTiles(dataset, rasterBand, maxZoomLevel, maxZoomLevel, info.m_tileMatrixInfos.back());


    /*
	 * Epilogue
	 */

	GDALClose(dataset);
    GDALDestroyDriverManager();

    /* This is necessary for Austria's OGD 10m. */
    if (spatialReference.GetAxisMappingStrategy() == OAMS_AUTHORITY_COMPLIANT)
    {
        for ( auto& tmInfo : info.m_tileMatrixInfos )
        {
            tmInfo.m_originUpperLeft = QPointF(tmInfo.m_originUpperLeft.y(), tmInfo.m_originUpperLeft.x());
        }
    }

	return true;
}

//------------------------------------------------------------------------------

/*! \brief Export a single zoom level
 *
 */
bool gpsinfoMainDialog::writeTiles(GDALDataset* dataset,
                                   GDALRasterBand* rasterBand,
                                   const int xmlZoomLevel,
                                   const int maxZoomLevel,
                                   TileMatrixSetInfo::TileMatrixInfo& info)
{
    if (!rasterBand)
    {
        reportError("Internal Error");
        return false;
    }

    /* Number of pixels in the current raster band */
    const int nrXTotal = rasterBand->GetXSize();
    const int nrYTotal = rasterBand->GetYSize();
    /* Number of pixels per tile */
    const int nrXTile = ui->spinBox_x->value();
    const int nrYTile = ui->spinBox_y->value();

    /*
     * Transformation info
     */

    std::vector< double > geoTransform(6);
    dataset->GetGeoTransform(geoTransform.data());
    /* The coordinates of the upper left corner of the upper left pixel (0,0) */
    info.m_originUpperLeft = QPointF(geoTransform[0], geoTransform[3]);
    auto& pixelSizeX = geoTransform[1];
    pixelSizeX *= (1 << (maxZoomLevel-xmlZoomLevel));
    auto& pixelSizeY = geoTransform[5];
    pixelSizeY *= (1 << (maxZoomLevel-xmlZoomLevel));
    if (fabs(fabs(pixelSizeX) - fabs(pixelSizeY)) > 1e-8)
    {
        reportError("We support raster data sets with square pixels only.");
        GDALClose(dataset);
        return false;
    }
    info.m_pixelSize = fabs(pixelSizeX);

    int hasNoDataValue;
    double noDataValue = rasterBand->GetNoDataValue(&hasNoDataValue);
    if (!hasNoDataValue)
    {
        noDataValue = -9999;
    }

    /* Number of tiles for this raster band. */
    info.m_nrTilesX = ceil(((float) nrXTotal) / nrXTile);
    info.m_nrTilesY = ceil(((float) nrYTotal) / nrYTile);

    std::clog << "Writing zoom level #" << xmlZoomLevel << ": " << std::endl
              << "\torigin = (" << info.m_originUpperLeft.x() << ", " << info.m_originUpperLeft.y() << ")." << std::endl
              << "\t" << nrXTotal << " x " << nrYTotal << " raster points" << std::endl
              << "\t" << info.m_nrTilesX << " x " << info.m_nrTilesY << " tiles" << std::endl
              << "\t" << pixelSizeX << " x " << pixelSizeY<< " pixel size" << std::endl
              << "\tno data value = '" << noDataValue << "'. " << std::endl
              << std::endl;

    ui->progressBar->setFormat(QString("Writing zoom level %1 ... %p%").arg(xmlZoomLevel));
    ui->progressBar->setRange(0, info.m_nrTilesX*info.m_nrTilesY-1);
    ui->progressBar->setValue(0);

    /*
     * Write the data
     */

    std::vector< double > geoTransformC = geoTransform;
    std::vector< float > data(nrXTile*nrYTile);
    /* cols run from left to right */
    for ( int col=0 ; col<info.m_nrTilesX ; ++col )
    {
        QString pathCol = QString(ui->lineEdit_output->text() + "/" + titleAsDirectory() + "/%1/%2").arg(xmlZoomLevel).arg(col);
        QDir().mkpath(pathCol);
        const double xLeft = info.m_originUpperLeft.x() + col*nrXTile*fabs(pixelSizeX);

        /* rows run from top to bottom */
        for ( int row=0 ; row<info.m_nrTilesY ; ++row )
        {
            ui->progressBar->setValue(col*info.m_nrTilesY + row);
            QApplication::processEvents();

            std::fill(data.begin(), data.end(), noDataValue);

            /* Clip reading size to available extent of data */
            int sizeX = std::min(nrXTile, nrXTotal - col*nrXTile);
            int sizeY = std::min(nrYTile, nrYTotal - row*nrYTile);

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

            /* Write to ASC file. */
            if (ui->combo_tileFormat->currentIndex() == 0)
            {

                /* See
                 *      https://gdal.org/drivers/raster/gtiff.html
                 * and
                 *      https://gerasimosmichalitsianos.wordpress.com/2018/01/08/178/
                 * Regarding compression,
                 *      https://trac.osgeo.org/gdal/wiki/CloudOptimizedGeoTIFF
                 * recommends LZW for greatest compatibility
                 */
                const QString filename = QString("%1/%2.tif").arg(pathCol).arg(row);
                char **papszOptions = nullptr;
                papszOptions = CSLSetNameValue(papszOptions, "COMPRESS", "LZW");
                GDALDataset* datasetOut = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
                            filename.toStdString().c_str(),
                            nrXTile,
                            nrYTile,
                            1,
                            GDT_Float32,
                            papszOptions);
                /* The coordinates of the upper left corner of the upper left pixel (0,0) */
                geoTransformC[0] = xLeft;
                geoTransformC[3] = info.m_originUpperLeft.y() - row*nrYTile*fabs(pixelSizeY);
                datasetOut->SetGeoTransform(geoTransformC.data());
                datasetOut->SetProjection(dataset->GetProjectionRef());
                datasetOut->GetRasterBand(1)->SetNoDataValue(noDataValue);
                if (datasetOut->GetRasterBand(1)->RasterIO(GF_Write,
                                                        0,
                                                        0,
                                                        nrXTile,
                                                        nrYTile,
                                                        data.data(),
                                                        nrXTile,
                                                        nrYTile,
                                                        GDT_Float32,
                                                        0,
                                                        0) != CE_None)
                {
                    reportError("We failed to write tile '" + filename + "'.");
                    GDALClose(datasetOut);
                    GDALClose(dataset);
                    return false;
                }
                GDALClose(datasetOut);
            }
            else if ((ui->combo_tileFormat->currentIndex() == 1) ||
                     (ui->combo_tileFormat->currentIndex() == 2))
            {
                const QString filename = QString("%1.asc").arg(row);
                if (!writeASC(nrXTile,
                              nrYTile,
                              xLeft,
                              info.m_originUpperLeft.y() - (row+1)*nrYTile*fabs(pixelSizeY),
                              fabs(pixelSizeX),
                              noDataValue,
                              data,
                              pathCol,
                              filename,
                              ui->combo_tileFormat->currentIndex() == 2))
                {
                    reportError("We failed to write tile '" + filename + "'.");
                    GDALClose(dataset);
                    return false;
                }
            }
            else
            {
                reportError("Unknown or unsupported tile format.");
                GDALClose(dataset);
                return false;
            }
        }
    }

    return true;
}

//------------------------------------------------------------------------------

/*! \brief Stores the given data in ASC file format, possibly ZIP compressed
 *
 * \param nrXTile Horizontal raster size, e.g. the number of columns
 * \param nrYTile Vertical raster size, e.g. the number of rows
 * \param xllCorner Horizontal coordinate of the tile's lower left corner
 * \param yllCorner Vertical coordinate of the tile's lower left corner
 * \param cellSize Size of a (square) raster pixel
 * \param noDataValue No data value
 * \param data The raster tile's data, in left to right, top to bottom pixel
 *      order
 * \param pathCol Path to write file to
 * \param filename Name of file to export to
 * \param compression If true, the ASC file is ZIP compressed.
 *
 * \return True on success, false otherwise.
 */
bool gpsinfoMainDialog::writeASC(
        const int nrXTile,
        const int nrYTile,
        const double xllCorner,
        const double yllCorner,
        const double cellSize,
        const double noDataValue,
        const std::vector< float >& data,
        const QString& pathCol,
        const QString& filenameASC,
        const bool compression)
{
    /*
     * Write plain text ASC file to memory
     */

    QString asc;
    QTextStream ascStream(&asc);
    ascStream.setRealNumberNotation(QTextStream::ScientificNotation);
    ascStream.setRealNumberPrecision(10);
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

    /*
     * Write to (compressed) file.
     */

    if (compression)
    {
        /* We utilize GDAL's virtual file system to write to a zip file, see
         *      https://gdal.org/user/virtual_file_systems.html
         * and
         *      https://gdal.org/api/cpl.html
         * for information.
         */
        QString filenameZIP = "/vsizip/" + pathCol + "/" + filenameASC + ".zip";
        std::clog << filenameZIP.toStdString() << std::endl;
        VSILFILE* fileZIP = VSIFOpenL(filenameZIP.toStdString().c_str(), "wb");
        VSILFILE* fileASCinZIP = VSIFOpenL((filenameZIP + "/" + filenameASC).toStdString().c_str(), "wb");
        std::string ascSTL = asc.toStdString();
        VSIFWriteL(ascSTL.c_str(), sizeof(char), ascSTL.size(), fileASCinZIP);
        VSIFCloseL(fileASCinZIP);
        VSIFCloseL(fileZIP);
    }
    else
    {
        QSaveFile file(pathCol + "/" + filenameASC);
        if (!file.open(QIODevice::WriteOnly))
        {
            return false;
        }
        file.write(asc.toUtf8());
        file.commit();
    }

    return true;
}

//------------------------------------------------------------------------------

/*! \brief Converts the given title to a string without any spaces
 *
 * \return Title with spaces replaced by underscores.
 */
QString gpsinfoMainDialog::titleAsDirectory() const
{
    return QString(ui->lineEdit_title->text()).replace(' ', '_');
}

//------------------------------------------------------------------------------

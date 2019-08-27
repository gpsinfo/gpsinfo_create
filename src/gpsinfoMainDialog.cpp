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
	std::clog << "gpsinfoMainDialog::on_pushButton_create_clicked - not implemented yet." << std::endl;

    const QString& title = ui->lineEdit_title->text();
    const QString& description = ui->lineEdit_description->text();
    const QString& copyright = ui->lineEdit_copyright->text();
    const QString& url = ui->lineEdit_URL->text();

    const QString& name = ui->lineEdit_name->text();
    const QString& city = ui->lineEdit_city->text();
    const QString& country = ui->lineEdit_country->text();
    const QString& email = ui->lineEdit_email->text();

    const unsigned int tileSizeX = (unsigned int) ui->spinBox_x->value();
    const unsigned int tileSizeY = (unsigned int) ui->spinBox_y->value();

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
    xml.writeTextElement("ows:Title", title);
    xml.writeEndElement();

    /*
     * ows:ServiceProvider
     */

    xml.writeStartElement("ows:ServiceIdentification");
    xml.writeTextElement("ows:Title", title);
    xml.writeEndElement();

    /*
     * Contents - Layer
     */

    xml.writeStartElement("Contents");

    xml.writeStartElement("Layer");
    xml.writeTextElement("ows:Title", "Layer for " + title);
    xml.writeEndElement();

    /*
     * Contents - TileMatrixSet
     */

    xml.writeStartElement("TileMatrixSet");
    xml.writeTextElement("ows:Title", "Tiles for " + title);
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

    /* For compressing the asc tiles, we could use
     *      QByteArray::qCompress
     * which uses zlib.
     */

	ui->progressBar->setEnabled(false);
}

//------------------------------------------------------------------------------

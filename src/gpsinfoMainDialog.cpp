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
//	ui->lineEdit_input->setText(m_filenameInput);
	m_directoryOutput = settings.value("m_directoryOutput", "").toString();
//	ui->lineEdit_output->setText(m_directoryOutput);
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

void gpsinfoMainDialog::on_pushButton_create_clicked()
{
	ui->progressBar->setEnabled(true);
	std::clog << "gpsinfoMainDialog::on_pushButton_create_clicked - not implemented yet." << std::endl;
	ui->progressBar->setEnabled(false);
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

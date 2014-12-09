/*
 * Histogram.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: tuomas
 */

#include "Histogram.hpp"
#include "ui_Histogram.h"

#include <QDialogButtonBox>

Histogram::Histogram(QWidget * parent, CommandLineParser const & cmd_parser) :
		QMainWindow(parent), ui(new Ui::MainWindow), cmd_parser(cmd_parser)
{
	ui->setupUi(this);
	connectSignals();
	showData();
}

void Histogram::connectSignals()
{
	connect(ui->buttonBox, &QDialogButtonBox::clicked, this,
			&Histogram::on_buttonBox_clicked);
}

void Histogram::on_buttonBox_clicked(QAbstractButton * button)
{
	switch (ui->buttonBox->buttonRole(button))
	{
	case QDialogButtonBox::ButtonRole::AcceptRole:
		qDebug() << "AcceptRole";
		break;
	case QDialogButtonBox::ButtonRole::DestructiveRole:
		qDebug() << "DestructiveRole";
		close();
		break;
	default:
		break;
	}
}

void Histogram::readData()
{

}

void Histogram::showData()
{
	ui->lineEdit_limit->setText(QString::number(cmd_parser.getLimit()));
	ui->lineEdit_range->setText(QString::number(cmd_parser.getRange()));
}


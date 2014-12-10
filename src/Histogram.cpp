/*
 * Histogram.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: tuomas
 */

#include "Histogram.hpp"
#include "ui_Histogram.h"

#include <QBrush>
#include <QColor>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QPixmap>
#include <QRectF>

#include <qwt_plot_histogram.h>
#include <qwt_samples.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_shapeitem.h>

Histogram::Histogram(QWidget * parent, const CommandLineParser & cmd_parser) :
		QMainWindow(parent), ui(new Ui::MainWindow), cmd_parser(cmd_parser)
{
	ui->setupUi(this);
	connectSignals();
	readData();
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
		saveData();
		break;
	case QDialogButtonBox::ButtonRole::DestructiveRole:
		close();
		break;
	default:
		break;
	}
}

void Histogram::saveData()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
			"untitled.png", tr("Images (*.png *.jpg)"));

	if (fileName != "")
	{
		QPixmap img(ui->qwtPlot->grab());
		if (img.save(fileName))
		{
			ui->statusbar->showMessage(tr("%1 saved.").arg(fileName), 5000);
		}
		else
		{
			ui->statusbar->showMessage(tr("Error saving %1.").arg(fileName),
					5000);
		}
	}
}

void Histogram::readData()
{
	samples.clear();
	for (int i = -30; i < 30; ++i)
	{
		samples.append(QwtIntervalSample(i, i, i + 1));
	}
}

void Histogram::showData()
{
	double limit = cmd_parser.getLimit();
	double range = cmd_parser.getRange();

	// set range and limit texts
	ui->lineEdit_limit->setText(QString::number(limit));
	ui->lineEdit_range->setText(QString::number(range));

	// set plot background
	ui->qwtPlot->setCanvasBackground(QBrush(Qt::white));

	// draw histogram
	QwtPlotHistogram * hist = new QwtPlotHistogram("Speed difference");
	hist->setBrush(QBrush(Qt::green));
	hist->setSamples(samples);
	hist->attach(ui->qwtPlot);

	// draw vertical lines
	for (auto i : QVector<double> { -1, 1 })
	{
		QwtPlotMarker * line = new QwtPlotMarker;
		line->setLineStyle(QwtPlotMarker::LineStyle::VLine);
		line->setLinePen(Qt::black, 2);
		line->setXValue(limit * i);
		line->attach(ui->qwtPlot);
	}

	// draw boxes around lines
	QColor c = Qt::magenta;
	c.setAlpha(128);
	for (auto i : QVector<double> { -1, 1 })
	{
		QwtPlotShapeItem * box = new QwtPlotShapeItem;
		QRectF r(limit * i - range, -30, 2 * range, 60);
		box->setRect(r);
		box->setBrush(QBrush(c));
		box->setZ(20);
		box->attach(ui->qwtPlot);
	}
}


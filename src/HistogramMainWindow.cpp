/*
 * Histogram.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: tuomas
 */

#include "HistogramMainWindow.hpp"
#include "ui_Histogram.h"
#include <algorithm>
#include <QBrush>
#include <QColor>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QPixmap>
#include <QRectF>
#include <qwt_plot_histogram.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_shapeitem.h>
#include <qwt_samples.h>

HistogramMainWindow::HistogramMainWindow(QWidget * parent,
		CommandLineParserInterface const & cmd_line,
		Histogram const & histogram, double ratio) :
		QMainWindow(parent), ui(new Ui::MainWindow), cmd_line(cmd_line), histogram(
				histogram), ratio(ratio)
{
	ui->setupUi(this);
	connectSignals();
	readData();
	showData();
}

void HistogramMainWindow::connectSignals()
{
	connect(ui->buttonBox, &QDialogButtonBox::clicked, this,
			&HistogramMainWindow::on_buttonBox_clicked);
}

void HistogramMainWindow::on_buttonBox_clicked(QAbstractButton * button)
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

void HistogramMainWindow::saveData()
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

void HistogramMainWindow::readData()
{
	double value, bin_start, bin_end;
	samples.clear();
	for (int i = 0; i < histogram.data.size(); ++i)
	{
		value = histogram.data[i];
		bin_start = histogram.low_limit + i * histogram.bin_size;
		bin_end = histogram.low_limit + (i + 1) * histogram.bin_size;
		samples.append(QwtIntervalSample(value, bin_start, bin_end));
	}
}

void HistogramMainWindow::showData()
{
	double limit = cmd_line.getLimit();
	double range = cmd_line.getRange();

	// set range and limit texts
	ui->lineEdit_limit->setText(QString::number(limit));
	ui->lineEdit_range->setText(QString::number(range));
	ui->lineEdit_ratio->setText(QString::number(ratio));

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
	auto max_value = std::max_element(histogram.data.begin(),
			histogram.data.end());
	QColor c = Qt::magenta;
	c.setAlpha(128);
	for (auto i : QVector<double> { -1, 1 })
	{
		QwtPlotShapeItem * box = new QwtPlotShapeItem;
		QRectF r(limit * i - range, 0, 2 * range, *max_value);
		box->setRect(r);
		box->setBrush(QBrush(c));
		box->setZ(20);
		box->attach(ui->qwtPlot);
	}
}


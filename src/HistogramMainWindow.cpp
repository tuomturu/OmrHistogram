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
	double target = cmd_line.getTargetVelocity();
	double lower_limit = cmd_line.getLowerLimit();
	double upper_limit = cmd_line.getUpperLimit();
	int filter_len = cmd_line.getFilterLength();

	// set range and limit texts
	ui->lineEdit_lower_limit->setText(QString::number(lower_limit));
	ui->lineEdit_upper_limit->setText(QString::number(upper_limit));
	ui->lineEdit_ratio->setText(QString::number(ratio));
	ui->lineEdit_filterLength->setText(QString::number(filter_len));

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
		line->setXValue(target * i);
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
		// QRectF(qreal left, qreal top, qreal width, qreal height);
		QRectF r((target - lower_limit) * i, 0, (upper_limit + lower_limit)*i, *max_value);
		box->setRect(r);
		box->setBrush(QBrush(c));
		box->setZ(20);
		box->attach(ui->qwtPlot);
	}
}


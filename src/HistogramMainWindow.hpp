/*
 * Histogram.hpp
 *
 *  Created on: Dec 9, 2014
 *      Author: tuomas
 */

#pragma once

#include "Analyzer.hpp"
#include "CommandLineParserInterface.hpp"
#include <memory>
#include <QAbstractButton>
#include <QMainWindow>
#include <qwt_samples.h>

namespace Ui
{
class MainWindow;
}

class HistogramMainWindow: public QMainWindow
{
	Q_OBJECT

private:
	typedef QVector<QwtIntervalSample> SampleVector;

public:
	HistogramMainWindow(QWidget * parent,
			CommandLineParserInterface const & cmd_line,
			Histogram const & histogram, double ratio);

private:
	void connectSignals();
	void readData();
	void showData();

	void on_buttonBox_clicked(QAbstractButton * button);
	void saveData();

private:
	std::shared_ptr<Ui::MainWindow> ui;
	SampleVector samples;

	CommandLineParserInterface const & cmd_line;
	Histogram const & histogram;
	double ratio;
};

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

/**
 * Mainwindow with histogram.
 */
class HistogramMainWindow: public QMainWindow
{
	Q_OBJECT

private:
	typedef QVector<QwtIntervalSample> SampleVector;

public:
	/**
	 * Shows command line parameters and calculated histogram and ratio in a gui
	 */
	HistogramMainWindow(QWidget * parent,
			CommandLineParserInterface const & cmd_line,
			Histogram const & histogram, double ratio);

private:
	/**
	 * Connects signals
	 */
	void connectSignals();

	/**
	 * Reads data from histogram structure
	 */
	void readData();

	/**
	 * Draws data to histogram plot
	 */
	void showData();

	/**
	 * Saves data if ok clicked
	 */
	void on_buttonBox_clicked(QAbstractButton * button);

	/**
	 * Saves data to an image
	 */
	void saveData();

private:
	//! ui elements
	std::shared_ptr<Ui::MainWindow> ui;
	//! histogram bins
	SampleVector samples;

	//! ref to command line
	CommandLineParserInterface const & cmd_line;
	//! ref to calculates histogram
	Histogram const & histogram;
	//! calculated ratio
	double ratio;
};

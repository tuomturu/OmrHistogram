/*
 * Histogram.hpp
 *
 *  Created on: Dec 9, 2014
 *      Author: tuomas
 */

#pragma once

#include <memory>

#include <QAbstractButton>
#include <QMainWindow>

#include <qwt_samples.h>

#include "CommandLineParser.hpp"

namespace Ui
{
class MainWindow;
}

class Histogram: public QMainWindow
{
Q_OBJECT

private:
	typedef QVector<QwtIntervalSample> SampleVector;

public:
	Histogram(QWidget * parent, CommandLineParser const & cmd_parser);

private:
	void connectSignals();
	void readData();
	void showData();

	void on_buttonBox_clicked(QAbstractButton * button);
	void saveData();

private:
	std::shared_ptr<Ui::MainWindow> ui;
	SampleVector samples;

	CommandLineParser const & cmd_parser;
};

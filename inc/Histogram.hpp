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

#include "CommandLineParser.hpp"

namespace Ui
{
class MainWindow;
}

class Histogram: public QMainWindow
{
Q_OBJECT

public:
	Histogram(QWidget * parent, CommandLineParser const & cmd_parser);

private:
	void connectSignals();
	void readData();
	void showData();

	void on_buttonBox_clicked(QAbstractButton * button);

private:
	std::shared_ptr<Ui::MainWindow> ui;
	CommandLineParser const & cmd_parser;
};

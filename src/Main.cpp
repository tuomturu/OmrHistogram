/*
 * main.cpp
 *
 *  Created on: Dec 8, 2014
 *      Author: tuomas
 */

#include <iostream>
#include <stdexcept>

#include <QApplication>
#include <QDebug>
#include <QtGlobal>

#include "Analyzer.hpp"
#include "CommandLineParser.hpp"
#include "DataParser.hpp"
#include "HistogramMainWindow.hpp"

int main(int argc, char * argv[])
{
	QApplication app(argc, argv);
	QApplication::setApplicationName("okr-histogram");
	QApplication::setApplicationVersion("0.1");

	try
	{
		CommandLineParser cmd_line(app);
		DataParser stimulus(cmd_line.getStimulusPath());
		DataParser signal(cmd_line.getSignalPath());

		Analyzer analyzer(cmd_line, stimulus.getDataVector("angle"),
				signal.getDataVector("angle"));

		HistogramMainWindow histogram(nullptr, cmd_line, analyzer.getHistogram(), analyzer.getRatio());
		histogram.show();
		return app.exec();

	} catch (std::runtime_error & e)
	{
		qCritical(e.what());
		return EXIT_FAILURE;
	}
}

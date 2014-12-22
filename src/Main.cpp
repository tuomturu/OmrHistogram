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
#include "Histogram.hpp"

int main(int argc, char * argv[])
{
	QApplication app(argc, argv);
	QApplication::setApplicationName("okr-histogram");
	QApplication::setApplicationVersion("0.1");

	try
	{
		CommandLineParser parser(app);
		DataParser stimulus(parser.getStimulusPath());
		DataParser signal(parser.getSignalPath());

		Analyzer analyzer(parser, stimulus, signal);

		Histogram histogram(nullptr, parser);
		histogram.show();

	} catch (std::runtime_error & e)
	{
		qCritical(e.what());
		return EXIT_FAILURE;
	}

	qDebug() << "Ready";
	return app.exec();
}

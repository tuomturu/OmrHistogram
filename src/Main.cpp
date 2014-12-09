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

#include "CommandLineParser.hpp"
#include "Histogram.hpp"

int main(int argc, char * argv[])
{
	QApplication app(argc, argv);
	QApplication::setApplicationName("okr-histogram");
	QApplication::setApplicationVersion("0.1");
	CommandLineParser parser;

	try
	{
		parser.process(app);

	} catch (std::runtime_error & e)
	{
		qCritical(e.what());
		return EXIT_FAILURE;
	}

	Histogram histogram(nullptr, parser);
	histogram.show();

	qDebug() << "Ready";
	return app.exec();
}

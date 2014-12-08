/*
 * main.cpp
 *
 *  Created on: Dec 8, 2014
 *      Author: tuomas
 */

#include <iostream>
#include <stdexcept>

#include <QCoreApplication>
#include <QDebug>
#include <QtGlobal>

#include "CommandLineParser.hpp"

int main(int argc, char * argv[])
{
	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("okr-histogram");
	QCoreApplication::setApplicationVersion("0.1");
	CommandLineParser parser;

	try
	{
		parser.process(app);
	} catch (std::runtime_error & e)
	{
		qCritical(e.what());
		return EXIT_FAILURE;
	}

	//return app.exec();
	return EXIT_SUCCESS;
}

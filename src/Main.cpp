/*
 * main.cpp
 *
 *  Created on: Dec 8, 2014
 *      Author: tuomas
 */

#include <iostream>

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>

int main(int argc, char * argv[])
{
	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("okr-histogram");
	QCoreApplication::setApplicationVersion("0.1");

	QCommandLineParser parser;
	parser.setApplicationDescription("Test helper");
	parser.addHelpOption();
	parser.addVersionOption();

	qDebug() << "Moi maailma";

	return app.exec();
}

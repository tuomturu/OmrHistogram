/*
 * CommandLineParser.h
 *
 *  Created on: Dec 8, 2014
 *      Author: tuomas
 */

#pragma once

#include <QObject>
#include <QCoreApplication>

class CommandLineParser: public QObject
{
	Q_OBJECT

public:
	CommandLineParser();
	void process(QCoreApplication & app);

public:
	QString getStimulusPath() const;
	QString getResultPath() const;
	double getLimit() const;
	double getRange() const;

private:
	QString stimulus_path;
	QString result_path;
	double limit;
	double range;
};

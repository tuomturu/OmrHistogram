/*
 * CommandLineParser.h
 *
 *  Created on: Dec 8, 2014
 *      Author: tuomas
 */

#pragma once

#include "CommandLineParserInterface.hpp"

#include <QObject>
#include <QCoreApplication>

class CommandLineParser: public QObject, public CommandLineParserInterface
{
	Q_OBJECT

public:
	CommandLineParser(QCoreApplication & app);

	QString getStimulusPath() const;
	QString getSignalPath() const;
	double getStimulusSamplingRate() const;
	double getSignalSamplingRate() const;
	double getLimit() const;
	double getRange() const;
	int getFilter() const;

private:
	void process(QCoreApplication & app);

private:
	QString stimulus_path;
	QString signal_path;

	double stimulus_fs;
	double signal_fs;

	double limit;
	double range;
	int filter;
};

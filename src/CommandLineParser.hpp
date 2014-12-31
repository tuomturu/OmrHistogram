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
#include <QCommandLineParser>

class CommandLineParser: public QObject, public CommandLineParserInterface
{
	Q_OBJECT

public:
	CommandLineParser(QCoreApplication & app);

	QString getStimulusPath() const;
	QString getSignalPath() const;
	double getStimulusSamplingRate() const;
	double getSignalSamplingRate() const;
	double getTargetVelocity() const;
	double getLowerLimit() const;
	double getUpperLimit() const;
	int getFilterLength() const;
	double getHistogramRange() const;
	double getHistogramBinSize() const;

private:
	void process(QCoreApplication & app);
	QCommandLineOption addOption(QString const & name,
			QString const & description, double default_value);

	int parseInt(QCommandLineOption const & option);
	double parseDouble(QCommandLineOption const & option);

private:
	QCommandLineParser parser;

	QString stimulus_path;
	QString signal_path;

	double stimulus_fs;
	double signal_fs;

	double target_velocity;
	double lower_limit;
	double upper_limit;
	int filter_length;

	double histogram_range;
	double histogram_bin_size;
};

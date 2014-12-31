/*
 * CommandLineParser.cpp
 *
 *  Created on: Dec 8, 2014
 *      Author: tuomas
 */

#include "CommandLineParser.hpp"

#include <stdexcept>

#include <QCommandLineParser>
#include <QtGlobal>

CommandLineParser::CommandLineParser(QCoreApplication & app) :
		lower_limit(0), upper_limit(0), filter_length(0)
{
	process(app);
}

// helpers -->

QCommandLineOption CommandLineParser::addOption(QString const & name,
		QString const & description, double default_value)
{
	QString value_name = QString::number(default_value);
	QCommandLineOption option(name, description, value_name,
			QString::number(default_value));
	parser.addOption(option);

	return option;
}

int CommandLineParser::parseInt(QCommandLineOption const & option)
{
	bool ok;
	int value = parser.value(option).toInt(&ok);
	if (!ok)
	{
		QStringList names = option.names();
		QString name = names.last();

		QString msg =
				QString("Error parsing command line input for %1. '%2' given").arg(
						name).arg(parser.value(option));
		throw std::runtime_error(msg.toStdString());
	}
	return value;
}
double CommandLineParser::parseDouble(QCommandLineOption const & option)
{
	bool ok;
	double value = parser.value(option).toDouble(&ok);
	if (!ok)
	{
		QStringList names = option.names();
		QString name = names.last();

		QString msg =
				QString("Error parsing command line input for %1. '%2' given").arg(
						name).arg(parser.value(option));
		throw std::runtime_error(msg.toStdString());
	}
	return value;
}

// <-- helpers

void CommandLineParser::process(QCoreApplication & app)
{
	parser.setApplicationDescription("Okr histogram plotter");
	parser.addHelpOption();
	parser.addVersionOption();

	// add options -->
	parser.addPositionalArgument("stimulus",
			tr("Stimulus control file used in the experiment."));

	parser.addPositionalArgument("signal",
			tr("Tracking result from the experiment."));

	QCommandLineOption stimulus_fs = addOption("stimulus-fs",
			tr("Stimulus sampling rate"), 60);

	QCommandLineOption signal_fs = addOption("signal-fs",
			tr("Signal sampling rate"), 25);

	QCommandLineOption target_velocity =
			addOption("target-velocity",
					tr(
							"Target velocity for drawing histogram. Does not affect to the calculated ratio."),
					12);

	QCommandLineOption lower_limit = addOption("lower-limit",
			tr("Max deviation from stimulus speed to negative direction"), 10);

	QCommandLineOption upper_limit = addOption("upper-limit",
			tr("Max deviation from stimulus speed to positive direction"), 2);

	QCommandLineOption filter_length = addOption("filter-length",
			tr("Filter length"), 15);

	QCommandLineOption histogram_range = addOption("histogram-range",
			tr("Histogram range (-range,range) "), 20);

	QCommandLineOption histogram_bin_size = addOption("histogram-bin-size",
			tr("Histogram bin size"), 0.5);

	// process output -->

	parser.process(app);
	QStringList const args = parser.positionalArguments();

	if (args.size() != 2)
	{
		throw std::runtime_error(
				"Application takes 2 positional arguments (stimulus file path and signal file path). See help for details.");
	}

	// positional options
	stimulus_path = args[0];
	signal_path = args[1];

	// key name options
	this->stimulus_fs = parseDouble(stimulus_fs);
	this->signal_fs = parseDouble(signal_fs);
	this->target_velocity = parseDouble(target_velocity);
	this->lower_limit = parseDouble(lower_limit);
	this->upper_limit = parseDouble(upper_limit);
	this->filter_length = parseInt(filter_length);
	this->histogram_range = parseDouble(histogram_range);
	this->histogram_bin_size = parseDouble(histogram_bin_size);
}

QString CommandLineParser::getStimulusPath() const
{
	return stimulus_path;
}

QString CommandLineParser::getSignalPath() const
{
	return signal_path;
}

double CommandLineParser::getStimulusSamplingRate() const
{
	return stimulus_fs;
}

double CommandLineParser::getSignalSamplingRate() const
{
	return signal_fs;
}

double CommandLineParser::getTargetVelocity() const
{
	return target_velocity;
}

double CommandLineParser::getLowerLimit() const
{
	return lower_limit;
}

double CommandLineParser::getUpperLimit() const
{
	return upper_limit;
}

int CommandLineParser::getFilterLength() const
{
	return filter_length;
}

double CommandLineParser::getHistogramRange() const
{
	return histogram_range;
}

double CommandLineParser::getHistogramBinSize() const
{
	return histogram_bin_size;
}

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
		limit(0), range(0), filter(0)
{
	process(app);
}

void CommandLineParser::process(QCoreApplication & app)
{
	QCommandLineParser parser;

	// add options -->

	parser.setApplicationDescription("Okr histogram plotter");
	parser.addHelpOption();
	parser.addVersionOption();

	parser.addPositionalArgument("stimulus",
			tr("Stimulus control file used in the experiment."));
	parser.addPositionalArgument("result",
			tr("Tracking result from the experiment."));

	QCommandLineOption stimulus_fs_option("stimulus_fs",
			tr("Stimulus sampling rate (default 60)."),
			tr("Stimulus sampling rate"), "60");
	parser.addOption(stimulus_fs_option);

	QCommandLineOption signal_fs_option("signal_fs",
			tr("Signal sampling rate (default 25)."),
			tr("Signal sampling rate"), "25");
	parser.addOption(signal_fs_option);

	QCommandLineOption limitOption(QStringList { "l", "limit" },
			tr("Limit value (default 12)."), tr("limit"), "12");
	parser.addOption(limitOption);

	QCommandLineOption rangeOption(QStringList { "r", "range" },
			tr("Range around limit (default +-2)."), tr("range"), "2");
	parser.addOption(rangeOption);

	QCommandLineOption filterOption(QStringList { "f", "filter" },
			tr("Filter length (default 15)."), tr("filter"), "15");
	parser.addOption(filterOption);

	// process output -->

	parser.process(app);
	QStringList const args = parser.positionalArguments();

	if (args.size() != 2)
	{
		throw std::runtime_error(
				"Application takes 2 positional arguments. See help for details.");
	}

	stimulus_path = args[0];
	signal_path = args[1];

	bool ok;

	stimulus_fs = parser.value(stimulus_fs_option).toDouble(&ok);
	if (!ok)
	{
		QString msg = QString(
				"Stimulus sample rate has to be a number. '%1' given").arg(
				parser.value(stimulus_fs_option));
		throw std::runtime_error(msg.toStdString());
	}

	signal_fs = parser.value(signal_fs_option).toDouble(&ok);
	if (!ok)
	{
		QString msg = QString(
				"Signal sample rate has to be a number. '%1' given").arg(
				parser.value(signal_fs_option));
		throw std::runtime_error(msg.toStdString());
	}

	limit = parser.value(limitOption).toDouble(&ok);
	if (!ok)
	{
		QString msg = QString("Limit has to be a number. '%1' given").arg(
				parser.value(limitOption));
		throw std::runtime_error(msg.toStdString());
	}

	range = parser.value(rangeOption).toDouble(&ok);
	if (!ok)
	{
		QString msg = QString("Range has to be a number. '%1' given").arg(
				parser.value(rangeOption));
		throw std::runtime_error(msg.toStdString());
	}

	filter = parser.value(filterOption).toDouble(&ok);
	if (!ok)
	{
		QString msg =
				QString("Filter length has to be a number. '%1' given").arg(
						parser.value(filterOption));
		throw std::runtime_error(msg.toStdString());
	}
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

double CommandLineParser::getLimit() const
{
	return limit;
}

double CommandLineParser::getRange() const
{
	return range;
}

int CommandLineParser::getFilter() const
{
	return filter;
}

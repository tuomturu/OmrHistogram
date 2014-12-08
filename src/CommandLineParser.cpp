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

CommandLineParser::CommandLineParser() :
		limit(0), range(0)
{
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

	QCommandLineOption limitOption(QStringList() << "l" << "limit",
			tr("Limit value (default 12)."), tr("limit"), "12");
	parser.addOption(limitOption);

	QCommandLineOption rangeOption(QStringList() << "r" << "range",
			tr("Range around limit (default +-2)."), tr("range"), "2");
	parser.addOption(rangeOption);

	// process output -->

	parser.process(app);
	QStringList const args = parser.positionalArguments();

	if (args.size() != 2)
	{
		throw std::runtime_error(
				"Application takes 2 positional arguments. See help for details.");
	}

	stimulus_path = args[0];
	result_path = args[1];

	bool ok;
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
}

QString CommandLineParser::getStimulusPath() const
{
	return stimulus_path;
}

QString CommandLineParser::getResultPath() const
{
	return result_path;
}

double CommandLineParser::getLimit() const
{
	return limit;
}

double CommandLineParser::getRange() const
{
	return range;
}

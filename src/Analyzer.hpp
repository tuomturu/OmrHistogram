/*
 * Analyzer.h
 *
 *  Created on: Dec 22, 2014
 *      Author: tuomas
 */

#pragma once

#include "CommandLineParserInterface.hpp"
#include <QVector>

struct Histogram
{
	QVector<double> data;
	double low_limit;
	double high_limit;
	double bin_size;
};

class Analyzer
{
public:
	typedef QVector<double> Data;

public:
	Analyzer(CommandLineParserInterface const & cmd_line, Data const & stimulus,
			Data const & signal);

	Histogram const & getHistogram() const;
	double getRatio() const;

protected:
	Analyzer(CommandLineParserInterface const & cmd_line, Data const & stimulus,
			Data const & signal, bool testing);

	Data mirrorPadSignal(Data signal, int pad_length,
			int default_value = 0) const;
	Data borderValuePadSignal(Data signal, int pad_length) const;

	Data filterSignal(Data const & signal, int filter_len) const;

	double angleDiff(double current, double previous) const;
	Data compareSignals(Data const & stimulus, double stimulus_fs,
			Data const & signal, double signal_fs) const;

	double correctIncorrectRatio(Data const & data, double target,
			double tolerance) const;

	Histogram calculateHistogram(Data const & data, double bin_count = 10,
			double bin_size = -1);

	Histogram calculateHistogram(Data const & data, double low_lim,
			double high_lim, double bin_size);

private:
	CommandLineParserInterface const & cmd_line;
	Data stimulus;
	Data signal;

	// calculated results
	double ratio;
	Histogram histogram;
};

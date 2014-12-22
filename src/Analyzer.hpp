/*
 * Analyzer.h
 *
 *  Created on: Dec 22, 2014
 *      Author: tuomas
 */

#pragma once

#include "CommandLineParserInterface.hpp"
#include "DataParserInterface.hpp"

class Analyzer
{
public:
	typedef QVector<double> Data;
public:
	Analyzer(CommandLineParserInterface const & cmd_line,
			DataParserInterface const & stimulus, double stimulus_fs,
			DataParserInterface const & signal, double signal_fs);

	double run();

protected:
	Data mirrorPadSignal(Data signal, int pad_length, int default_value = 0);
	Data borderValuePadSignal(Data signal, int pad_length);

	Data filterSignal(Data const & signal, int filter_len);
	Data compareSignals(Data const & stimulus, Data const & signal);


private:
	CommandLineParserInterface const & cmd_line;

	double stimulus_fs;
	double signal_fs;

	Data stimulus;
	Data signal;

};

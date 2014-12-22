/*
 * Analyzer.cpp
 *
 *  Created on: Dec 22, 2014
 *      Author: tuomas
 */

#include "Analyzer.hpp"

Analyzer::Analyzer(CommandLineParserInterface const & cmd_line,
		DataParserInterface const & stimulus, double stimulus_fs,
		DataParserInterface const & signal, double signal_fs) :
		cmd_line(cmd_line), stimulus_fs(stimulus_fs), signal_fs(signal_fs)
{
	this->stimulus = stimulus.getDataVector("angle");
	this->signal = signal.getDataVector("angle");
}

double Analyzer::run()
{
	Data filtered = filterSignal(signal, cmd_line.getFilter());
	Data comparison = compareSignals(stimulus, signal);

	return 0;
}

Analyzer::Data Analyzer::mirrorPadSignal(Data signal, int pad_length,
		int default_value)
{
	// copy original signal
	Data padded = signal;
	int signal_len = signal.size();

	// pad front
	for (int i = 1; i < pad_length + 1; ++i)
	{
		padded.push_front(signal.value(i, 0));
	}

	// pad back
	for (int i = 1; i < pad_length + 1; ++i)
	{
		padded.push_back(signal.value(signal_len - i - 1, 0));
	}

	return padded;
}

Analyzer::Data Analyzer::borderValuePadSignal(Data signal, int pad_length)
{
	// copy original signal
	Data padded = signal;

	// pad front
	double val = signal[0];
	for (int i = 1; i < pad_length + 1; ++i)
	{
		padded.push_front(val);
	}

	// pad back
	val = signal.last();
	for (int i = 1; i < pad_length + 1; ++i)
	{
		padded.push_back(val);
	}

	return padded;
}

Analyzer::Data Analyzer::filterSignal(Data const & signal, int filter_len)
{
	// target vector
	Data filtered = Data(signal.size(), 0);

	// filter length
	int half_filter = (filter_len - 1) / 2;
	int signal_len = signal.size();

	// pad signal with mirrored ends
	//Data padded = mirrorPadSignal(signal, half_filter);

	// pad with border value
	Data padded = borderValuePadSignal(signal, half_filter);

	// filter signal with a moving average centered window
	int s, e;
	double val;
	for (int i = 0; i < signal_len; ++i)
	{
		s = i - half_filter;
		e = i + half_filter;

		val = 0;
		for (int j = s; j <= e; ++j)
		{
			val += padded[j + half_filter];
			padded.value(j, 0);
		}
		filtered[i] = val / filter_len;
	}

	return filtered;
}

Analyzer::Data Analyzer::compareSignals(Data const & stimulus,
		Data const & signal)
{
	Data comparison;
	// assume both start the same time
	// take all values from signal and compare to the closest time point in stimulus

	// scale times according to sampling rates
	double scale = stimulus_fs / signal_fs;

	// length according to the shortest one
	int len = qMin(int(stimulus.size() / scale), signal.size());

	int j = 0;
	double v_stimulus;
	double v_signal;

	// for every measured signal value
	for (int i = 0; i < len; ++i)
	{
		j = i * scale;
		if (j - 1 < 0 || i - 1 < 0)
		{
			// skip values at the beginning
			continue;
		}

		// calculate speeds
		v_stimulus = (stimulus[j] - stimulus[j - 1]) * stimulus_fs;
		v_signal = (signal[i] - signal[i - 1]) * signal_fs;

		// store speed difference
		comparison.append(v_stimulus - v_signal);
	}

	return comparison;
}

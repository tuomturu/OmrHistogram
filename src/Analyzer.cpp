/*
 * Analyzer.cpp
 *
 *  Created on: Dec 22, 2014
 *      Author: tuomas
 */

#include "Analyzer.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <QDebug>
#include <stdexcept>

Analyzer::Analyzer(CommandLineParserInterface const & cmd_line,
		Data const & stimulus, Data const & signal) :
		cmd_line(cmd_line), stimulus(stimulus), signal(signal)
{
	// filter signal
	Data filtered = filterSignal(signal, cmd_line.getFilter());

	// read sampling rates and compare speeds
	double stimulus_fs = cmd_line.getStimulusSamplingRate();
	double signal_fs = cmd_line.getSignalSamplingRate();
	Data comparison = compareSignals(stimulus, stimulus_fs, filtered,
			signal_fs);

	// calculate correct-incorrect ratio
	double target = cmd_line.getLimit();
	double tolerance = cmd_line.getRange();
	ratio = correctIncorrectRatio(comparison, target, tolerance);

	// generate histogram
	histogram = calculateHistogram(comparison, -50, 50, tolerance/2);
}

Analyzer::Analyzer(const CommandLineParserInterface& cmd_line,
		Data const & stimulus, Data const & signal, bool testing) :
		cmd_line(cmd_line), stimulus(stimulus), signal(signal), ratio(0)
{
}

Histogram const & Analyzer::getHistogram() const
{
	return histogram;
}

double Analyzer::getRatio() const
{
	return ratio;
}

Analyzer::Data Analyzer::mirrorPadSignal(Data signal, int pad_length,
		int default_value) const
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

Analyzer::Data Analyzer::borderValuePadSignal(Data signal, int pad_length) const
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

Analyzer::Data Analyzer::filterSignal(Data const & signal, int filter_len) const
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

double Analyzer::angleDiff(double current, double previous) const
{
	double v = current - previous;
	if (v > 180)
	{
		v -= 360;
	}
	else if (v < -180)
	{
		v += 360;
	}

	return v;
}

//double Analyzer::angleDiff(double current, double previous) const
//{
//	double v = current - previous;
//	return v;
//}

Analyzer::Data Analyzer::compareSignals(Data const & stimulus,
		double stimulus_fs, Data const & signal, double signal_fs) const
{
	Data comparison;
	// assume both start the same time
	// take all values from signal and compare to the closest time point in stimulus

	// scale times according to sampling rates
	double scale = stimulus_fs / signal_fs;

	// length according to the shortest one
	int len = qMin(int(stimulus.size() / scale), signal.size());

	int j = 0;
	double v;
	double v_stimulus;
	double v_signal;
	bool correct_direction;
	double v_diff;
	// for every measured signal value
	for (int i = 0; i < len; ++i)
	{
		j = i * scale;

		if (j - 1 < 0 || i - 1 < 0)
		{
			// skip values at the beginning
			continue;
		}

		// calculate speeds with angle wrapping. assume values are between [0-360[
		v = angleDiff(stimulus[j], stimulus[j - 1]);
		v_stimulus = v * stimulus_fs;
		//
		v = angleDiff(signal[i], signal[i - 1]);
		v_signal = v * signal_fs;

		// remove direction and calculate absolute difference
		correct_direction = std::signbit(v_stimulus) == std::signbit(v_signal);
		v_diff = std::abs(std::abs(v_stimulus) - std::abs(v_signal));

		// sign tells if both are goint to the same direction
		if (!correct_direction)
		{
			v_diff *= -1;
		}

		qDebug() << i << "\t" << j << "\t" << int(std::round(v_signal)) << "\t" << int(std::round(v_stimulus))
				<< "\t" << int(std::round(v_diff));
		// store speed difference
		comparison.append(v_diff);
	}

	return comparison;
}

double Analyzer::correctIncorrectRatio(Data const & data, double target,
		double tolerance) const
{
	// 0 for correct, 1 for incorrect
	Data bins(2, 0);
	double a;

	// calculate if signal starts to follow stimulus with a given target speed and tolerance
	for (auto d : data)
	{
		a = std::abs(d);
		if (target - tolerance < a && a < target + tolerance)
		{
			bins[std::signbit(d)] += 1;
		}
	}

	// may return inf
	return bins[0] / bins[1];
}

Histogram Analyzer::calculateHistogram(Data const & data, double bin_count,
		double bin_size)
{
	if (data.isEmpty() || (bin_count <= 0 && bin_size <= 0))
	{
		throw std::runtime_error(
				"data.isEmpty() || (bin_count <= 0 && bin_size <= 0)");
	}

	// bounds from min and max
	auto minmax = std::minmax_element(data.begin(), data.end());

	// from start of the first bin to the end of the last
	if (bin_size <= 0)
	{
		bin_size = (*minmax.second - *minmax.first) / bin_count;
	}
	// call worker
	return calculateHistogram(data, *minmax.first, *minmax.second, bin_size);
}

Histogram Analyzer::calculateHistogram(Data const & data, double low_lim,
		double high_lim, double bin_size)
{
	if (data.isEmpty() || low_lim >= high_lim || bin_size <= 0)
	{
		throw std::runtime_error(
				"data.isEmpty() || low_lim >= high_lim || bin_size <= 0");
	}

	// calculate bin backwards from the simpler function
	int bin_count = (high_lim - low_lim) / bin_size
			+ std::numeric_limits<double>::epsilon();
	double hv = high_lim - low_lim;

	Histogram hist;
	hist.data = Data(bin_count, 0);
	// add data to bins

	int i;
	for (auto d : data)
	{
		// calculate index from position between low and high lim
		i = (d - low_lim) / hv * bin_count;

		// put all values outside of the bounds to boundary bins
		i = qMin(i, bin_count - 1);
		i = qMax(i, 0);
		hist.data[i] += 1;
	}

	hist.bin_size = bin_size;
	hist.low_limit = low_lim;
	hist.high_limit = high_lim;
	return hist;
}

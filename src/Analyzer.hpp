/*
 * Analyzer.h
 *
 *  Created on: Dec 22, 2014
 *      Author: tuomas
 */

#pragma once

#include "CommandLineParserInterface.hpp"
#include <QVector>
#include <QPair>

/**
 * Contains histogram data, limits and bin size
 */
struct Histogram
{
	QVector<double> data;
	double low_limit;
	double high_limit;
	double bin_size;
};

/**
 * Holds stimulus and signals speeds and the speed difference
 */
struct SpeedComparison
{
	double stimulus_speed;
	double signal_speed;

	bool same_direction;
	double difference;
};

/**
 * Analyses stimulus and recorded signal
 */
class Analyzer
{
public:
	typedef QVector<double> Data;
	typedef QVector<SpeedComparison> SpeedComparisonVector;

public:
	/**
	 * Uses command line parameters and calculates speed difference
	 * between stimulus and recorded signal.
	 *
	 * Calculates histogram and ratio which can be fetched from object
	 */
	Analyzer(CommandLineParserInterface const & cmd_line, Data const & stimulus,
			Data const & signal);

	/**
	 * @return reference to calculated histogram
	 */
	Histogram const & getHistogram() const;

	/**
	 * @return correct incorrect ratio
	 */
	double getRatio() const;

protected:
	/**
	 * Constructor used for testing that does not run calculations
	 */
	Analyzer(CommandLineParserInterface const & cmd_line, Data const & stimulus,
			Data const & signal, bool testing);

	/**
	 * Pads signal by mirroring edge values
	 */
	Data mirrorPadSignal(Data signal, int pad_length,
			int default_value = 0) const;

	/**
	 * Pads signal by copying edge values
	 */
	Data borderValuePadSignal(Data signal, int pad_length) const;

	/**
	 * Filters signal with running average filter
	 */
	Data filterSignal(Data const & signal, int filter_len) const;

	/**
	 * Calculates difference of angles.
	 * Takes account the wrap in 360 degrees
	 */
	double angleDiff(double current, double previous) const;

	/**
	 * Compares stimulus and signal speeds and directions to each other
	 */
	SpeedComparisonVector calculateSpeedDifference(Data const & stimulus,
			double stimulus_fs, Data const & signal, double signal_fs) const;

	/**
	 * Calculates correct-incorrect ratio from speed comparison
	 */
	double correctIncorrectRatio(SpeedComparisonVector const & data,
			double lower_limit, double upper_limit) const;

	/**
	 * Calculates histogram
	 * Calculates low and high limits and bin sizes from data
	 */
	Histogram calculateHistogram(Data const & data, double bin_count = 10,
			double bin_size = -1);

	/**
	 * Calculates histogram
	 */
	Histogram calculateHistogram(Data const & data, double low_lim,
			double high_lim, double bin_size);

private:
	//! ref to command line interface
	CommandLineParserInterface const & cmd_line;
	//! parsed stimulus
	Data stimulus;
	//! parsed signal
	Data signal;

	//! correct-incorrect ratio
	double ratio;
	//! calculated histogram
	Histogram histogram;
};

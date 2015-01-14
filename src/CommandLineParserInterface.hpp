/*
 * CommandLineParserInterface.hpp
 *
 *  Created on: Dec 22, 2014
 *      Author: tuomas
 */

#pragma once

#include <QString>

/**
 * Interface for command line parameter parser
 */
class CommandLineParserInterface
{

public:
	virtual ~CommandLineParserInterface()
	{
	}

	/**
	 * @return stimulus path
	 */
	virtual QString getStimulusPath() const = 0;

	/**
	 * @return signal path
	 */
	virtual QString getSignalPath() const = 0;

	/**
	 * @return stimulus sampling rate
	 */
	virtual double getStimulusSamplingRate() const = 0;

	/**
	 * @return recorded signal sampling rate
	 */
	virtual double getSignalSamplingRate() const = 0;

	/**
	 * @return target velocity for drawing boundaries to histogram gui
	 */
	virtual double getTargetVelocity() const = 0;

	/**
	 * @return low limit speed
	 */
	virtual double getLowerLimit() const = 0;

	/**
	 * @return upper limit for speed
	 */
	virtual double getUpperLimit() const = 0;

	/**
	 * @return filter length
	 */
	virtual int getFilterLength() const = 0;

	/**
	 * @return range for histogram gui +-range
	 */
	virtual double getHistogramRange() const = 0;

	/**
	 * @return histogram bin size
	 */
	virtual double getHistogramBinSize() const = 0;
};

/*
 * CommandLineParserInterface.hpp
 *
 *  Created on: Dec 22, 2014
 *      Author: tuomas
 */

#pragma once

#include <QString>

class CommandLineParserInterface
{

public:
	virtual ~CommandLineParserInterface()
	{
	}

	virtual QString getStimulusPath() const = 0;
	virtual QString getSignalPath() const = 0;
	virtual double getStimulusSamplingRate() const = 0;
	virtual double getSignalSamplingRate() const = 0;
	virtual double getTargetVelocity() const = 0;
	virtual double getLowerLimit() const = 0;
	virtual double getUpperLimit() const = 0;
	virtual int getFilterLength() const = 0;
	virtual double getHistogramRange() const = 0;
	virtual double getHistogramBinSize() const = 0;
};

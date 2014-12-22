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
	virtual double getLimit() const = 0;
	virtual double getRange() const = 0;
	virtual int getFilter() const = 0;
};

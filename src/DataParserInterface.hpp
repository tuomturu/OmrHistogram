/*
 * DataParserInterface.hpp
 *
 *  Created on: Dec 10, 2014
 *      Author: tuomas
 */

#pragma once

#include <QVector>
#include <QString>

/**
 * Interface to read data from a file
 */
class DataParserInterface
{
public:
	virtual ~DataParserInterface()
	{
	}

	/**
	 * Reads and parses given file
	 * @param filename The file to read
	 * @return True when successful
	 */
	virtual bool parseFile(const QString & filename) = 0;

	/**
	 * @return True when parsed data contains key
	 */
	virtual bool contains(const QString & key) const = 0;

	/**
	 * Returns a copy of parsed vector with a given key
	 * @param key Key to search
	 * @return Vector for a given key or an empty vector if not found
	 */
	virtual QVector<double> getDataVector(const QString & key) const = 0;
};

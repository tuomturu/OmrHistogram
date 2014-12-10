/*
 * DataParser.hpp
 *
 *  Created on: Dec 10, 2014
 *      Author: tuomas
 */

#pragma once

#include <QObject>
#include <QStringList>

#include "DataParserInterface.hpp"

/**
 * Implements data parser for simple csv files with header used as a key
 */
class DataParser: public DataParserInterface
{
public:
	DataParser();
	bool parseFile(const QString & filename);
	bool contains(const QString & key) const;
	QVector<double> getDataVector(const QString & key) const;

private:
	//! typedef for data structure
	typedef QVector<QVector<double> > Matrix;

	//! csv column separator
	const char separator;

	//! header
	QStringList header;

	//! data
	Matrix data;
};

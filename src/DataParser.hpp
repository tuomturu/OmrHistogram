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
	DataParser(const QString & filename);
	bool contains(const QString & key) const;
	QVector<double> getDataVector(const QString & key) const;

private:
	void parseFile(const QString & filename);

private:
	//! typedef for data structure
	typedef QVector<QVector<double> > Matrix;

	//! csv column separator
	static char const separator = ',';

	//! comment char
	static char const comment = '#';

	// comments from start
	QVector<QStringList> comments;

	//! header
	QStringList header;

	//! data
	Matrix data;
};

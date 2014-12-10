/*
 * DataParser.hpp
 *
 *  Created on: Dec 10, 2014
 *      Author: tuomas
 */

#pragma once

#include <QMap>
#include <QObject>


#include "DataParserInterface.hpp"

/**
 * Implements data parser for simple csv files with header used as a key
 */
class DataParser: public DataParserInterface
{
public:
	bool parseFile(const QString & filename);
	bool contains(const QString & key) const;
	QVector<double> getDataVector(const QString & key) const;

private:
	//! datastructure
	typedef QMap<QString, QVector<double> > Matrix;

	//! data
	Matrix data;
};

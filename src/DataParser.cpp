/*
 * DataParser.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: tuomas
 */

#include "DataParser.hpp"

#include <algorithm>
#include <stdexcept>

#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QVector>

DataParser::DataParser(const QString & filename)
{
	parseFile(filename);
}

void DataParser::parseFile(const QString & filename)
{
	QFile file(filename);
	QTextStream in(&file);
	if (!file.open(QIODevice::ReadOnly))
	{
		throw std::runtime_error(
				("Cannot open file " + filename).toStdString());
	}

	QStringList splitted;
	QString line;
	double d;

	// read header
	while (!in.atEnd())
	{
		// remove white space and comment chars
		line = in.readLine().trimmed();
		if (line.startsWith(comment))
		{
			line = line.remove(comment);
			if (line != "")
			{
				splitted = line.split(separator);
				// trim tags
				for (int i = 0; i < splitted.size(); ++i)
				{
					splitted[i] = splitted[i].trimmed();
				}
				comments.append(splitted);
			}
		}
		else if (line != "")
		{
			break;
		}
	}

	// Assume the line containing most splits is the header
	auto longest = std::max_element(comments.begin(), comments.end(),
			[](QStringList const & a, QStringList const & b)
			{	return a.size() < b.size();});

	// no header found
	if (longest == comments.end())
	{
		throw std::runtime_error(
				("Cannot parse header from file " + filename).toStdString());
	}
	header = *longest;

	// create empty data matrix
	int width = header.size();
	data = Matrix(width, QVector<double>());

	// read numeric data
	while (!in.atEnd() || !line.isEmpty())
	{
		if (line != "")
		{
			splitted = line.split(separator);
			if (splitted.size() != width)
			{
				// size does not match header
				throw std::runtime_error(
						("Error parsing data from file " + filename).toStdString());
			}

			for (int i = 0; i < width; ++i)
			{
				d = splitted[i].toDouble();
				data[i].append(d);
			}
		}
		line = in.readLine().trimmed();
	}
}

bool DataParser::contains(const QString & key) const
{
	return header.contains(key);
}

QVector<double> DataParser::getDataVector(const QString & key) const
{
	int idx = header.indexOf(key);
	if (idx >= 0 && idx < data.size())
	{
		return data[idx];
	}

	throw std::runtime_error((key + "not found").toStdString());
}

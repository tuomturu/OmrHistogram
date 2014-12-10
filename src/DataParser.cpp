/*
 * DataParser.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: tuomas
 */

#include "DataParser.hpp"

#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QVector>

DataParser::DataParser() :
		separator(';')
{
}

bool DataParser::parseFile(const QString & filename)
{
	// clear old data
	header.clear();
	data.clear();

	QFile file(filename);
	QTextStream in(&file);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	QStringList splitted;
	QString line;
	double d;

	// read header
	while (!in.atEnd())
	{
		line = in.readLine().trimmed();
		if (line != "")
		{
			header = line.split(separator);
			//qDebug() << line;
			break;
		}
	}

	// create empty data matrix
	int width = header.size();
	data = Matrix(width, QVector<double>());

	// read numeric data
	while (!in.atEnd())
	{
		line = in.readLine().trimmed();
		if (line != "")
		{
			splitted = line.split(separator);
			if (splitted.size() != width)
			{
				// size does not match header
				header.clear();
				data.clear();
				return false;
			}

			for (int i = 0; i < width; ++i)
			{
				//qDebug() << line;
				//qDebug() << splitted[i];
				d = splitted[i].toDouble();
				data[i].append(d);
			}
		}
	}

	return true;
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

	return QVector<double>();
}


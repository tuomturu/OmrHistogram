/*
 * DataParser_test.cpp
 *
 *  Created on: Dec 10, 2014
 *      Author: tuomas
 */

#include "DataParser.hpp"
#include <QtTest/QtTest>

/**
 * Tests data parser
 */
class TestDataParser: public QObject
{
Q_OBJECT

private slots:
	void parseFile()
	{
		bool b;
		DataParser dp;
		b = dp.parseFile("tmp/stimulus.csv");
		QCOMPARE(b, true);

		b = dp.parseFile("tmp/stimulus.csv.error");
		QCOMPARE(b, false);
	}

	void contains()
	{
		bool b;
		DataParser dp;
		dp.parseFile("tmp/stimulus.csv");

		QVector<QString> test_set { "x", "y", "x1", "y1", "angle", "result",
				"rand" };

		for (auto s : test_set)
		{
			b = dp.contains("x");
			QCOMPARE(b, true);
		}

		b = dp.contains("err");
		QCOMPARE(b, false);
	}

	void getDataVector()
	{
		QVector<double> d;
		DataParser dp;
		dp.parseFile("tmp/stimulus.csv");

		d = dp.getDataVector("x");
		QCOMPARE(d.size(), 719);

		d = dp.getDataVector("rand");
		QCOMPARE(d[0], 1.37);

		d = dp.getDataVector("err");
		QCOMPARE(d.size(), 0);
	}
};

QTEST_MAIN(TestDataParser)
#include "DataParser_test.moc"

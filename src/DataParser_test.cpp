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
		DataParser("tmp/stimulus.txt");
		DataParser("tmp/signal.txt");
		QVERIFY_EXCEPTION_THROWN(DataParser("tmp/stimulus.txt.error"),
				std::runtime_error);
	}

	void stimulusContains()
	{
		bool b;
		DataParser dp("tmp/stimulus.txt");

		QVector<QString> test_set { "C", "V", "x", "y", "z", "angle", "angle_c",
				"maskAngle", "maskAngle_c" };

		for (auto s : test_set)
		{
			b = dp.contains(s);
			QCOMPARE(b, true);
		}

		b = dp.contains("err");
		QCOMPARE(b, false);
	}

	void signalContains()
	{
		bool b;
		DataParser dp("tmp/signal.txt");

		QVector<QString> test_set { "Frame", "Snout X", "Snout Y", "Head1 X",
				"Head1 Y", "Head2 X", "Head2 Y", "Body X", "Body Y", "Tail X",
				"Tail Y", "angle" };

		for (auto s : test_set)
		{
			b = dp.contains(s);
			QCOMPARE(b, true);
		}

		b = dp.contains("err");
		QCOMPARE(b, false);
	}

	void stimulusGetDataVector()
	{
		QVector<double> d;
		DataParser dp("tmp/stimulus.txt");

		d = dp.getDataVector("x");
		QCOMPARE(d.size(), 3600);

		d = dp.getDataVector("angle");
		QCOMPARE(d[1], 0.2);
		QCOMPARE(d.last(), 0.2);

		QVERIFY_EXCEPTION_THROWN(dp.getDataVector("err"), std::runtime_error);
	}

	void signalGetDataVector()
	{
		QVector<double> d;
		DataParser dp("tmp/signal.txt");

		d = dp.getDataVector("Frame");
		QCOMPARE(d.size(), 1489);
		QCOMPARE(d[0], 0.0);
		QCOMPARE(d.last(), 1488.0);

		d = dp.getDataVector("Head1 X");
		QCOMPARE(d[1], 294.0);

		QVERIFY_EXCEPTION_THROWN(dp.getDataVector("err"), std::runtime_error);
	}

};

QTEST_MAIN(TestDataParser)
#include "DataParser_test.moc"

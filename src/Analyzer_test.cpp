/*
 * Analyzer_test.cpp
 *
 *  Created on: Dec 22, 2014
 *      Author: tuomas
 */

#include "Analyzer.hpp"
#include <QtTest/QtTest>
#include <QtMath>

#include "CommandLineParserInterface.hpp"
#include "DataParser.hpp"

class DummyCommandLine: public CommandLineParserInterface
{
public:
	QString getStimulusPath() const
	{
		return "tmp/stimulus.txt";
	}
	QString getSignalPath() const
	{
		return "tmp/signal.txt";
	}
	double getLimit() const
	{
		return 12;
	}
	double getRange() const
	{
		return 2;
	}
	int getFilter() const
	{
		return 15;
	}
};

class DummyDataParser: public DataParserInterface
{
public:
	int samples;

	DummyDataParser(int samples = 250) :
			samples(samples)
	{
	}

	bool contains(const QString & key) const
	{
		return true;
	}

	QVector<double> getDataVector(const QString & key) const
	{
		//double end = 2 * 3.141592;
		double end = 10;

		QVector<double> s;
		for (double i = 0; i < samples; ++i)
		{
			//s.append(qSin(i / samples * end));
			s.append(i / samples * end);
		}
		return s;
	}
};

class AnalyzerProxy: public Analyzer
{
public:
	AnalyzerProxy(CommandLineParserInterface const & cmd_line,
			DataParserInterface const & stimulus, double stimulus_fs,
			DataParserInterface const & signal, double signal_fs) :
			Analyzer(cmd_line, stimulus, stimulus_fs, signal, signal_fs)
	{
	}

	using Analyzer::mirrorPadSignal;
	using Analyzer::borderValuePadSignal;
	using Analyzer::filterSignal;
	using Analyzer::compareSignals;
};

/**
 * Tests data parser
 */
class TestAnalyzer: public QObject
{
Q_OBJECT

private:
	typedef Analyzer::Data Data;

	DummyCommandLine cmd_line;
	DummyDataParser dummy_stimulus;
	DummyDataParser dummy_signal;

	int stimulus_len;
	int signal_len;

	int stimulus_fs;
	int signal_fs;

private slots:

	void initTestCase()
	{
		stimulus_len = 60;
		signal_len = 25;

		stimulus_fs = 60;
		signal_fs = 25;
		dummy_stimulus = DummyDataParser(stimulus_len);
		dummy_signal = DummyDataParser(signal_len);
	}

	void mirrorPadSignal()
	{
		Data data = dummy_signal.getDataVector("");
		AnalyzerProxy ap(cmd_line, dummy_stimulus, stimulus_fs, dummy_signal,
				signal_fs);

		int pad = 3;
		Data padded = ap.mirrorPadSignal(data, pad);

		QCOMPARE(data.size(), signal_len);
		QCOMPARE(padded.size(), signal_len + 2 * pad);

		QCOMPARE(data[0], padded[pad]);
		QCOMPARE(data[1], padded[pad - 1]);
		QCOMPARE(data[2], padded[pad - 2]);
		QCOMPARE(data[3], padded[pad - 3]);

		QCOMPARE(data[signal_len - 1], padded[signal_len + pad - 1]);
		QCOMPARE(data[signal_len - 2], padded[signal_len + pad - 1 + 1]);
		QCOMPARE(data[signal_len - 3], padded[signal_len + pad - 1 + 2]);
		QCOMPARE(data[signal_len - 4], padded[signal_len + pad - 1 + 3]);
	}

	void borderValuePadSignal()
	{
		Data data = dummy_signal.getDataVector("");
		AnalyzerProxy ap(cmd_line, dummy_stimulus, stimulus_fs, dummy_signal,
				signal_fs);

		int pad = 3;
		Data padded = ap.borderValuePadSignal(data, pad);

		QCOMPARE(data.size(), signal_len);
		QCOMPARE(padded.size(), signal_len + 2 * pad);

		QCOMPARE(data[0], padded[pad]);
		QCOMPARE(data[0], padded[pad - 1]);
		QCOMPARE(data[0], padded[pad - 2]);
		QCOMPARE(data[0], padded[pad - 3]);

		QCOMPARE(data[signal_len - 1], padded[signal_len + pad - 1]);
		QCOMPARE(data[signal_len - 1], padded[signal_len + pad - 1 + 1]);
		QCOMPARE(data[signal_len - 1], padded[signal_len + pad - 1 + 2]);
		QCOMPARE(data[signal_len - 1], padded[signal_len + pad - 1 + 3]);
	}

	void filterSignal()
	{
		Data data = dummy_signal.getDataVector("");
		AnalyzerProxy ap(cmd_line, dummy_stimulus, stimulus_fs, dummy_signal,
				signal_fs);

		int filter = 3;
		Data filtered = ap.filterSignal(data, filter);

		int w = (filter - 1) / 2;
		double d;
		for (int i = w; i < signal_len - w; ++i)
		{
			d = 0;
			for (int j = -w; j <= w; ++j)
			{
				d += data[i + j];

			}
			QCOMPARE(d / filter, filtered[i]);
		}
	}

	void compareSignals()
	{
		AnalyzerProxy ap(cmd_line, dummy_stimulus, stimulus_fs, dummy_signal,
				signal_fs);
		Data signal = dummy_signal.getDataVector("");

		QVector<int> samples = { 10, 25, 30, 50, 60, 120, 180 };
		for (auto sample_size : samples)
		{
			Data stimulus = DummyDataParser(sample_size).getDataVector("");
			Data comparison = ap.compareSignals(stimulus, signal);

			double target = (stimulus[1] - stimulus[0]) * stimulus_fs
					- (signal[1] - signal[0]) * signal_fs;

			for (auto d : comparison)
			{
				QVERIFY(d > target - 1e-6 && d < target + 1e-6);
			}
		}
	}

};

QTEST_MAIN(TestAnalyzer)
#include "Analyzer_test.moc"

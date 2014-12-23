/*
 * Analyzer_test.cpp
 *
 *  Created on: Dec 22, 2014
 *      Author: tuomas
 */

#include "Analyzer.hpp"
#include "CommandLineParserInterface.hpp"
#include "DataParser.hpp"
#include <algorithm>
#include <cmath>
#include <QDebug>
#include <QtMath>
#include <QtTest/QtTest>

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
	double getStimulusSamplingRate() const
	{
		return 60;
	}
	double getSignalSamplingRate() const
	{
		return 25;
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

class AnalyzerProxy: public Analyzer
{
public:
	typedef Analyzer::Data Data;

	AnalyzerProxy(CommandLineParserInterface const & cmd_line,
			Data const & stimulus, Data const & signal) :
			Analyzer(cmd_line, stimulus, signal, true)
	{
	}

	using Analyzer::mirrorPadSignal;
	using Analyzer::borderValuePadSignal;
	using Analyzer::filterSignal;
	using Analyzer::compareSignals;
	using Analyzer::correctIncorrectRatio;
	using Analyzer::calculateHistogram;

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

	Data stimulus;
	Data signal;

	int stimulus_len;
	int signal_len;

	int stimulus_fs;
	int signal_fs;

private:
	QVector<double> getDataVector(int samples) const
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

private slots:

	void initTestCase()
	{
		stimulus_len = 60;
		signal_len = 25;

		stimulus_fs = 60;
		signal_fs = 25;

		stimulus = getDataVector(stimulus_len);
		signal = getDataVector(signal_len);
	}

	void mirrorPadSignal()
	{
		AnalyzerProxy ap(cmd_line, stimulus, signal);

		int pad = 3;
		Data padded = ap.mirrorPadSignal(signal, pad);

		QCOMPARE(signal.size(), signal_len);
		QCOMPARE(padded.size(), signal_len + 2 * pad);

		QCOMPARE(signal[0], padded[pad]);
		QCOMPARE(signal[1], padded[pad - 1]);
		QCOMPARE(signal[2], padded[pad - 2]);
		QCOMPARE(signal[3], padded[pad - 3]);

		QCOMPARE(signal[signal_len - 1], padded[signal_len + pad - 1]);
		QCOMPARE(signal[signal_len - 2], padded[signal_len + pad - 1 + 1]);
		QCOMPARE(signal[signal_len - 3], padded[signal_len + pad - 1 + 2]);
		QCOMPARE(signal[signal_len - 4], padded[signal_len + pad - 1 + 3]);
	}

	void borderValuePadSignal()
	{
		AnalyzerProxy ap(cmd_line, stimulus, signal);

		int pad = 3;
		Data padded = ap.borderValuePadSignal(signal, pad);

		QCOMPARE(signal.size(), signal_len);
		QCOMPARE(padded.size(), signal_len + 2 * pad);

		QCOMPARE(signal[0], padded[pad]);
		QCOMPARE(signal[0], padded[pad - 1]);
		QCOMPARE(signal[0], padded[pad - 2]);
		QCOMPARE(signal[0], padded[pad - 3]);

		QCOMPARE(signal[signal_len - 1], padded[signal_len + pad - 1]);
		QCOMPARE(signal[signal_len - 1], padded[signal_len + pad - 1 + 1]);
		QCOMPARE(signal[signal_len - 1], padded[signal_len + pad - 1 + 2]);
		QCOMPARE(signal[signal_len - 1], padded[signal_len + pad - 1 + 3]);
	}

	void filterSignal()
	{
		AnalyzerProxy ap(cmd_line, stimulus, signal);

		int filter = 3;
		Data filtered = ap.filterSignal(signal, filter);

		int w = (filter - 1) / 2;
		double d;
		for (int i = w; i < signal_len - w; ++i)
		{
			d = 0;
			for (int j = -w; j <= w; ++j)
			{
				d += signal[i + j];

			}
			QCOMPARE(d / filter, filtered[i]);
		}
	}

	void compareSignals()
	{
		AnalyzerProxy ap(cmd_line, stimulus, signal);

		QVector<int> samples = { 10, 25, 30, 50, 60, 120, 180 };
		for (auto sample_size : samples)
		{
			Data test_stimulus = getDataVector(sample_size);
			Data comparison = ap.compareSignals(test_stimulus, stimulus_fs,
					signal, signal_fs);

			double target = std::abs(
					(test_stimulus[1] - test_stimulus[0]) * stimulus_fs
							- (signal[1] - signal[0]) * signal_fs);

			for (auto d : comparison)
			{
				QVERIFY(d > target - 1e-6 && d < target + 1e-6);
			}
		}
	}

	void correctIncorrectRatio()
	{
		AnalyzerProxy ap(cmd_line, stimulus, signal);

		Data samples = { 12, 12, 13, -11, -12 };

		double ratio = ap.correctIncorrectRatio(samples, 12, 2);
		QCOMPARE(ratio, 3.0 / 2);
	}

	void calculateHistogram1()
	{
		AnalyzerProxy ap(cmd_line, stimulus, signal);

		// pl.hstack((pl.randn(50) - 1, pl.randn(50) + 1))
		Data samples = { -1.58275997, -2.17062385, -0.57155631, -3.2105393,
				-1.9917614, -1.61701411, -2.78371857, 0.39484506, -1.40578281,
				0.6652892, -0.21238868, -0.80629895, -2.11855708, -1.0892493,
				1.25314808, -0.21730133, 0.59999126, 0.66587083, -1.26339376,
				1.19768668, 0.97902889, -3.02257791, -2.11240892, -1.91418776,
				-2.0614589, -0.78719851, 1.31829536, -0.71089719, 0.84173314,
				-0.43904019, -2.69428184, -1.66350467, -1.24958304, -1.24019497,
				0.34556666, -1.08792584, -0.92818582, -2.18056522, -0.73369833,
				-0.33192006, -2.16549396, -0.90789643, -0.20171148, -1.17135509,
				-2.23968517, 0.83083294, -1.31088247, 1.42588271, -1.57951484,
				-3.10395987, -0.05155893, 2.78745435, 1.43566394, -0.34751668,
				0.97660521, 1.0802003, -0.59176671, 0.25606418, 0.41863478,
				-0.80979136, 1.79844847, 0.97237003, 1.36850438, 1.92885669,
				-0.30314704, -0.59552987, -0.47403244, 1.58468463, 0.38725202,
				1.26493649, -0.90282333, -0.79822681, -0.08963273, -0.02744054,
				-0.25541307, 0.55042103, 0.16582595, 1.53841126, 1.32939869,
				0.87727602, 1.3777731, 0.51237188, -1.58093722, -1.13634725,
				1.45328666, 2.01103185, -0.67521749, -0.67860028, 3.41473541,
				1.43273427, 1.04432757, 1.28241459, 1.77544757, 0.79253083,
				1.56406405, 1.5161107, 1.05511196, 1.4729429, -1.19498303,
				-0.43880651 };

		int bins = 15;
		Histogram hist = ap.calculateHistogram(samples, bins);
		// pl.histogram(samples, 15)
		Data numpy_hist { 4, 1, 9, 5, 10, 14, 10, 5, 9, 11, 16, 4, 0, 1, 1 };
		QCOMPARE(hist.data.size(), bins);
		QVERIFY(
				std::equal(hist.data.begin(), hist.data.end(),
						numpy_hist.begin()));

		bins = 7;
		hist = ap.calculateHistogram(samples, bins);
		numpy_hist = Data { 5, 15, 26, 18, 27, 7, 2 };
		QCOMPARE(hist.data.size(), bins);
		QVERIFY(
				std::equal(hist.data.begin(), hist.data.end(),
						numpy_hist.begin()));

		bins = 3;
		hist = ap.calculateHistogram(samples, bins);
		numpy_hist = Data { 29, 49, 22 };
		QCOMPARE(hist.data.size(), bins);
		QVERIFY(
				std::equal(hist.data.begin(), hist.data.end(),
						numpy_hist.begin()));

		bins = 76;
		hist = ap.calculateHistogram(samples, bins);
		numpy_hist = Data { 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 4, 2, 2, 1, 0, 0,
				1, 4, 0, 1, 1, 3, 3, 2, 0, 3, 4, 2, 3, 2, 3, 1, 3, 3, 1, 2, 0,
				1, 1, 1, 3, 1, 2, 2, 1, 3, 1, 4, 1, 1, 4, 3, 5, 3, 1, 0, 2, 1,
				1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 };
		QCOMPARE(hist.data.size(), bins);
		QVERIFY(
				std::equal(hist.data.begin(), hist.data.end(),
						numpy_hist.begin()));
	}

	void calculateHistogram2()
	{
		DataParser stimulus_parser(cmd_line.getStimulusPath());
		DataParser signal_parser(cmd_line.getSignalPath());

		stimulus = stimulus_parser.getDataVector("angle");
		signal = signal_parser.getDataVector("angle");

		AnalyzerProxy ap(cmd_line, stimulus, signal);

		// filter signal
		Data filtered = ap.filterSignal(signal, cmd_line.getFilter());

		// read sampling rates and compare speeds
		double stimulus_fs = cmd_line.getStimulusSamplingRate();
		double signal_fs = cmd_line.getSignalSamplingRate();
		Data comparison = ap.compareSignals(stimulus, stimulus_fs, filtered,
				signal_fs);

		auto minmax = std::minmax_element(comparison.begin(), comparison.end());
		qDebug() << *minmax.first;
		qDebug() << *minmax.second;

		// generate histogram
		Histogram histogram = ap.calculateHistogram(comparison, -200, 200, 1);
		qDebug() << histogram.data.size();
	}

	void fullTest()
	{
		DataParser stimulus_parser(cmd_line.getStimulusPath());
		DataParser signal_parser(cmd_line.getSignalPath());

		stimulus = stimulus_parser.getDataVector("angle");
		signal = signal_parser.getDataVector("angle");

		Analyzer analyzer(cmd_line, stimulus, signal);
		qDebug() << analyzer.getRatio();
	}

};

QTEST_MAIN(TestAnalyzer)
#include "Analyzer_test.moc"

// **************************************************************************
// File       [ tm_usage.h ]
// Author     [ littleshamoo ]
// Synopsis   [ Get CPU time and Memory usage ]
// Date       [ Ver 2.0 started 2010/03/23 ]
// History    [ created TmStat structure to store usage ]
// **************************************************************************

#ifndef _COMMON_TM_USAGE_H_
#define _COMMON_TM_USAGE_H_

namespace CommonNs {

	// define a data structure to store time and memory usage
	struct TmStat {
		long vmSize; // in kilobytes
		long vmPeak; // in kilobytes
		long vmDiff; // in kilobytes
		long rTime;  // real time in micro seconds
		long uTime;  // user time in micro seconds
		long sTime;  // system time in micro seconds 
	};

	class TmUsage {
		public:
			TmUsage();
			~TmUsage();

		bool totalStart();     // start the total timer.  this is called only once.
		bool periodStart();    // start the period timer. this can be called many times
		bool getTotalUsage(TmStat &st) const;   // get the total tm usage 
		bool getPeriodUsage(TmStat &st) const;  // get the priod tm usage
		bool checkUsage(TmStat &st) const;      // 

		private:
		TmStat tStart_;
		TmStat pStart_;
	};
} // namespace

#endif

// **************************************************************************
// HOW TO USE TMUSAGE?
//
//     CommonNs::TmUsage tmusg;
//     CommonNs::TmStat stat;
//     tmusg.totalStart();
//
//     // do something...
//
//     tmusg.periodStart();
//
//     // do something...
//
//     tmusg.getPeriodUsage(stat);
//     cout <<"user time: " << stat.uTime / 1000000.0 << "s" << endl; // print period user time in seconds
//     cout <<"system time: " << stat.sTime / 1000000.0 << "s" << endl; // print period systemtime in seconds
//     cout <<"user+system time:" << (stat.uTime + stat.sTime) / 1000000.0 << "s" << endl; 
//
//     // do something...
//
//     tmusg.getTotalUsage(stat);
//     cout << stat.vmSize / 1024.0 << "MB" << endl; // print current memory
//     cout << stat.vmPeak / 1024.0 << "MB" << endl; // print peak memory
// **************************************************************************

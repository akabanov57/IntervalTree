//============================================================================
// Name        : interval_tree_test.cpp
// Author      : andrei
// Version     :
// Copyright   : Your copyright notice
// Description : interval tree test
//============================================================================

#include <Interval.hpp>
#include <IntervalTree.hpp>
#include <iostream>
#include <utility>
#include <set>

int main() {
	using std::cout;
	using std::endl;
	using std::pair;
	using std::set;


	IntervalTree it1;

	// test overlap.
	// you can imagine your own intervals.
	Interval input[] = {
			Interval::valueOf(16,4),
			Interval::valueOf(2,6),
			Interval::valueOf(21,4),
			Interval::valueOf(26,5),
			Interval::valueOf(5,5),
			Interval::valueOf(8,2),
			Interval::valueOf(9,6),
			Interval::valueOf(0,6),
			Interval::valueOf(13,5),
			Interval::valueOf(0,26),
			Interval::valueOf(10,5)
	};

	/**
	 * Before inserting a new interval into the tree, look for the overlap for this interval.
	 */
	for (int i = 0; i < (int)(sizeof(input)/sizeof(Interval)); ++i) {
		cout << "New interval:    " << input[i] << endl;
		static set<Interval> overlaps;
		Interval interval;
		interval = input[i];
		it1.overlapSearch(interval, overlaps);
		cout << "overlaps for " << interval << ": ";
		for (set<Interval>::iterator j = overlaps.begin(); j != overlaps.end(); ++j) {
			cout << *j << " ";
		}
		cout << endl;
		it1.insert(interval);
		overlaps.clear();
		cout << "Result sequence: ";
		it1.prints();

		cout << "Tree structure:" << endl;
		it1.printt();
	}

	return 0;
}

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
#include <sstream>
#include <cassert>

void demoOverlap() {
    using std::cout;
    using std::endl;
    using std::pair;
    using std::set;


    IntervalTree it;

    // test overlap.
    // you can imagine your own intervals.
    Interval input[] = {
            Interval::valueOf(16,20),
            Interval::valueOf(2,8),
            Interval::valueOf(21,25),
            Interval::valueOf(26,31),
            Interval::valueOf(5,10),
            Interval::valueOf(8,10),
            Interval::valueOf(9,15),
            Interval::valueOf(0,6),
            Interval::valueOf(13,18),
            Interval::valueOf(0,26),
            Interval::valueOf(10,15)
    };

    /**
     * Before inserting a new interval into the tree, look for the overlap for this interval.
     */
    for (int i = 0; i < (int)(sizeof(input)/sizeof(Interval)); ++i) {
        cout << "New interval:    " << input[i] << endl;
        static set<Interval> overlaps;
        Interval interval;
        interval = input[i];
        it.overlapSearch(interval, overlaps);
        cout << "overlaps for " << interval << ": ";
        for (set<Interval>::iterator j = overlaps.begin(); j != overlaps.end(); ++j) {
            cout << *j << " ";
        }
        cout << endl;
        it.insert(interval);
        overlaps.clear();
        cout << "Result sequence: ";
        cout << SequenceWriter(it) << endl;

        cout << "Tree structure:" << endl;
        cout << HierarchyWriter(it) << endl;
    }
}

void interval_set_difference_Test() {
    using std::pair;
    using std::ostringstream;

    /**
     *                    i1
     *       |--------------------|
     *       5                    15
     *             i2
     *   |--------------|
     *   3              7
     */
    {
        Interval i1 = Interval::valueOf(5, 15);
        Interval i2 = Interval::valueOf(3, 7);
        pair<Interval, Interval> p = set_difference(i1, i2);
        ostringstream out;
        out << p.first << " " << p.second;
        assert(out.str() == "[0,0[ [7,15[");
    }

    /**
     *                    i1
     *       |--------------------|
     *       5                    15
     *                  i2
     *           |------------------|
     *           7                  16
     */
    {
        Interval i1 = Interval::valueOf(5, 15);
        Interval i2 = Interval::valueOf(7, 16);
        pair<Interval, Interval> p = set_difference(i1, i2);
        ostringstream out;
        out << p.first << " " << p.second;
        assert(out.str() == "[5,7[ [0,0[");
    }

    /**
     *                    i1
     *       |--------------------|
     *       5                    15
     *                  i2
     *           |----------|
     *           7          12
     */
    {
        Interval i1 = Interval::valueOf(5, 15);
        Interval i2 = Interval::valueOf(7, 12);
        pair<Interval, Interval> p = set_difference(i1, i2);
        ostringstream out;
        out << p.first << " " << p.second;
        assert(out.str() == "[5,7[ [12,15[");
    }

    /**
     *                    i1
     *       |--------------------|
     *       5                    15
     *                  i2
     *       |--------------------|
     *       5                    15
     */
    {
        Interval i1 = Interval::valueOf(5, 15);
        Interval i2 = Interval::valueOf(5, 15);
        pair<Interval, Interval> p = set_difference(i1, i2);
        ostringstream out;
        out << p.first << " " << p.second;
        assert(out.str() == "[0,0[ [0,0[");
    }

    /**
     *                    i1
     *       |--------------------|
     *       5                    15
     *                  i2
     *     |------------------------|
     *     4                        16
     */
    {
        Interval i1 = Interval::valueOf(5, 15);
        Interval i2 = Interval::valueOf(4, 16);
        pair<Interval, Interval> p = set_difference(i1, i2);
        ostringstream out;
        out << p.first << " " << p.second;
        assert(out.str() == "[0,0[ [0,0[");
    }
}

void interval_set_intersect_Test() {
    using std::pair;
    using std::ostringstream;

    /**
     *                    i1
     *       |--------------------|
     *       5                    15
     *             i2
     *   |--------------|
     *   3              7
     */
    {
        Interval i1 = Interval::valueOf(5, 15);
        Interval i2 = Interval::valueOf(3, 7);
        Interval i = set_intersect(i1, i2);
        ostringstream out;
        out << i;
        assert(out.str() == "[5,7[");
    }

    /**
     *                    i1
     *       |--------------------|
     *       5                    15
     *                  i2
     *           |------------------|
     *           7                  16
     */
    {
        Interval i1 = Interval::valueOf(5, 15);
        Interval i2 = Interval::valueOf(7, 16);
        Interval i = set_intersect(i1, i2);
        ostringstream out;
        out << i;
        assert(out.str() == "[7,15[");
    }

    /**
     *                    i1
     *       |--------------------|
     *       5                    15
     *                  i2
     *           |----------|
     *           7          12
     */
    {
        Interval i1 = Interval::valueOf(5, 15);
        Interval i2 = Interval::valueOf(7, 12);
        Interval i = set_intersect(i1, i2);
        ostringstream out;
        out << i;
        assert(out.str() == "[7,12[");
    }

    /**
     *                    i1
     *       |--------------------|
     *       5                    15
     *                  i2
     *       |--------------------|
     *       5                    15
     */
    {
        Interval i1 = Interval::valueOf(5, 15);
        Interval i2 = Interval::valueOf(5, 15);
        Interval i = set_intersect(i1, i2);
        ostringstream out;
        out << i;
        assert(out.str() == "[5,15[");
    }

    /**
     *                    i1
     *       |--------------------|
     *       5                    15
     *                  i2
     *     |------------------------|
     *     4                        16
     */
    {
        Interval i1 = Interval::valueOf(5, 15);
        Interval i2 = Interval::valueOf(4, 16);
        Interval i = set_intersect(i1, i2);
        ostringstream out;
        out << i;
        assert(out.str() == "[5,15[");
    }
}

void interval_set_union_Test() {
    using std::pair;
    using std::ostringstream;

    /**
     *                    i1
     *       |--------------------|
     *       5                    15
     *             i2
     *   |--------------|
     *   3              7
     */
    {
        Interval i1 = Interval::valueOf(5, 15);
        Interval i2 = Interval::valueOf(3, 7);
        Interval i = set_union(i1, i2);
        ostringstream out;
        out << i;
        assert(out.str() == "[3,15[");
    }

    /**
     *                    i1
     *       |--------------------|
     *       5                    15
     *                  i2
     *           |------------------|
     *           7                  16
     */
    {
        Interval i1 = Interval::valueOf(5, 15);
        Interval i2 = Interval::valueOf(7, 16);
        Interval i = set_union(i1, i2);
        ostringstream out;
        out << i;
        assert(out.str() == "[5,16[");
    }

    /**
     *                    i1
     *       |--------------------|
     *       5                    15
     *                  i2
     *           |----------|
     *           7          12
     */
    {
        Interval i1 = Interval::valueOf(5, 15);
        Interval i2 = Interval::valueOf(7, 12);
        Interval i = set_union(i1, i2);
        ostringstream out;
        out << i;
        assert(out.str() == "[5,15[");
    }

    /**
     *                    i1
     *       |--------------------|
     *       5                    15
     *                  i2
     *       |--------------------|
     *       5                    15
     */
    {
        Interval i1 = Interval::valueOf(5, 15);
        Interval i2 = Interval::valueOf(5, 15);
        Interval i = set_union(i1, i2);
        ostringstream out;
        out << i;
        assert(out.str() == "[5,15[");
    }

    /**
     *                    i1
     *       |--------------------|
     *       5                    15
     *                  i2
     *     |------------------------|
     *     4                        16
     */
    {
        Interval i1 = Interval::valueOf(5, 15);
        Interval i2 = Interval::valueOf(4, 16);
        Interval i = set_union(i1, i2);
        ostringstream out;
        out << i;
        assert(out.str() == "[4,16[");
    }
}

int main() {
    interval_set_difference_Test();
    interval_set_intersect_Test();
    interval_set_union_Test();
    demoOverlap();
	return 0;
}

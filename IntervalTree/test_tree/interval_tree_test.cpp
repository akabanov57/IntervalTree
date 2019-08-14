#include <iostream>
#include <sstream>
#include <set>
#include <exception>
#include <cassert>

#include <Interval.hpp>
#include <IntervalTree.hpp>
#include <interval_operations.hpp>

/**
 * User defined Interval.
 *
 * See also Interval.hpp, interval_operations.hpp
 */
template<typename T>
class ExtentT {
private:
    T offset_;
    T length_;
    ExtentT(T start, T end): offset_(start), length_(end - start) {}
public:
    ExtentT() {
        offset_ = static_cast<T>(0);
        length_ = static_cast<T>(0);
    }
    static ExtentT<T> valueOf(T start, T end) {
        using std::invalid_argument;
        if (start < 0) {
            throw invalid_argument("start < 0");
        }
        if (end < 0) {
            throw invalid_argument("end < 0");
        }
        if (end < start) {
            throw invalid_argument("end < start");
        }
        return ExtentT(start, end);
    }
    T start() const {
        return offset_;
    }
    T end() const {
        return offset_ + length_;
    }
    T length() const {
        return length_;
    }
};

template<typename T>
inline std::ostream& operator <<(std::ostream &out, const ExtentT<T>& i) {
    out << "[" << i.start() << "," << i.end() << "[";
    return out;
}

/**
 * Test with default Interval.
 */
void interval_set_difference_Test() {
    using std::pair;
    using std::ostringstream;

    typedef unsigned long IntType;
    typedef IntervalT<IntType> Interval;

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

/**
 * Test with the user defined Interval.
 */
void interval_set_difference_Test1() {
    using std::pair;
    using std::ostringstream;

    typedef int IntType;
    typedef ExtentT<IntType> Interval;

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

/**
 * Test with default Interval.
 */
void interval_set_intersect_Test() {
    using std::pair;
    using std::ostringstream;

    typedef unsigned long IntType;
    typedef IntervalT<IntType> Interval;

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

/**
 * Test with the user defined Interval.
 */
void interval_set_intersect_Test1() {
    using std::pair;
    using std::ostringstream;

    typedef unsigned int IntType;
    typedef ExtentT<IntType> Interval;

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

/**
 * Test with default Interval.
 */
void interval_set_union_Test() {
    using std::pair;
    using std::ostringstream;

    typedef unsigned long IntType;
    typedef IntervalT<IntType> Interval;

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

/**
 * Test with the user defined Interval.
 */
void interval_set_union_Test1() {
    using std::pair;
    using std::ostringstream;

    typedef long IntType;
    typedef ExtentT<IntType> Interval;

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

void intervalTree_Test() {
    using std::set;
    using std::cout;
    using std::endl;

    /**
     * use default Interval
     */
    {
        typedef unsigned long IntType;
        typedef IntervalT<IntType> Interval;

        IntervalTree<IntType> it;
        it.insert(Interval::valueOf(10,15));
        set<Interval> res;
        it.overlapSearch(Interval::valueOf(9,14),res);
        for (auto i: res) {
            cout << i << " " << endl;
        }
        cout << HierarchyWriter<IntType>(it) << endl;
        cout << SequenceWriter<IntType>(it) << endl;
    }

    /**
     * use the user defined Interval
     */
    {
        typedef int IntType;
        typedef ExtentT<IntType> Interval;

        IntervalTree<IntType, Interval> it;
        it.insert(Interval::valueOf(10,15));
        set<Interval> res;
        it.overlapSearch(Interval::valueOf(9,14),res);
        for (auto i: res) {
            cout << i << " " << endl;
        }
        cout << HierarchyWriter<IntType, Interval>(it) << endl;
        cout << SequenceWriter<IntType, Interval>(it) << endl;
    }
}

void demoOverlap() {
    using std::cout;
    using std::endl;
    using std::pair;
    using std::set;

    /**
     * use default Interval
     */
    {
        typedef unsigned long IntType;
        typedef IntervalT<IntType> Interval;

        IntervalTree<IntType> it;

        // test overlap.
        // you can imagine your own intervals.
        Interval input[] = {
                Interval::valueOf(16, 20),
                Interval::valueOf(2, 8),
                Interval::valueOf(21, 25),
                Interval::valueOf(26, 31),
                Interval::valueOf(5, 10),
                Interval::valueOf(8, 10),
                Interval::valueOf(9, 15),
                Interval::valueOf(0, 6),
                Interval::valueOf(13, 18),
                Interval::valueOf(0, 26),
                Interval::valueOf(10, 15)
        };

        /**
         * Before inserting a new interval into the tree, look for the overlap for this interval.
         */
        for (int i = 0; i < (int) (sizeof(input) / sizeof(Interval)); ++i) {
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
            cout << SequenceWriter<IntType>(it) << endl;

            cout << "Tree structure:" << endl;
            cout << HierarchyWriter<IntType>(it) << endl;
        }
    }

    /**
     * use the user defined Interval
     */
    {
        typedef unsigned int IntType;
        typedef ExtentT<IntType> Interval;

        IntervalTree<IntType, Interval> it;

        // test overlap.
        // you can imagine your own intervals.
        Interval input[] = {
                Interval::valueOf(16, 20),
                Interval::valueOf(2, 8),
                Interval::valueOf(21, 25),
                Interval::valueOf(26, 31),
                Interval::valueOf(5, 10),
                Interval::valueOf(8, 10),
                Interval::valueOf(9, 15),
                Interval::valueOf(0, 6),
                Interval::valueOf(13, 18),
                Interval::valueOf(0, 26),
                Interval::valueOf(10, 15)
        };

        /**
         * Before inserting a new interval into the tree, look for the overlap for this interval.
         */
        for (int i = 0; i < (int) (sizeof(input) / sizeof(Interval)); ++i) {
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
            cout << SequenceWriter<IntType, Interval>(it) << endl;

            cout << "Tree structure:" << endl;
            cout << HierarchyWriter<IntType, Interval>(it) << endl;
        }
    }
}

int main(int argc, char **argv) {
    interval_set_difference_Test();
    interval_set_difference_Test1();
    interval_set_intersect_Test();
    interval_set_intersect_Test1();
    interval_set_union_Test();
    interval_set_union_Test1();
    intervalTree_Test();
    demoOverlap();
	return 0;
}

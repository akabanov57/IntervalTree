/*
 * Interval.hpp
 *
 *  Created on: Jun 2, 2019
 *      Author: andrei
 */

#ifndef INTERVAL_HPP_
#define INTERVAL_HPP_

#include <iostream>
#include <exception>
#include <utility>
#include <algorithm>

struct Interval;
bool overlap(const Interval& i1, const Interval& i2);

/**
 * Half open interval.
 * see https://en.wikipedia.org/wiki/Interval_(mathematics)
 * see https://www.bowdoin.edu/~ltoma/teaching/cs231/spring14/Lectures/10-augmentedTrees/augtrees.pdf
 *
 */
struct Interval {
private:
    long _start;
    long _end;

    Interval(long _start_, long _end_) {
        _start = _start_;
        _end = _end_;
    }

public:

    Interval() {
        _start = 0L;
        _end = 0L;
    }

    static Interval valueOf(long _start_, long _end_) {
        using std::invalid_argument;
        if (_start_ < 0) {
            throw invalid_argument("_start_ < 0");
        }
        if (_end_ < 0) {
            throw invalid_argument("_end_ < 0");
        }
        if (_start_ > _end_) {
            throw invalid_argument("_start_ > _end_");
        }
        return Interval(_start_, _end_);
    }

    long start() const {
        return _start;
    }

    long end() const {
        return _end;
    }

    long length() const {
        return _end - _start;
    }

    bool contained(long point) const {
        return point >= this->start() && point < this->end();
    }

    bool isValid() const {
        return length() > 0;
    }

};

/**
 * The start i1 "is to the left" than the start i2.
 */
inline bool operator <(const Interval &i1, const Interval &i2) {
    return i1.start() < i2.start();
}

/**
 * The start i1 "is more to the right" than the start i2.
 */
inline bool operator >(const Interval &i1, const Interval &i2) {
    return i1.start() > i2.start();
}

inline bool operator ==(const Interval &i1, const Interval &i2) {
    return i1.start() == i2.start();
}

inline bool operator !=(const Interval &i1, const Interval &i2) {
    return i1.start() != i2.start();
}

/**
 *             i1
 *         |--------|
 *    i2
 * |------|
 *
 *      i1
 * |---------|
 *               i2
 *            |------|
 * returns negation "do not overlap".
 */
inline bool overlap(const Interval &i1, const Interval &i2) {
    return !(i1.start() >= i2.end() || i1.end() <= i2.start());
}

/**
 * adapted from https://github.com/childsish/interval
 *       i1
 *  |----------|
 * start      end
 *            i2
 *        |---------|
 *      start      end
 *
 *             i1
 *        |---------|
 *      start      end
 *       i2
 *  |----------|
 * start      end
 *
 *            i1
 *  |----------------------|
 * start                  end
 *              i2
 *        |-----------|
 *      start        end
 */
inline std::pair<Interval, Interval> set_difference(const Interval& i1, const Interval& i2) {
    if (!overlap(i1, i2)) {
        return std::pair<Interval, Interval>();
    }
    Interval left;
    Interval right;
    if (i1.start() < i2.start()) {
        left = Interval::valueOf(i1.start(), i2.start());
    }
    if (i1.end() > i2.end()) {
        right = Interval::valueOf(i2.end(), i1.end());
    }
    return std::pair<Interval, Interval>{left, right};
}

/**
 * See comments to set_difference.
 */
inline Interval set_intersect(const Interval& i1, const Interval& i2) {
    using std::max;
    using std::min;

    if (!overlap(i1, i2)) {
        return Interval();
    }
    return Interval::valueOf(max(i1.start(),i2.start()), min(i1.end(), i2.end()));
}

/**
 * See comments to set_difference.
 */
inline Interval set_union(const Interval& i1, const Interval& i2) {
    using std::max;
    using std::min;

    if (!overlap(i1, i2)) {
        return Interval();
    }
    return Interval::valueOf(min(i1.start(), i2.start()), max(i1.end(), i2.end()));
}

inline std::ostream& operator <<(std::ostream &out, const Interval &i) {
    out << "[" << i.start() << "," << i.end() << "[";
    return out;
}

#endif /* INTERVAL_HPP_ */

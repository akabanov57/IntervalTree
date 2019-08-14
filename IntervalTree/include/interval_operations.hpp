/*
 * interval_operations.hpp
 *
 *  Created on: Aug 14, 2019
 *      Author: andrei
 */

#ifndef INTERVAL_OPERATIONS_HPP_
#define INTERVAL_OPERATIONS_HPP_

#include <utility>
#include <algorithm>

/**
 * The start i1 "is to the left" than the start i2.
 */
template<typename Interval>
inline bool operator <(const Interval& i1, const Interval& i2) {
    return i1.start() < i2.start();
}

/**
 * The start i1 "is more to the right" than the start i2.
 */
template<typename Interval>
inline bool operator >(const Interval& i1, const Interval& i2) {
    return i1.start() > i2.start();
}

template<typename Interval>
inline bool operator ==(const Interval& i1, const Interval& i2) {
    return i1.start() == i2.start();
}

template<typename Interval>
inline bool operator !=(const Interval& i1, const Interval& i2) {
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
template<typename Interval>
inline bool overlap(const Interval& i1, const Interval& i2) {
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
template<typename Interval>
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
template<typename Interval>
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
template<typename Interval>
inline Interval set_union(const Interval& i1, const Interval& i2) {
    using std::max;
    using std::min;

    if (!overlap(i1, i2)) {
        return Interval();
    }
    return Interval::valueOf(min(i1.start(), i2.start()), max(i1.end(), i2.end()));
}

#endif /* INTERVAL_OPERATIONS_HPP_ */

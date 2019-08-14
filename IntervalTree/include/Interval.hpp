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

/**
 * Half open interval.
 * see https://en.wikipedia.org/wiki/Interval_(mathematics)
 * see https://www.bowdoin.edu/~ltoma/teaching/cs231/spring14/Lectures/10-augmentedTrees/augtrees.pdf
 *
 * Default Interval. The user can define his own Interval with the same interface. See interval_tree_test.cpp
 */
template<typename T>
struct IntervalT {
private:
    T _start;
    T _end;

    IntervalT(T _start_, T _end_) {
        _start = _start_;
        _end = _end_;
    }

public:

    IntervalT() {
        _start = static_cast<T>(0);
        _end = static_cast<T>(0);
    }

    static IntervalT<T> valueOf(T _start_, T _end_) {
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
        return IntervalT(_start_, _end_);
    }

    T start() const {
        return _start;
    }

    T end() const {
        return _end;
    }

    T length() const {
        return _end - _start;
    }

    bool contained(T point) const {
        return point >= this->start() && point < this->end();
    }

    bool isValid() const {
        return length() > 0;
    }

};

template<typename T>
inline std::ostream& operator <<(std::ostream &out, const IntervalT<T>& i) {
    out << "[" << i.start() << "," << i.end() << "[";
    return out;
}

#endif /* INTERVAL_HPP_ */

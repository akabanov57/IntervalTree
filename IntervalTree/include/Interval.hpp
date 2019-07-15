/*
 * Interval.hpp
 *
 *  Created on: Jun 2, 2019
 *      Author: andrei
 */

#ifndef INTERVAL_HPP_
#define INTERVAL_HPP_

/**
 * For demo only.
 */
#include <iostream>
#include <exception>
#include <utility>
#include <algorithm>

struct Interval;
bool overlap(const Interval& i1, const Interval& i2);

/**
 * see https://en.wikipedia.org/wiki/Interval_(mathematics)
 * see https://www.bowdoin.edu/~ltoma/teaching/cs231/spring14/Lectures/10-augmentedTrees/augtrees.pdf
 * can be renamed to "Extent", etc...
 */
struct Interval {
private:
	long _offset;
	long _length;
	Interval(long offset, long length) {
		_offset = offset;
		_length = length;
	}
public:
	Interval() {
		_offset = 0L;
		_length = 0L;
	}

	static Interval valueOf(long _offset_, long _length_) {
		using std::invalid_argument;
		if (_offset_ < 0) {
			throw invalid_argument("_offset_ < 0");
		}
		if (_length_ < 0) {
			throw invalid_argument("_length_ < 0");
		}
		return Interval(_offset_, _length_);
	}

	inline long start() const {
		return _offset;
	}

	inline Interval& start(long _start_) {
		_length -= _start_ - _offset;
		_offset = _start_;
		return *this;
	}

	inline long end() const {
		return _offset + _length;
	}

	inline Interval& end(long _end_) {
		_length = _end_ - _offset;
		return *this;
	}

	inline long offset() const {
		return _offset;
	}

	inline long length() const {
		return _length;
	}

	inline bool containded(long point) const {
		return point >= this->start() && point < this->end();
	}

	inline bool isValid() const {
		return this->_length > 0;
	}

	friend bool operator < (const Interval& i1, const Interval& i2);
	friend bool operator > (const Interval& i1, const Interval& i2);
	friend bool operator == (const Interval& i1, const Interval& i2);

	friend bool youngerThen(const Interval& i1, const Interval& i2);

	friend std::ostream& operator << (std::ostream& out, const Interval& i);
};

/**
 * The offset i1 "is to the left" than the offset i2.
 */
inline bool operator < (const Interval& i1, const Interval& i2) {
	return i1._offset < i2._offset;
}

/**
 * The offset i1 "is more to the right" than the offset i2.
 */
inline bool operator > (const Interval& i1, const Interval& i2) {
	return i1._offset > i2._offset;
}

/**
 * The offsets i1 and i2 are equal.
 */
inline bool operator == (const Interval& i1, const Interval& i2) {
	return i1._offset == i2._offset;
}

/**
 * returns negation "do not overlap".
 */
inline bool overlap(const Interval& i1, const Interval& i2) {
	return !((i1.start() >= i2.end()) || (i1.end() <= i2.start()));
}

inline std::ostream& operator << (std::ostream& out, const Interval& i) {
	out << "[" << i._offset << "," << i.end() << ")";
	return out;
}

#endif /* INTERVAL_HPP_ */

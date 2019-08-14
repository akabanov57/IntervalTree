/*
 * IntervalTree.hpp
 *
 *  Created on: May 28, 2019
 *      Author: andrei
 */

#ifndef INTERVALTREE_HPP_
#define INTERVALTREE_HPP_

#include <iostream>
#include <algorithm>
#include <set>
#include <cassert>

#include <Interval.hpp>

template<typename T, typename Interval>
class HierarchyWriter;

template<typename T, typename Interval>
class SequenceWriter;

/**
 * In computer science, an interval tree is a tree data structure to hold intervals.
 * Specifically, it allows one to efficiently find all intervals that overlap with
 * any given interval.
 *
 * see also https://en.wikipedia.org/wiki/Interval_tree
 *
 * see also
 * "Introduction to Algorithms"
 * Second Edition
 *
 * Thomas H. Cormen
 * Charles E. Leiserson
 * Ronald L. Rivest
 * Clifford Stein
 *
 */

template<typename T, typename Interval = IntervalT<T>>
class IntervalTree {
private:

    enum Color {
        BLACK, RED
    };

    /**
     * type that represents a node in the tree
     */
    class OrdinaryNode {
    private:
        Color color_;
        OrdinaryNode *parent_;
        OrdinaryNode *left_;
        OrdinaryNode *right_;
        Interval key_;
        /**
         * Node is augmented with maximal right endpoint in subtree rooted in x.
         */
        unsigned long max_;
        /**
         * Node is augmented with minimal left endpoint in subtree rooted in x.
         */
        unsigned long min_;

        OrdinaryNode() {
            color_ = BLACK;
            parent_ = nullptr;
            left_ = this;
            right_ = this;
            max_ = 0UL;
            min_ = 0UL;
        }
    public:
        /**
         * new OrdinaryNode must be RED.
         */
        OrdinaryNode(const Interval& key_, OrdinaryNode* parent) {
            color_ = RED;
            this->parent_ = parent;
            left_ = TNIL;
            right_ = TNIL;
            this->key_ = key_;
            max_ = this->key_.end();
            min_ = this->key_.start();
        }

        OrdinaryNode(const Interval& key_): OrdinaryNode(key_, nullptr) {}

        Color color() const {
            return color_;
        }
        void color(Color color_) {
            assert(left_ != this && right_ != this);
            this->color_ = color_;
        }
        OrdinaryNode* parent() const {
            return parent_;
        }
        void parent(OrdinaryNode *parent_) {
            this->parent_ = parent_;
        }
        OrdinaryNode* left() const {
            return left_;
        }
        void left(OrdinaryNode *left_) {
            assert(left_ != this && right_ != this);
            this->left_ = left_;
        }
        OrdinaryNode* right() const {
            return right_;
        }
        void right(OrdinaryNode *right_) {
            assert(left_ != this && right_ != this);
            this->right_ = right_;
        }
        const Interval& key() const {
            return key_;
        }
        void key(const Interval& key_) {
            assert(left_ != this && right_ != this);
            this->key_ = key_;
        }
        unsigned long max() const {
            return max_;
        }
        void max(unsigned long _max_) {
            assert(left_ != this && right_ != this);
            max_ = _max_;
        }
        unsigned long min() const {
            return min_;
        }
        void min(unsigned long _min_) {
            assert(left_ != this && right_ != this);
            min_ = _min_;
        }
        ~OrdinaryNode() {
            if (left_ != TNIL) {
                delete left_;
            }
            if (right_ != TNIL) {
                delete right_;
            }
        }
        friend class IntervalTree;
    };

    typedef OrdinaryNode* NodePtr;

private:
    NodePtr root_;

    static OrdinaryNode nilNode;
    static OrdinaryNode *const TNIL;

    static const Interval& search(const NodePtr node, const Interval& key);
    static const Interval& search(const NodePtr node, long offset);

    /**
     * Iterative implementation.
     * see https://www.bowdoin.edu/~ltoma/teaching/cs231/spring14/Lectures/10-augmentedTrees/augtrees.pdf
     */
    static void overlapSearch(const NodePtr _root_, const Interval& i, std::set<Interval>& res);

   /**
     * fix the rb tree modified by the delete operation
     */
    void fixDelete(NodePtr x);

    /**
     *  fix the red-black tree
     *  Possible violations:
     *    - the root is RED;
     *    - both k and k's parent are RED.
     *  The node pointed to by k is always red.
     */
    void fixInsert(NodePtr k);

    void rbTransplant(NodePtr u, NodePtr v) {
        if (u->parent() == nullptr) {
            root_ = v;
        } else if (u == u->parent()->left()) {
            u->parent()->left(v);
        } else {
            u->parent()->right(v);
        }
        v->parent(u->parent());
    }

    /**
     * remove the key from the tree, starting at root.
     *
     * see also https://doxygen.postgresql.org/rbtree_8c_source.html
     */
    bool remove(NodePtr root, const Interval& key);

    /**
     * max(x) = max(rightendpoint(x), max(left(x)), max(right(x)))
     *
     * see also https://www.bowdoin.edu/~ltoma/teaching/cs231/spring14/Lectures/10-augmentedTrees/augtrees.pdf
     */
    static unsigned long max(unsigned long end, NodePtr left, NodePtr right) {
        using std::max;
        if (left == TNIL && right == TNIL) {
            return end;
        } else if (left == TNIL) {
            return max(end, right->max());
        } else if (right == TNIL) {
            return max(end, left->max());
        } else {
            return max(end, max(left->max(), right->max()));
        }
    }

    /**
     * min(x) = min(leftendpoint(x), min(left(x)), min(right(x)))
     *
     * similar to max
     */
    static unsigned long min(unsigned long start, NodePtr left, NodePtr right) {
        using std::min;
        if (left == TNIL && right == TNIL) {
            return start;
        } else if (left == TNIL) {
            return min(start, right->min());
        } else if (right == TNIL) {
            return min(start, left->min());
        } else {
            return min(start, min(left->min(), right->min()));
        }
    }

    /**
     *  rotate left at node x
     *
     *  x's right child takes its place in the tree, and x becomes the left
     *  child of that node.
     *
     *  parent x    parent x
     *   |            |
     *   x            y
     *  / \          / \
	 * a   \        /   c
     *      y      x
     *     / \    / \
	 *    b   c  a   b
     */
    void rotateLeft(NodePtr x);

    /**
     *  rotate right at node x
     *
     *  x's left right child takes its place in the tree, and x becomes the right
     *  child of that node.
     *
     *   parent x  parent x
     *       |        |
     *       x        y
     *      / \      / \
	 *     /   c    a   \
	 *    y              x
     *   / \            / \
	 *  a   b          b   c
     */
    void rotateRight(NodePtr x);

    /**
     *  find the node with the minimum key
     */
    static NodePtr minimum(const NodePtr node);

    /**
     *  find the node with the maximum key
     */
    static NodePtr maximum(const NodePtr node);

    /**
     * find the successor of a given node.
     *
     * if the right subtree is not null, the successor is the leftmost node in the right subtree
     * else it is the lowest ancestor of x whose left child is also an ancestor of x.
     */
    static NodePtr successor(const NodePtr x);

    /**
     * find the predecessor of a given node.
     *
     * if the left subtree is not null, the predecessor is the rightmost node in the left subtree.
     */
    static NodePtr predecessor(const NodePtr x);

public:

    IntervalTree() {
        root_ = TNIL;
    }
    ~IntervalTree() {
        if (!empty()) {
            delete root_;
        }
    }

    bool empty() const {
        return root_ == TNIL;
    }

    void clear() {
        if (!empty()) {
            delete root_;
            root_ = TNIL;
        }
    }

    /**
     * search the tree for the key k and return the corresponding Interval
     * Return reference to valid interval if found and reference to not valid otherwise.
     * The client should check the interval by calling Interval::isValid ().
     * Semantic - "is there interval with offset like k.offset?"
     */
    const Interval& search(const Interval& k) const {
        return search(this->root_, k);
    }

    /**
     * Search the tree for the interval with given offset.
     * Return reference to valid interval if found and reference to not valid otherwise.
     * The client should check the interval by calling Interval::isValid ().
     * Semantic - is there interval with such offset?
     */
    const Interval& search(unsigned long offset) const {
        return search(this->root_, offset);
    }

    /**
     * Finds in the tree intervals overlapping with the given.
     * The best case (the fastest) - there is no such intervals.
     * The worst case (the slowest) - there are overlaps with all intervals.
     */
    void overlapSearch(const Interval& i, std::set<Interval>& res) const {
        overlapSearch(root_, i, res);
    }

    /**
     *  insert the key to the tree in its appropriate position and fix the tree
     */
    bool insert(const Interval& key);

    /**
     * delete the node from the tree
     */
    bool remove(const Interval& key) {
        return remove(this->root_, key);
    }

    friend class HierarchyWriter<T, Interval>;
    friend class SequenceWriter<T, Interval>;
};

/**
 * writes the tree structure to text file.
 */
template<typename T, typename Interval = IntervalT<T>>
class HierarchyWriter {
private:
    const IntervalTree<T, Interval>& tree;

    std::ostream& print(std::ostream& os, const typename IntervalTree<T, Interval>::NodePtr root, std::string indent, bool last) const;
public:
    HierarchyWriter(const IntervalTree<T, Interval>& t) : tree(t) {}

    std::ostream& print(std::ostream& os) const {
        return print(os, tree.root_, "", true);
    }
};

template<typename T, typename Interval = IntervalT<T>>
inline std::ostream& operator << (std::ostream& os, const HierarchyWriter<T, Interval>& prnt) {
   return prnt.print(os);
}

/**
 * writes a sequence of intervals to text file.
 */
template<typename T, typename Interval = IntervalT<T>>
class SequenceWriter {
private:
   const IntervalTree<T, Interval>& tree;

   std::ostream& print(std::ostream& os, const typename IntervalTree<T, Interval>::NodePtr root) const;
public:
   SequenceWriter(const IntervalTree<T, Interval>& t) : tree(t) {}

   std::ostream& print(std::ostream& os) const {
       return print(os, tree.root_);
   }
};

template<typename T, typename Interval = IntervalT<T>>
inline std::ostream& operator << (std::ostream& os, const SequenceWriter<T, Interval>& prnt) {
   return prnt.print(os);
}

#include "IntervalTree.cpp"

#endif /* INTERVALTREE_HPP_ */

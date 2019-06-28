/*
 * IntervalTree.hpp
 *
 *  Created on: May 28, 2019
 *      Author: andrei
 */

#ifndef INTERVALTREE_HPP_
#define INTERVALTREE_HPP_

#include <Interval.hpp>
#include <iostream>
#include <algorithm>
#include <set>

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
class IntervalTree {
private:

	enum Color {
		BLACK, RED
	};

	/**
	 * interface that represents a node in the tree
	 */
	class RBNode {
	public:
		virtual Color color() const = 0;
		virtual RBNode& color(Color color_) = 0;
		virtual RBNode* parent() const = 0;
		virtual RBNode& parent(RBNode* parent_) = 0;
		virtual RBNode* left() const = 0;
		virtual RBNode& left(RBNode* left_) = 0;
		virtual RBNode* right() const = 0;
		virtual RBNode& right(RBNode *right_) = 0;
		virtual const Interval& key() const = 0;
		virtual RBNode& key(const Interval& key_) = 0;
		virtual long max() const = 0;
		virtual RBNode& max(long _max_) = 0;
		virtual long min() const = 0;
		virtual RBNode& min(long _min_) = 0;
		virtual ~RBNode() {
		}
	};

	class OrdinaryNode: public RBNode {
	private:
		Color color_;
		RBNode *parent_;
		RBNode *left_;
		RBNode *right_;
		Interval key_;
		/**
		 * Node is augmented with maximal right endpoint in subtree rooted in x.
		 */
		long max_;
		/**
		 * Node is augmented with minimal left endpoint in subtree rooted in x.
		 */
		long min_;
	public:
		/**
		 * new OrdinaryNode must be RED.
		 */
		OrdinaryNode(const Interval& key_) {
			color_ = RED;
			parent_ = nullptr;
			left_ = TNIL;
			right_ = TNIL;
			this->key_ = key_;
			max_ = this->key_.end();
			min_ = this->key_.start();
		}
		Color color() const {
			return color_;
		}
		RBNode& color(Color color_) {
			this->color_ = color_;
			return *this;
		}
		RBNode* parent() const {
			return parent_;
		}
		RBNode& parent(RBNode* parent_) {
			this->parent_ = parent_;
			return *this;
		}
		RBNode* left() const {
			return left_;
		}
		RBNode& left(RBNode* left_) {
			this->left_ = left_;
			return *this;
		}
		RBNode* right() const {
			return right_;
		}
		RBNode& right(RBNode *right_) {
			this->right_ = right_;
			return *this;
		}
		const Interval& key() const {
			return key_;
		}
		RBNode& key(const Interval& key_) {
			this->key_ = key_;
			return *this;
		}
		long max() const {
			return max_;
		}
		RBNode& max(long _max_) {
			max_ = _max_;
			return *this;
		}
		long min() const {
			return min_;
		}
		RBNode& min(long _min_) {
			min_ = _min_;
			return *this;
		}
		~OrdinaryNode() {
			if (left_ != TNIL) {
				delete left_;
			}
			if (right_ != TNIL) {
				delete right_;
			}
		}

	};

	class LeafNode: public RBNode {
	private:
		/*
		 * we need parent when fixDelete.
		 */
		RBNode *parent_;
		Interval key_;
	public:
		LeafNode() {
			parent_ = nullptr;
		}
		Color color() const {
			return BLACK;
		}
		RBNode& color(Color _color_) {
			return *this;
		}
		RBNode* parent() const {
			return parent_;
		}
		RBNode& parent(RBNode *_parent_) {
			parent_ = _parent_;
			return *this;
		}
		RBNode* left() const {
			return nullptr;
		}
		RBNode& left(RBNode* _left_) {
			return *this;
		}
		RBNode* right() const {
			return nullptr;
		}
		RBNode& right(RBNode* _right_) {
			return *this;
		}
		const Interval& key() const {
			return key_;
		}
		RBNode& key(const Interval& _key_) {
			return *this;
		}
		long max() const {
			return -1L;
		}
		RBNode& max(long _max_) {
			return *this;
		}
		long min() const {
			return -1L;
		}
		RBNode& min(long _min_) {
			return *this;
		}
		~LeafNode() {
		}
	};

	typedef RBNode *NodePtr;

	NodePtr root_;

	static LeafNode nilNode;
	static RBNode * const TNIL;

	NodePtr search(NodePtr node, Interval key) {
		if (node == TNIL || key == node->key()) {
			return node;
		}

		if (key < node->key()) {
			return search(node->left(), key);
		}
		return search(node->right(), key);
	}

	/**
	 * Recursion.
	 * see https://www.bowdoin.edu/~ltoma/teaching/cs231/spring14/Lectures/10-augmentedTrees/augtrees.pdf
	 */
	void overlapSearch(NodePtr _root_, const Interval& i, std::set<Interval>& res);

	void prints(NodePtr _root_) {
		using std::cout;
		if (_root_ == TNIL) {
			return;
		}
		prints(_root_->left());
		cout << _root_->key() << " ";
		prints(_root_->right());
	}

	/**
	 * fix the rb tree modified by the delete operation
	 */
	void fixDelete(NodePtr x);

	/**
	 *  fix the red-black tree
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
	void remove(NodePtr root, const Interval& key);

	/**
	 * print the tree structure on the screen
	 */
	void printth(NodePtr root, std::string indent, bool last) {
		using std::cout;
		using std::endl;
		if (root != TNIL) {
			cout << indent;
			if (last) {
				cout << "R----";
				indent += "     ";
			} else {
				cout << "L----";
				indent += "|    ";
			}

			cout << "{key:" << root->key() << ", max:" << root->max()<< ", min:" << root->min() << "}" << "(" << (root->color() == RED ? "RED" : "BLACK") << ")" << endl;
			printth(root->left(), indent, false);
			printth(root->right(), indent, true);
		}
	}

	/**
	 * max(x) = max(rightendpoint(x), max(left(x)), max(right(x)))
	 *
	 * see also https://www.bowdoin.edu/~ltoma/teaching/cs231/spring14/Lectures/10-augmentedTrees/augtrees.pdf
	 */
	long max(long end, NodePtr left, NodePtr right) {
		using std::max;
		if (left == TNIL && right == TNIL) {
			return end;
		} else if (left == TNIL) {
			return max(end,right->max());
		} else if (right == TNIL) {
			return max(end,left->max());
		} else {
			return max(end,max(left->max(),right->max()));
		}
	}

	/**
	 * min(x) = min(leftendpoint(x), min(left(x)), min(right(x)))
	 *
	 * similar to max
	 */
	long min(long start, NodePtr left, NodePtr right) {
		using std::min;
		if (left == TNIL && right == TNIL) {
			return start;
		} else if (left == TNIL) {
			return min(start, right->min());
		} else if (right == TNIL) {
			return min(start, left->min());
		} else {
			return min(start, min(left->min(),right->min()));
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
	NodePtr minimum(NodePtr node);

	/**
	 *  find the node with the maximum key
	 */
	NodePtr maximum(NodePtr node);

public:

	IntervalTree() {
		root_ = TNIL;
	}
	~IntervalTree() {
		if (root_ != TNIL) {
			delete root_;
		}
	}

	void clear() {
		if (root_ != TNIL) {
			delete root_;
			root_ = TNIL;
		}
	}

	/**
	 * search the tree for the key k and return the corresponding node
	 */
	NodePtr search(Interval k) {
		return search(this->root_, k);
	}

	void overlapSearch(const Interval i, std::set<Interval>& res) {
		overlapSearch(root_, i, res);
	}

	/**
	 * find the successor of a given node.
	 *
	 * if the right subtree is not null, the successor is the leftmost node in the right subtree
	 * else it is the lowest ancestor of x whose left child is also an ancestor of x.
	 */
	NodePtr successor(NodePtr x);

	/**
	 * find the predecessor of a given node.
	 *
	 * if the left subtree is not null, the predecessor is the rightmost node in the, left subtree.
	 */
	NodePtr predecessor(NodePtr x);

	/**
	 *  insert the key to the tree in its appropriate position and fix the tree
	 */
	void insert(const Interval& key);

	/**
	 * delete the node from the tree
	 */
	void remove(const Interval& key) {
		remove(this->root_, key);
	}

	/**
	 * print the tree structure on the screen
	 */
	void printt() {
		using std::cout;
		using std::endl;
		if (root_ != TNIL) {
			printth(this->root_, "", true);
		}
		cout << endl;
	}

	void prints() {
		using std::cout;
		using std::endl;
		if (root_ != TNIL) {
			prints(root_);
		}
		cout << endl;
	}

};

#endif /* INTERVALTREE_HPP_ */

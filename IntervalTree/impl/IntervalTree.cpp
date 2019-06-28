#include <iostream>

#include <IntervalTree.hpp>

IntervalTree::LeafNode IntervalTree::nilNode;
IntervalTree::RBNode *const IntervalTree::TNIL = &IntervalTree::nilNode;

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
void IntervalTree::rotateLeft(NodePtr x) {

	NodePtr y = x->right();

     /* establish x->right link */
     x->right(y->left());
     if (x->right() != TNIL) {
    	 x->right()->parent(x);
     }

     /* establish y->parent link */
     if (y != TNIL) {
         y->parent(x->parent());
     }

     /**
      * establish x->parent link
      *
      * if x is root, than y become root.
      */
     if (x->parent() != nullptr) {
         if (x == x->parent()->left()) {
             x->parent()->left(y);
         } else {
             x->parent()->right(y);
         }
     } else {
         root_ = y;
     }

     /* link x and y */
     y->left(x);
     if (x != TNIL)
         x->parent(y);

     /**
      * recalculate augmentation.
      */
     for (NodePtr z = x; z != nullptr; z = z->parent()) {
	     z->max(max(z->key().end(),z->left(),z->right()));
	     z->min(min(z->key().start(),z->left(),z->right()));
     }

}

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
void IntervalTree::rotateRight(NodePtr x) {

	NodePtr y = x->left();

     /* establish x->left link */
     x->left(y->right());
     if (x->left() != TNIL) {
    	 x->left()->parent(x);
     }

     /* establish y->parent link */
     if (y != TNIL) {
         y->parent(x->parent());
     }

     /**
      * establish x->parent link
      *
      * if x is root, than y become root.
      */
     if (x->parent() != nullptr) {
         if (x == x->parent()->right()) {
             x->parent()->right(y);
         } else {
             x->parent()->left(y);
         }
     }
     else {
         root_ = y;
     }

     /* link x and y */
     y->right(x);
     if (y->right() != TNIL) {
         y->right()->parent(y);
     }

     /**
      * recalculate augmentation.
      */
     for (NodePtr z = x; z != nullptr; z = z->parent()) {
	     z->max(max(z->key().end(),z->left(),z->right()));
	     z->min(min(z->key().start(),z->left(),z->right()));
     }

}

void IntervalTree::fixInsert(NodePtr k) {
	NodePtr u;
	while (k->parent()->color() == RED) {
		if (k->parent() == k->parent()->parent()->right()) {
			u = k->parent()->parent()->left(); // uncle
			if (u->color() == RED) {
				// case 3.1
				u->color(BLACK);
				k->parent()->color(BLACK);
				k->parent()->parent()->color(RED);
				k = k->parent()->parent();
			} else {
				if (k == k->parent()->left()) {
					// case 3.2.2
					k = k->parent();
					rotateRight(k);
				}
				// case 3.2.1
				k->parent()->color(BLACK);
				k->parent()->parent()->color(RED);
				rotateLeft(k->parent()->parent());
			}
		} else {
			u = k->parent()->parent()->right(); // uncle

			if (u->color() == RED) {
				// mirror case 3.1
				u->color(BLACK);
				k->parent()->color(BLACK);
				k->parent()->parent()->color(RED);
				k = k->parent()->parent();
			} else {
				if (k == k->parent()->right()) {
					// mirror case 3.2.2
					k = k->parent();
					rotateLeft(k);
				}
				// mirror case 3.2.1
				k->parent()->color(BLACK);
				k->parent()->parent()->color(RED);
				rotateRight(k->parent()->parent());
			}
		}
		if (k == root_) {
			break;
		}
	}
	root_->color(BLACK);
}

void IntervalTree::fixDelete(NodePtr x) {
	NodePtr s;
	while (x != root_ && x->color() == BLACK) {
		if (x == x->parent()->left()) {
			s = x->parent()->right();
			if (s->color() == RED) {
				// case 3.1
				s->color(BLACK);
				x->parent()->color(RED);
				rotateLeft(x->parent());
				s = x->parent()->right();
			}

			if (s->left()->color() == BLACK && s->right()->color() == BLACK) {
				// case 3.2
				s->color(RED);
				x = x->parent();
			} else {
				if (s->right()->color() == BLACK) {
					// case 3.3
					s->left()->color(BLACK);
					s->color(RED);
					rotateRight(s);
					s = x->parent()->right();
				}

				// case 3.4
				s->color(x->parent()->color());
				x->parent()->color(BLACK);
				s->right()->color(BLACK);
				rotateLeft(x->parent());
				x = root_;
			}
		} else {
			s = x->parent()->left();
			if (s->color() == RED) {
				// case 3.1
				s->color(BLACK);
				x->parent()->color(RED);
				rotateRight(x->parent());
				s = x->parent()->left();
			}

			if (s->right()->color() == BLACK && s->right()->color() == BLACK) {
				// case 3.2
				s->color(RED);
				x = x->parent();
			} else {
				if (s->left()->color() == BLACK) {
					// case 3.3
					s->right()->color(BLACK);
					s->color(RED);
					rotateLeft(s);
					s = x->parent()->left();
				}

				// case 3.4
				s->color(x->parent()->color());
				x->parent()->color(BLACK);
				s->left()->color(BLACK);
				rotateRight(x->parent());
				x = root_;
			}
		}
	}
	x->color(BLACK);
}

/**
 * remove the key from the tree, starting at root.
 */
void IntervalTree::remove(NodePtr root, const Interval& key) {
	/*
	 * the cursor should point to the node to be deleted.
	 */
	NodePtr cursor = root;
	while (cursor != TNIL){
		if (key < cursor->key()) {
			cursor = cursor->left();
		} else if (key > cursor->key()){
			cursor = cursor->right();
		} else {
			break;
		}
	}

	if (cursor == TNIL) {
//		cout << "Couldn't find key in the tree" << endl;
		return;
	}

    /*
     * y points to a node that will actually be removed from the tree. This will
     * be cursor if cursor has fewer than two children, or the tree successor of
     * cursor otherwise.
     */
	NodePtr y;

	/*
	 * x points to a child of y.
	 */
	NodePtr x;

	if (cursor->left() == TNIL || cursor->right() == TNIL) {
		y = cursor;
	} else {
		y = minimum(cursor->right());
	}

	/**
	 * x can be equal TNIL if y->left() == TNIL && y->right() == TNIL.
	 */
	if (y->left() != TNIL) {
		x = y->left();
	} else {
		x = y->right();
	}

	/**
	 * remove y from the tree.
	 */
	x->parent(y->parent());
	if (y->parent() != nullptr) {
		if (y == y->parent()->left()) {
			y->parent()->left(x);
		} else {
			y->parent()->right(x);
		}
	} else {
		root_ = x;
	}

    /*
     * If we removed the tree successor of cursor rather than cursor itself, then move
     * the data for the removed node to the one we were supposed to remove.
     */
	if (y != cursor) {
		cursor->key(y->key());
	}

	/**
	 * recalculate augmentation.
	 */
	for (NodePtr z = y->parent(); z != nullptr; z = z->parent()) {
		z->max(max(z->key().end(),z->left(),z->right()));
		z->min(min(z->key().start(),z->left(),z->right()));
	}

    /*
     * Removing a black node might make some paths from root to leaf contain
     * fewer black nodes than others, or it might make two red nodes adjacent.
     */
	if (y->color() == BLACK) {
		fixDelete(x);
	}

	/*
	 * protect subtrees from deletion
	 */
	y->left(nullptr);
	y->right(nullptr);
	/**
	 * delete node from memory.
	 */
	delete y;
}

/**
 * Ordinary Binary Search Insertion
 */
void IntervalTree::insert(const Interval& key) {
	NodePtr node = new OrdinaryNode(key);

	/**
	 *  y is parent of x
	 */
	NodePtr y = nullptr;
	NodePtr x = this->root_;

	while (x != TNIL) {
		y = x;
		if (node->key() < x->key()) {
			x = x->left();
		} else {
			x = x->right();
		}
	}

	/**
	 * Insert node in the tree.
	 */
	node->parent(y);
	if (y == nullptr) {
		root_ = node;
	} else if (node->key() < y->key()) {
		y->left(node);
	} else {
		y->right(node);
	}

	/**
	 * recalculate augmentation.
	 */
	for (NodePtr z = node->parent(); z != nullptr; z = z->parent()) {
		z->max(max(z->key().end(),z->left(),z->right()));
		z->min(min(z->key().start(),z->left(),z->right()));
	}

	// if new node is a root node, simply return
	if (node->parent() == nullptr){
		node->color(BLACK);
		return;
	}

	// if the grandparent is null, simply return
	if (node->parent()->parent() == nullptr) {
		return;
	}

	// Fix the tree
	fixInsert(node);
}

IntervalTree::NodePtr IntervalTree::minimum(IntervalTree::NodePtr node) {
	while (node->left() != TNIL) {
		node = node->left();
	}
	return node;
}

IntervalTree::NodePtr IntervalTree::maximum(IntervalTree::NodePtr node) {
	while (node->right() != TNIL) {
		node = node->right();
	}
	return node;
}

/**
 * if the right subtree is not null, the successor is the leftmost node in the right subtree
 * else it is the lowest ancestor of x whose left child is also an ancestor of x.
 */
IntervalTree::NodePtr IntervalTree::successor(IntervalTree::NodePtr x) {
	if (x->right() != TNIL) {
		return minimum(x->right());
	}

	NodePtr y = x->parent();
	while (y != TNIL && x == y->right()) {
		x = y;
		y = y->parent();
	}
	return y;
}

/**
 * if the left subtree is not null, the predecessor is the rightmost node in the, left subtree.
 */
IntervalTree::NodePtr IntervalTree::predecessor(IntervalTree::NodePtr x) {
	if (x->left() != TNIL) {
		return maximum(x->left());
	}

	NodePtr y = x->parent();
	while (y != TNIL && x == y->left()) {
		x = y;
		y = y->parent();
	}

	return y;
}

void IntervalTree::overlapSearch(NodePtr _root_, const Interval& i, std::set<Interval>& res) {
	if (_root_ == TNIL) {
		return;
	}
	if (overlap(_root_->key(), i)) {
		res.insert(_root_->key());
	}

	/**
	 *          | max
	 *  start |----------| end
	 */
	if (_root_->left() != TNIL && _root_->left()->max() >= i.start()) {
		overlapSearch(_root_->left(), i, res);
	} else {
		overlapSearch(_root_->right(), i, res);
	}

	/**
	 *                  | min
	 *  start |------------| end
	 */
	if (_root_->right() != TNIL && _root_->right()->min() <= i.end()) {
		overlapSearch(_root_->right(), i, res);
	} else {
		overlapSearch(_root_->left(), i, res);
	}
}


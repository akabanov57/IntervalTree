
#include <iostream>
#include <stack>

#include <IntervalTree.hpp>

IntervalTree::OrdinaryNode IntervalTree::nilNode;
IntervalTree::OrdinaryNode *const IntervalTree::TNIL = &IntervalTree::nilNode;

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
        z->max(max(z->key().end(), z->left(), z->right()));
        z->min(min(z->key().start(), z->left(), z->right()));
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
        z->max(max(z->key().end(), z->left(), z->right()));
        z->min(min(z->key().start(), z->left(), z->right()));
    }

}

void IntervalTree::fixInsert(NodePtr k) {
    NodePtr u(nullptr);
    while (k != root_ && k->parent()->color() == RED) {
        if (k->parent() == k->parent()->parent()->right()) { // k's parent is right child
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
        } else { // k's parent is left child
            u = k->parent()->parent()->right(); // uncle

            if (u->color() == RED) {
                // mirror case 3.1
                u->color(BLACK);
                k->parent()->color(BLACK);
                k->parent()->parent()->color(RED);
                k = k->parent()->parent();
            } else {
                if (k == k->parent()->right()) { // k is right child
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
    }
    root_->color(BLACK);
}

void IntervalTree::fixDelete(NodePtr x) {
    while (x != root_ && x->color() == BLACK) {
        if (x == x->parent()->left()) {
            NodePtr    w = x->parent()->right();

            if (w->color() == RED) {
                w->color(BLACK);
                x->parent()->color(RED);
                rotateLeft(x->parent());
                w = x->parent()->right();
            }

            if (w->left()->color() == BLACK && w->right()->color() == BLACK) {
                w->color(RED);
                x = x->parent();
            } else {
                if (w->right()->color() == BLACK) {
                    w->left()->color(BLACK);
                    w->color(RED);
                    rotateRight(w);
                    w = x->parent()->right();
                }
                w->color(x->parent()->color());
                x->parent()->color(BLACK);
                w->right()->color(BLACK);
                rotateLeft(x->parent());
                x = root_;  /* Arrange for loop to terminate. */
            }
        } else {
            NodePtr    w = x->parent()->left();
            if (w->color() == RED) {
                w->color(BLACK);
                x->parent()->color(RED);
                rotateRight(x->parent());
                w = x->parent()->left();
            }
            if (w->right()->color() == BLACK && w->left()->color() == BLACK) {
                w->color(RED);
                x = x->parent();
            }
            else {
                if (w->left()->color() == BLACK) {
                    w->right()->color(BLACK);
                    w->color(RED);
                    rotateLeft(w);
                    w = x->parent()->left();
                }
                w->color(x->parent()->color());
                x->parent()->color(BLACK);
                w->left()->color(BLACK);
                rotateRight(x->parent());
                x = root_;  /* Arrange for loop to terminate. */
            }
        }
    }
    if (x != TNIL) {
        x->color(BLACK);
    }
}

/**
 * remove the key from the tree, starting at root.
 */
bool IntervalTree::remove(NodePtr root, const Interval &key) {
    /*
     * the cursor should point to the node to be deleted.
     */
    NodePtr cursor = root;
    while (cursor != TNIL) {
        if (key < cursor->key()) {
            cursor = cursor->left();
        } else if (key > cursor->key()) {
            cursor = cursor->right();
        } else {
            break;
        }
    }

    if (cursor == TNIL) {
        return false;
    }

    /*
     * y points to a node that will actually be removed from the tree. This will
     * be cursor if cursor has fewer than two children, or the minimum of the
     * right subtree of the cursor otherwise.
     */
    NodePtr y(nullptr);

    if (cursor->left() == TNIL || cursor->right() == TNIL) {
        y = cursor;
    } else {
        y = cursor->right();
        while (y->left() != TNIL) {
            y = y->left();
        }
    }

    /*
     * x points to a child of y.
     */
    NodePtr x(nullptr);

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
        z->max(max(z->key().end(), z->left(), z->right()));
        z->min(min(z->key().start(), z->left(), z->right()));
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
    return true;
}

/**
 * Ordinary Binary Search Insertion
 */
bool IntervalTree::insert(const Interval& key) {
    NodePtr parent = nullptr;
    NodePtr current = this->root_;

    while (current != TNIL) {
        parent = current;
        if (key < current->key()) {
            current = current->left();
        } else if (key > current->key()) {
            current = current->right();
        } else {
            return false;
        }
    }

    NodePtr node = new OrdinaryNode(key, parent);
    /**
     * Insert node in the tree.
     */
    if (node->parent() == nullptr) {
        root_ = node;
    } else if (node->key() < parent->key()) {
        parent->left(node);
    } else {
        parent->right(node);
    }

    /**
     * recalculate augmentation.
     */
    for (NodePtr z = node->parent(); z != nullptr; z = z->parent()) {
        z->max(max(z->key().end(), z->left(), z->right()));
        z->min(min(z->key().start(), z->left(), z->right()));
    }

    /**
     *  Fix the tree
     *  node is RED
     */
    fixInsert(node);
    return true;
}

IntervalTree::NodePtr IntervalTree::minimum(const IntervalTree::NodePtr node) {
    NodePtr found = node;
    while (found->left() != TNIL) {
        found = found->left();
    }
    return found;
}

IntervalTree::NodePtr IntervalTree::maximum(const IntervalTree::NodePtr node) {
    NodePtr found = node;
    while (found->right() != TNIL) {
        found = found->right();
    }
    return found;
}

/**
 * if the right subtree is not null, the successor is the leftmost node in the right subtree
 * else it is the lowest ancestor of x whose left child is also an ancestor of x.
 */
IntervalTree::NodePtr IntervalTree::successor(const IntervalTree::NodePtr x) {
    /**
     * if right subtree is not empty.
     */
    if (x->right() != TNIL) {
        return minimum(x->right());
    }
    /**
     * parent of x
     */
    NodePtr parent = x->parent();
    /**
     * right child of parent
     */
    NodePtr rightChild = nullptr;
    /**
     * while parent is not root of tree and rightChild is really right child of parent.
     */
    while (parent != TNIL && rightChild == parent->right()) {
        rightChild = parent;
        parent = parent->parent();
    }
    return parent;
}

/**
 * if the left subtree is not null, the predecessor is the rightmost node in the, left subtree
 * else it is the lowest ancestor of x whose right child is also an ancestor of x.
 */
IntervalTree::NodePtr IntervalTree::predecessor(const IntervalTree::NodePtr x) {
    /**
     * if left subtree is not empty.
     */
    if (x->left() != TNIL) {
        return maximum(x->left());
    }
    /**
     * parent of x.
     */
    NodePtr parent = x->parent();
    /**
     * leftChild of parent.
     */
    NodePtr leftChild = nullptr;
    /**
     * while parent is not root of tree and leftChild is really left child of parent.
     */
    while (parent != TNIL && leftChild == parent->left()) {
        leftChild = parent;
        parent = parent->parent();
    }
    return parent;
}

void IntervalTree::overlapSearch(const NodePtr _root_, const Interval& i, std::set<Interval>& res) {
    using std::stack;

    NodePtr curr = _root_;
    if (curr == TNIL) {
        return;
    }
    std::stack<NodePtr> s;
    s.push(curr);
    while (!s.empty()) {
        curr = s.top();
        s.pop();
        if (overlap(curr->key(), i)) {
            res.insert(curr->key());
        }
        /**
         *          | max
         *  start |----------| end
         */
        if (curr->left() != TNIL && curr->left()->max() > i.start()) {
            s.push(curr->left());
        }
        /**
         *                  | min
         *  start |------------| end
         */
        if (curr->right() != TNIL && curr->right()->min() < i.end()) {
            s.push(curr->right());
        }
    }



//    if (_root_ == TNIL) {
//        return;
//    }
//    if (overlap(_root_->key(), i)) {
//        res.insert(_root_->key());
//    }
//
//    /**
//     *          | max
//     *  start |----------| end
//     */
//    if (_root_->left() != TNIL && _root_->left()->max() > i.start()) {
//        overlapSearch(_root_->left(), i, res);
//    }
//
//    /**
//     *                  | min
//     *  start |------------| end
//     */
//    if (_root_->right() != TNIL && _root_->right()->min() < i.end()) {
//        overlapSearch(_root_->right(), i, res);
//    }

}

const Interval& IntervalTree::search(const NodePtr node, const Interval& key) {
    NodePtr found = node;
    while (found != TNIL && found->key() != key) {
        if (key < found->key()) {
            found = found->left();
        } else {
            found = found->right();
        }
    }
    return found->key();
}

const Interval& IntervalTree::search(const NodePtr node, long offset) {
    NodePtr found = node;
    while (found != TNIL && found->key().start() != offset) {
        if (offset < found->key().start()) {
            found = found->left();
        } else {
            found = found->right();
        }
    }
    return found->key();
}

std::ostream& HierarchyWriter::print(std::ostream& os,const IntervalTree::NodePtr root, std::string indent, bool last) const {
    using std::endl;
    if (root != IntervalTree::TNIL) {
        os << indent;
        if (last) {
            os << "R----";
            indent += "     ";
        } else {
            os << "L----";
            indent += "|    ";
        }

        os << "{key:" << root->key() << ", max:" << root->max() << ", min:" << root->min() << "}" << "("
             << (root->color() == IntervalTree::RED ? "RED" : "BLACK") << ")" << endl;
        print(os, root->left(), indent, false);
        print(os, root->right(), indent, true);
    }
    return os;
}

std::ostream& SequenceWriter::print(std::ostream& os, const IntervalTree::NodePtr root) const {
     if (root == IntervalTree::TNIL) {
         return os;
     }
     print(os, root->left());
     os << root->key() << " ";
     print(os, root->right());
     return os;
 }


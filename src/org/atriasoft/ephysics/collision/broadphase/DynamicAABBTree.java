package org.atriasoft.ephysics.collision.broadphase;

import java.lang.ref.WeakReference;
import java.util.Stack;

import org.atriasoft.ephysics.Configuration;
import org.atriasoft.ephysics.collision.shapes.AABB;
import org.atriasoft.ephysics.internal.Log;
import org.atriasoft.ephysics.mathematics.Ray;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector3f;

/**
 *  It implements a dynamic AABB tree that is used for broad-phase
 * collision detection. This data structure is inspired by Nathanael Presson's
 * dynamic tree implementation in BulletPhysics. The following implementation is
 * based on the one from Erin Catto in Box2D as described in the book
 * "Introduction to Game Physics with Box2D" by Ian Parberry.
 */
public class DynamicAABBTree {
	
	private DTree rootNode; //!< Pointer to the memory location of the nodes of the tree
	
	private final float extraAABBGap; //!< Extra AABB Gap used to allow the collision shape to move a little bit without triggering a large modification of the tree which can be costly
	/// Allocate and return a node to use in the tree
	
	/// Constructor
	public DynamicAABBTree() {
		this(0.0f);
	}
	
	public DynamicAABBTree(final float extraAABBGap) {
		this.extraAABBGap = extraAABBGap;
		init();
	}
	
	/// Add an object into the tree (where node data are two integers)
	public DTree addObject(final AABB aabb, final int data1, final int data2) {
		final DTreeLeafInt node = new DTreeLeafInt(data1, data2);
		addObjectInternal(aabb, node);
		return node;
	}
	
	/// Add an object into the tree (where node data is a pointer)
	public DTree addObject(final AABB aabb, final Object data) {
		final DTreeLeafData node = new DTreeLeafData(data);
		addObjectInternal(aabb, node);
		return node;
	}
	
	/// Internally add an object into the tree
	private void addObjectInternal(final AABB aabb, final DTree leafNode) {
		// Create the fat aabb to use in the tree
		leafNode.aabb.setMin(aabb.getMin().lessNew(this.extraAABBGap));
		leafNode.aabb.setMax(aabb.getMax().addNew(this.extraAABBGap));
		// Set the height of the node in the tree
		leafNode.height = 0;
		// Insert the new leaf node in the tree
		insertLeafNode(leafNode);
		assert (leafNode.isLeaf());
	}
	
	/// Balance the sub-tree of a given node using left or right rotations.
	private DTree balanceSubTreeAtNode(final DTree _node) {
		assert (_node != null);
		// If the node is a leaf or the height of A's sub-tree is less than 2
		if (_node.isLeaf() || _node.height < 2) {
			// Do not perform any rotation
			return _node;
		}
		final DTreeNode nodeA = (DTreeNode) _node;
		// Get the two children nodes
		final DTree nodeB = nodeA.children_left;
		final DTree nodeC = nodeA.children_right;
		// Compute the factor of the left and right sub-trees
		final int balanceFactor = nodeC.height - nodeB.height;
		// If the right node C is 2 higher than left node B
		if (balanceFactor > 1) {
			assert (!nodeC.isLeaf());
			final DTreeNode nodeCTree = (DTreeNode) nodeC;
			final DTree nodeF = nodeCTree.children_left;
			final DTree nodeG = nodeCTree.children_right;
			nodeCTree.children_left = _node;
			nodeCTree.parent = nodeA.parent;
			nodeA.parent = new WeakReference<DTree>(nodeC);
			if (nodeC.parent != null) {
				final DTreeNode nodeCParent = (DTreeNode) nodeC.parent.get();
				if (nodeCParent.children_left == _node) {
					nodeCParent.children_left = nodeC;
				} else {
					assert (nodeCParent.children_right == _node);
					nodeCParent.children_right = nodeC;
				}
			} else {
				this.rootNode = nodeC;
			}
			assert (!nodeC.isLeaf());
			assert (!nodeA.isLeaf());
			// If the right node C was higher than left node B because of the F node
			if (nodeF.height > nodeG.height) {
				nodeCTree.children_right = nodeF;
				nodeA.children_right = nodeG;
				nodeG.parent = new WeakReference<DTree>(_node);
				// Recompute the AABB of node A and C
				nodeA.aabb.mergeTwoAABBs(nodeB.aabb, nodeG.aabb);
				nodeC.aabb.mergeTwoAABBs(nodeA.aabb, nodeF.aabb);
				// Recompute the height of node A and C
				nodeA.height = FMath.max(nodeB.height, nodeG.height) + 1;
				nodeC.height = FMath.max(nodeA.height, nodeF.height) + 1;
				assert (nodeA.height > 0);
				assert (nodeC.height > 0);
			} else {
				// If the right node C was higher than left node B because of node G
				nodeCTree.children_right = nodeG;
				nodeA.children_right = nodeF;
				nodeF.parent = new WeakReference<DTree>(_node);
				// Recompute the AABB of node A and C
				nodeA.aabb.mergeTwoAABBs(nodeB.aabb, nodeF.aabb);
				nodeC.aabb.mergeTwoAABBs(nodeA.aabb, nodeG.aabb);
				// Recompute the height of node A and C
				nodeA.height = FMath.max(nodeB.height, nodeF.height) + 1;
				nodeC.height = FMath.max(nodeA.height, nodeG.height) + 1;
				assert (nodeA.height > 0);
				assert (nodeC.height > 0);
			}
			// Return the new root of the sub-tree
			return nodeC;
		}
		// If the left node B is 2 higher than right node C
		if (balanceFactor < -1) {
			assert (!nodeB.isLeaf());
			final DTreeNode nodeBTree = (DTreeNode) nodeB;
			final DTree nodeF = nodeBTree.children_left;
			final DTree nodeG = nodeBTree.children_right;
			nodeBTree.children_left = _node;
			nodeB.parent = nodeA.parent;
			nodeA.parent = new WeakReference<DTree>(nodeB);
			if (nodeB.parent != null) {
				final DTreeNode nodeBParent = (DTreeNode) nodeB.parent.get();
				if (nodeBParent.children_left == _node) {
					nodeBParent.children_left = nodeB;
				} else {
					assert (nodeBParent.children_right == _node);
					nodeBParent.children_right = nodeB;
				}
			} else {
				this.rootNode = nodeB;
			}
			assert (!nodeB.isLeaf());
			assert (!nodeA.isLeaf());
			// If the left node B was higher than right node C because of the F node
			if (nodeF.height > nodeG.height) {
				nodeBTree.children_right = nodeF;
				nodeA.children_left = nodeG;
				nodeG.parent = new WeakReference<DTree>(_node);
				// Recompute the AABB of node A and B
				nodeA.aabb.mergeTwoAABBs(nodeC.aabb, nodeG.aabb);
				nodeB.aabb.mergeTwoAABBs(nodeA.aabb, nodeF.aabb);
				// Recompute the height of node A and B
				nodeA.height = FMath.max(nodeC.height, nodeG.height) + 1;
				nodeB.height = FMath.max(nodeA.height, nodeF.height) + 1;
				assert (nodeA.height > 0);
				assert (nodeB.height > 0);
			} else {
				// If the left node B was higher than right node C because of node G
				nodeBTree.children_right = nodeG;
				nodeA.children_left = nodeF;
				nodeF.parent = new WeakReference<DTree>(_node);
				// Recompute the AABB of node A and B
				nodeA.aabb.mergeTwoAABBs(nodeC.aabb, nodeF.aabb);
				nodeB.aabb.mergeTwoAABBs(nodeA.aabb, nodeG.aabb);
				// Recompute the height of node A and B
				nodeA.height = FMath.max(nodeC.height, nodeF.height) + 1;
				nodeB.height = FMath.max(nodeA.height, nodeG.height) + 1;
				assert (nodeA.height > 0);
				assert (nodeB.height > 0);
			}
			// Return the new root of the sub-tree
			return nodeB;
		}
		// If the sub-tree is balanced, return the current root node
		return _node;
	}
	
	/// Compute the height of the tree
	public int computeHeight() {
		return computeHeight(this.rootNode);
	}
	
	/// Compute the height of a given node in the tree
	private int computeHeight(final DTree _node) {
		// If the node is a leaf, its height is zero
		if (_node.isLeaf()) {
			return 0;
		}
		final DTreeNode nodeTree = (DTreeNode) _node;
		
		// Compute the height of the left and right sub-tree
		final int leftHeight = computeHeight(nodeTree.children_left);
		final int rightHeight = computeHeight(nodeTree.children_right);
		// Return the height of the node
		return 1 + Math.max(leftHeight, rightHeight);
	}
	
	/// Return the fat AABB corresponding to a given node ID
	public AABB getFatAABB(final DTree node) {
		return node.aabb;
	}
	
	public int getNodeDataInt_0(final DTree node) {
		assert (node.isLeaf());
		return ((DTreeLeafInt) node).dataInt_0;
	}
	
	public int getNodeDataInt_1(final DTree node) {
		assert (node.isLeaf());
		return ((DTreeLeafInt) node).dataInt_1;
	}
	
	/// Return the data pointer of a given leaf node of the tree
	public Object getNodeDataPointer(final DTree node) {
		assert (node.isLeaf());
		return ((DTreeLeafData) node).dataPointer;
	}
	
	/// Return the root AABB of the tree
	public AABB getRootAABB() {
		return getFatAABB(this.rootNode);
	}
	
	/// Initialize the tree
	private void init() {
		this.rootNode = null;
	}
	
	/// Insert a leaf node in the tree
	private void insertLeafNode(final DTree _node) {
		// If the tree is empty
		if (this.rootNode == null) {
			this.rootNode = _node;
			return;
		}
		// Find the best sibling node for the new node
		final AABB newNodeAABB = _node.aabb;
		DTree currentNode = this.rootNode;
		while (!currentNode.isLeaf()) {
			final DTreeNode node = (DTreeNode) currentNode;
			final DTree leftChild = node.children_left;
			final DTree rightChild = node.children_right;
			// Compute the merged AABB
			final float volumeAABB = currentNode.aabb.getVolume();
			final AABB mergedAABBs = new AABB();
			mergedAABBs.mergeTwoAABBs(currentNode.aabb, newNodeAABB);
			final float mergedVolume = mergedAABBs.getVolume();
			// Compute the cost of making the current node the sibbling of the new node
			final float costS = 2.0f * mergedVolume;
			// Compute the minimum cost of pushing the new node further down the tree (inheritance cost)
			final float costI = 2.0f * (mergedVolume - volumeAABB);
			// Compute the cost of descending into the left child
			float costLeft;
			final AABB currentAndLeftAABB = new AABB();
			currentAndLeftAABB.mergeTwoAABBs(newNodeAABB, leftChild.aabb);
			if (leftChild.isLeaf()) { // If the left child is a leaf
				costLeft = currentAndLeftAABB.getVolume() + costI;
			} else {
				final float leftChildVolume = leftChild.aabb.getVolume();
				costLeft = costI + currentAndLeftAABB.getVolume() - leftChildVolume;
			}
			// Compute the cost of descending into the right child
			float costRight;
			final AABB currentAndRightAABB = new AABB();
			currentAndRightAABB.mergeTwoAABBs(newNodeAABB, rightChild.aabb);
			if (rightChild.isLeaf()) { // If the right child is a leaf
				costRight = currentAndRightAABB.getVolume() + costI;
			} else {
				final float rightChildVolume = rightChild.aabb.getVolume();
				costRight = costI + currentAndRightAABB.getVolume() - rightChildVolume;
			}
			// If the cost of making the current node a sibbling of the new node is smaller than
			// the cost of going down into the left or right child
			if (costS < costLeft && costS < costRight) {
				break;
			}
			// It is cheaper to go down into a child of the current node, choose the best child
			if (costLeft < costRight) {
				currentNode = leftChild;
			} else {
				currentNode = rightChild;
			}
		}
		final DTree siblingNode = currentNode;
		// Create a new parent for the new node and the sibling node
		final WeakReference<DTree> oldParentNode = siblingNode.parent;
		final DTreeNode newParentNode = new DTreeNode();
		newParentNode.parent = currentNode.parent;
		newParentNode.aabb.mergeTwoAABBs(currentNode.aabb, newNodeAABB);
		newParentNode.height = currentNode.height + 1;
		// If the sibling node was not the root node
		if (oldParentNode != null) {
			// replace in parent the child with the new child
			final DTreeNode parentNode = (DTreeNode) oldParentNode.get();
			if (parentNode.children_left == siblingNode) {
				parentNode.children_left = newParentNode;
			} else {
				parentNode.children_right = newParentNode;
			}
		} else {
			// If the sibling node was the root node
			this.rootNode = newParentNode;
		}
		// setup the children
		newParentNode.children_left = siblingNode;
		newParentNode.children_right = _node;
		siblingNode.parent = new WeakReference<DTree>(newParentNode);
		_node.parent = new WeakReference<DTree>(newParentNode);
		
		// Move up in the tree to change the AABBs that have changed
		currentNode = newParentNode;
		assert (!currentNode.isLeaf());
		while (currentNode != null) {
			// Balance the sub-tree of the current node if it is not balanced
			currentNode = balanceSubTreeAtNode(currentNode);
			assert (_node.isLeaf());
			assert (!currentNode.isLeaf());
			final DTreeNode nodeDouble = (DTreeNode) currentNode;
			final DTree leftChild = nodeDouble.children_left;
			final DTree rightChild = nodeDouble.children_right;
			assert (leftChild != null);
			assert (rightChild != null);
			// Recompute the height of the node in the tree
			currentNode.height = FMath.max(leftChild.height, rightChild.height) + 1;
			assert (currentNode.height > 0);
			// Recompute the AABB of the node
			currentNode.aabb.mergeTwoAABBs(leftChild.aabb, rightChild.aabb);
			if (currentNode.parent != null) {
				currentNode = currentNode.parent.get();
			} else {
				currentNode = null;
			}
		}
		assert (_node.isLeaf());
	}
	
	/// Ray casting method
	public void raycast(final Ray _ray, final CallbackRaycast _callback) {
		if (_callback == null) {
			Log.error("call with null callback");
			return;
		}
		float maxFraction = _ray.maxFraction;
		// 128 max
		final Stack<DTree> stack = new Stack<DTree>();
		stack.push(this.rootNode);
		// Walk through the tree from the root looking for proxy shapes
		// that overlap with the ray AABB
		while (stack.size() > 0) {
			// Get the next node in the stack
			final DTree node = stack.pop();
			// If it is a null node, skip it
			if (node == null) {
				continue;
			}
			final Ray rayTemp = new Ray(_ray.point1, _ray.point2, maxFraction);
			// Test if the ray intersects with the current node AABB
			if (node.aabb.testRayIntersect(rayTemp) == false) {
				continue;
			}
			// If the node is a leaf of the tree
			if (node.isLeaf()) {
				// Call the callback that will raycast again the broad-phase shape
				final float hitFraction = _callback.callback(node, rayTemp);
				// If the user returned a hitFraction of zero, it means that
				// the raycasting should stop here
				if (hitFraction == 0.0f) {
					return;
				}
				// If the user returned a positive fraction
				if (hitFraction > 0.0f) {
					// We update the maxFraction value and the ray
					// AABB using the new maximum fraction
					if (hitFraction < maxFraction) {
						maxFraction = hitFraction;
					}
				}
				// If the user returned a negative fraction, we continue
				// the raycasting as if the proxy shape did not exist
			} else { // If the node has children
				final DTreeNode tmpNode = (DTreeNode) node;
				// Push its children in the stack of nodes to explore
				stack.push(tmpNode.children_left);
				stack.push(tmpNode.children_right);
			}
		}
		
	}
	
	/// Release a node
	private void releaseNode(final DTree _node) {
		//this.numberNodes--;
	}
	
	/// Remove a leaf node from the tree
	private void removeLeafNode(final DTree _node) {
		assert (_node.isLeaf());
		// If we are removing the root node (root node is a leaf in this case)
		if (this.rootNode == _node) {
			this.rootNode = null;
			return;
		}
		// parent can not be null.
		final DTreeNode parentNode = (DTreeNode) _node.parent.get();
		final WeakReference<DTree> grandParentNodeWeak = parentNode.parent;
		DTree siblingNode;
		if (parentNode.children_left == _node) {
			siblingNode = parentNode.children_right;
		} else {
			siblingNode = parentNode.children_left;
		}
		// If the parent of the node to remove is not the root node
		if (grandParentNodeWeak == null) {
			// If the parent of the node to remove is the root node
			// The sibling node becomes the new root node
			this.rootNode = siblingNode;
			siblingNode.parent = null;
			releaseNode(parentNode);
		} else {
			final DTreeNode grandParentNode = (DTreeNode) grandParentNodeWeak.get();
			// Destroy the parent node
			if (grandParentNode.children_left == parentNode) {
				grandParentNode.children_left = siblingNode;
			} else {
				grandParentNode.children_right = siblingNode;
			}
			siblingNode.parent = parentNode.parent;
			releaseNode(parentNode);
			// Now, we need to recompute the AABBs of the node on the path back to the root
			// and make sure that the tree is still balanced
			DTree currentNode = grandParentNode;
			while (currentNode != null) {
				// Balance the current sub-tree if necessary
				currentNode = balanceSubTreeAtNode(currentNode);
				assert (!currentNode.isLeaf());
				final DTreeNode currentTreeNode = (DTreeNode) currentNode;
				// Get the two children of the current node
				final DTree leftChild = currentTreeNode.children_left;
				final DTree rightChild = currentTreeNode.children_right;
				// Recompute the AABB and the height of the current node
				currentNode.aabb.mergeTwoAABBs(leftChild.aabb, rightChild.aabb);
				currentNode.height = FMath.max(leftChild.height, rightChild.height) + 1;
				assert (currentNode.height > 0);
				if (currentNode.parent == null) {
					currentNode = null;
				} else {
					currentNode = currentNode.parent.get();
				}
			}
		}
	}
	
	/// Remove an object from the tree
	public void removeObject(final DTree _node) {
		assert (_node.isLeaf());
		// Remove the node from the tree
		removeLeafNode(_node);
		releaseNode(_node);
	}
	
	/// Report all shapes overlapping with the AABB given in parameter.
	public void reportAllShapesOverlappingWithAABB(final AABB _aabb, final CallbackOverlapping _callback) {
		if (_callback == null) {
			Log.error("call with null callback");
			return;
		}
		//Log.error("reportAllShapesOverlappingWithAABB");
		// Create a stack with the nodes to visit
		final Stack<DTree> stack = new Stack<DTree>();
		// 64 max
		stack.push(this.rootNode);
		//Log.error("    add stack: " + this.rootNode);
		// While there are still nodes to visit
		while (stack.size() > 0) {
			// Get the next node ID to visit
			final DTree nodeIDToVisit = stack.pop();
			// Skip it if it is a null node
			if (nodeIDToVisit == null) {
				continue;
			}
			// Get the corresponding node
			final DTree nodeToVisit = nodeIDToVisit;
			//Log.error("      check colision: " + nodeIDToVisit);
			// If the AABB in parameter overlaps with the AABB of the node to visit
			if (_aabb.testCollision(nodeToVisit.aabb)) {
				// If the node is a leaf
				if (nodeToVisit.isLeaf()) {
					/*
					if (_aabb != nodeToVisit.aabb) {
						Log.error("           ======> Real collision ...");
					}
					*/
					// Notify the broad-phase about a new potential overlapping pair
					_callback.callback(nodeIDToVisit);
				} else {
					final DTreeNode tmp = (DTreeNode) nodeToVisit;
					// If the node is not a leaf
					// We need to visit its children
					stack.push(tmp.children_left);
					stack.push(tmp.children_right);
					//Log.error("    add stack: " + tmp.children_left);
					//Log.error("    add stack: " + tmp.children_right);
				}
			}
		}
	}
	
	/// Clear all the nodes and reset the tree
	public void reset() {
		// Initialize the tree
		init();
	}
	
	/// Update the dynamic tree after an object has moved.
	/// If the new AABB of the object that has moved is still inside its fat AABB, then
	/// nothing is done. Otherwise, the corresponding node is removed and reinserted into the tree.
	/// The method returns true if the object has been reinserted into the tree. The "displacement"
	/// argument is the linear velocity of the AABB multiplied by the elapsed time between two
	/// frames. If the "forceReinsert" parameter is true, we force a removal and reinsertion of the node
	/// (this can be useful if the shape AABB has become much smaller than the previous one for instance).
	public boolean updateObject(final DTree _node, final AABB _newAABB, final Vector3f _displacement) {
		return updateObject(_node, _newAABB, _displacement, false);
	}
	
	public boolean updateObject(final DTree _node, final AABB _newAABB, final Vector3f _displacement, final boolean _forceReinsert) {
		assert (_node.isLeaf());
		assert (_node.height >= 0);
		//Log.verbose(" compare : " + _node.aabb.getMin() + " " + _node.aabb.getMax());
		//Log.verbose("         : " + _newAABB.getMin() + " " + _newAABB.getMax());
		// If the new AABB is still inside the fat AABB of the node
		if (_forceReinsert == false && _node.aabb.contains(_newAABB)) {
			return false;
		}
		// If the new AABB is outside the fat AABB, we remove the corresponding node
		removeLeafNode(_node);
		// Compute the fat AABB by inflating the AABB with a ant gap
		_node.aabb = _newAABB;
		
		final Vector3f gap = new Vector3f(this.extraAABBGap, this.extraAABBGap, this.extraAABBGap);
		_node.aabb.getMin().less(gap);
		_node.aabb.getMax().add(gap);
		// Inflate the fat AABB in direction of the linear motion of the AABB
		if (_displacement.x < 0.0f) {
			_node.aabb.getMin().setX(_node.aabb.getMin().x + Configuration.DYNAMIC_TREE_AABB_LIN_GAP_MULTIPLIER * _displacement.x);
		} else {
			_node.aabb.getMax().setX(_node.aabb.getMax().x + Configuration.DYNAMIC_TREE_AABB_LIN_GAP_MULTIPLIER * _displacement.x);
		}
		if (_displacement.y < 0.0f) {
			_node.aabb.getMin().setY(_node.aabb.getMin().y + Configuration.DYNAMIC_TREE_AABB_LIN_GAP_MULTIPLIER * _displacement.y);
		} else {
			_node.aabb.getMax().setY(_node.aabb.getMax().y + Configuration.DYNAMIC_TREE_AABB_LIN_GAP_MULTIPLIER * _displacement.y);
		}
		if (_displacement.z < 0.0f) {
			_node.aabb.getMin().setZ(_node.aabb.getMin().z + Configuration.DYNAMIC_TREE_AABB_LIN_GAP_MULTIPLIER * _displacement.z);
		} else {
			_node.aabb.getMax().setZ(_node.aabb.getMax().z + Configuration.DYNAMIC_TREE_AABB_LIN_GAP_MULTIPLIER * _displacement.z);
		}
		//Log.error(" compare : " + _node.aabb.getMin() + " " + _node.aabb.getMax());
		//Log.error("         : " + _newAABB.getMin() + " " + _newAABB.getMax());
		if (_node.aabb.contains(_newAABB) == false) {
			//Log.critical("ERROR");
		}
		assert (_node.aabb.contains(_newAABB));
		// Reinsert the node into the tree
		insertLeafNode(_node);
		return true;
	}
	
}

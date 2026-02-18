/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Generic tree node holding a payload of type T.
 * Manages parent-child relationships, expand/collapse state.
 * @param <T> The type of data stored in this node.
 */
public class TreeNode<T> {
	private T data;
	private String label;
	private final List<TreeNode<T>> children;
	private TreeNode<T> parent;
	private boolean expanded;
	private boolean leaf;

	/**
	 * Create a tree node with data and label.
	 * @param data The payload data.
	 * @param label The display label.
	 */
	public TreeNode(final T data, final String label) {
		this.data = data;
		this.label = label;
		this.children = new ArrayList<>();
		this.parent = null;
		this.expanded = false;
		this.leaf = false;
	}

	/**
	 * Create a tree node with data, label, and leaf flag.
	 * @param data The payload data.
	 * @param label The display label.
	 * @param leaf True if this node is a leaf (no expand chevron).
	 */
	public TreeNode(final T data, final String label, final boolean leaf) {
		this.data = data;
		this.label = label;
		this.children = new ArrayList<>();
		this.parent = null;
		this.expanded = false;
		this.leaf = leaf;
	}

	public T getData() {
		return this.data;
	}

	public void setData(final T data) {
		this.data = data;
	}

	public String getLabel() {
		return this.label;
	}

	public void setLabel(final String label) {
		this.label = label;
	}

	public List<TreeNode<T>> getChildren() {
		return Collections.unmodifiableList(this.children);
	}

	public TreeNode<T> getParent() {
		return this.parent;
	}

	public boolean isExpanded() {
		return this.expanded;
	}

	public void setExpanded(final boolean expanded) {
		this.expanded = expanded;
	}

	public boolean isLeaf() {
		return this.leaf;
	}

	public void setLeaf(final boolean leaf) {
		this.leaf = leaf;
	}

	/**
	 * Add a child node. Sets the child's parent to this node.
	 * @param child The child node to add.
	 */
	public void addChild(final TreeNode<T> child) {
		child.parent = this;
		this.children.add(child);
	}

	/**
	 * Remove a child node. Clears the child's parent reference.
	 * @param child The child node to remove.
	 */
	public void removeChild(final TreeNode<T> child) {
		if (this.children.remove(child)) {
			child.parent = null;
		}
	}

	/**
	 * Remove all children and clear their parent references.
	 */
	public void removeAllChildren() {
		for (final TreeNode<T> child : this.children) {
			child.parent = null;
		}
		this.children.clear();
	}

	/**
	 * Get the depth of this node in the tree (root = 0).
	 * @return The depth level.
	 */
	public int getDepth() {
		int depth = 0;
		TreeNode<T> current = this.parent;
		while (current != null) {
			depth++;
			current = current.parent;
		}
		return depth;
	}

	/**
	 * Check if this node has children.
	 * @return True if this node has at least one child.
	 */
	public boolean hasChildren() {
		return !this.children.isEmpty();
	}

	/**
	 * Toggle the expanded state. No-op if this is a leaf node without children.
	 */
	public void toggleExpanded() {
		if (this.leaf && !hasChildren()) {
			return;
		}
		this.expanded = !this.expanded;
	}
}

package org.atriasoft.ephysics.collision.broadphase;

import java.lang.ref.WeakReference;

import org.atriasoft.ephysics.collision.shapes.AABB;

/**
 *  It represents a node of the dynamic AABB tree.
 */
public class DTree {
	private static int simpleCounter = 0;
	public final int uid;
	/**
	 *  A node is either in the tree (has a parent) or in the free nodes list (has a next node)
	 */
	WeakReference<DTree> parent = null; //!< Parent node ID (0 is ROOT)
	int height = -1; //!< Height of the node in the tree TODO check the need...
	AABB aabb = new AABB(); //!< Fat axis aligned bounding box (AABB) corresponding to the node
	
	public DTree() {
		this.uid = simpleCounter++;
	}
	
	boolean isLeaf() {
		return false;
	}
	
}

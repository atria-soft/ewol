package org.atriasoft.ephysics.collision;

/**
 *  This structure represents a single element of a linked list of contact manifolds
 */
public class ContactManifoldListElement {
	public ContactManifold contactManifold; //!< Pointer to the actual contact manifold
	public ContactManifoldListElement next; //!< Next element of the list
	
	public ContactManifoldListElement(final ContactManifold _initContactManifold, final ContactManifoldListElement _initNext) {
		this.contactManifold = _initContactManifold;
		this.next = _initNext;
	}
}
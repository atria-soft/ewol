package org.atriasoft.ephysics.constraint;

public class JointListElement {
	
	public Joint joint; //!< Pointer to the actual joint
	public JointListElement next; //!< Next element of the list
	
	/**
	 * Constructor
	 */
	public JointListElement(final Joint _initJoint, final JointListElement _initNext) {
		this.joint = _initJoint;
		this.next = _initNext;
		
	}
}

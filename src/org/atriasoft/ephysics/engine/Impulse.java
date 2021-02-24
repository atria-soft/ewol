package org.atriasoft.ephysics.engine;

import org.atriasoft.etk.math.Vector3f;

/**
 *  Represents an impulse that we can apply to bodies in the contact or raint solver.
 */
public class Impulse {
	public final Vector3f linearImpulseBody1; //!< Linear impulse applied to the first body
	public final Vector3f angularImpulseBody1; //!< Angular impulse applied to the first body
	public final Vector3f linearImpulseBody2; //!< Linear impulse applied to the second body
	public final Vector3f angularImpulseBody2; //!< Angular impulse applied to the second body
	
	public Impulse(final Impulse impulse) {
		this.linearImpulseBody1 = impulse.linearImpulseBody1.clone();
		this.angularImpulseBody1 = impulse.angularImpulseBody1.clone();
		this.linearImpulseBody2 = impulse.linearImpulseBody2.clone();
		this.angularImpulseBody2 = impulse.angularImpulseBody2.clone();
		
	}
	
	public Impulse(final Vector3f _initLinearImpulseBody1, final Vector3f _initAngularImpulseBody1, final Vector3f _initLinearImpulseBody2, final Vector3f _initAngularImpulseBody2) {
		this.linearImpulseBody1 = _initLinearImpulseBody1.clone();
		this.angularImpulseBody1 = _initAngularImpulseBody1.clone();
		this.linearImpulseBody2 = _initLinearImpulseBody2.clone();
		this.angularImpulseBody2 = _initAngularImpulseBody2.clone();
	}
}

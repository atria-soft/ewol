package org.atriasoft.ephysics.engine;

import org.atriasoft.ephysics.collision.ContactManifold;
import org.atriasoft.etk.math.Matrix3f;
import org.atriasoft.etk.math.Vector3f;

/**
 *  Contact solver internal data structure to store all the information relative to a contact manifold.
 */
public class ContactManifoldSolver {
	int indexBody1; //!< Index of body 1 in the raint solver
	int indexBody2; //!< Index of body 2 in the raint solver
	float massInverseBody1; //!< Inverse of the mass of body 1
	float massInverseBody2; //!< Inverse of the mass of body 2
	Matrix3f inverseInertiaTensorBody1 = new Matrix3f(); //!< Inverse inertia tensor of body 1
	Matrix3f inverseInertiaTensorBody2 = new Matrix3f(); //!< Inverse inertia tensor of body 2
	ContactPointSolver[] contacts = new ContactPointSolver[ContactManifold.MAX_CONTACT_POINTS_IN_MANIFOLD]; //!< Contact point raints
	int nbContacts; //!< Number of contact points
	boolean isBody1DynamicType; //!< True if the body 1 is of type dynamic
	boolean isBody2DynamicType; //!< True if the body 2 is of type dynamic
	float restitutionFactor; //!< Mix of the restitution factor for two bodies
	float frictionCoefficient; //!< Mix friction coefficient for the two bodies
	float rollingResistanceFactor; //!< Rolling resistance factor between the two bodies
	ContactManifold externalContactManifold; //!< Pointer to the external contact manifold
	// - Variables used when friction raints are apply at the center of the manifold-//
	Vector3f normal = new Vector3f(0, 0, 0); //!< Average normal vector of the contact manifold
	Vector3f frictionPointBody1 = new Vector3f(0, 0, 0); //!< Point on body 1 where to apply the friction raints
	Vector3f frictionPointBody2 = new Vector3f(0, 0, 0); //!< Point on body 2 where to apply the friction raints
	Vector3f r1Friction = new Vector3f(0, 0, 0); //!< R1 vector for the friction raints
	Vector3f r2Friction = new Vector3f(0, 0, 0); //!< R2 vector for the friction raints
	Vector3f r1CrossT1 = new Vector3f(0, 0, 0); //!< Cross product of r1 with 1st friction vector
	Vector3f r1CrossT2 = new Vector3f(0, 0, 0); //!< Cross product of r1 with 2nd friction vector
	Vector3f r2CrossT1 = new Vector3f(0, 0, 0); //!< Cross product of r2 with 1st friction vector
	Vector3f r2CrossT2 = new Vector3f(0, 0, 0); //!< Cross product of r2 with 2nd friction vector
	float inverseFriction1Mass; //!< Matrix K for the first friction raint
	float inverseFriction2Mass; //!< Matrix K for the second friction raint
	float inverseTwistFrictionMass; //!< Matrix K for the twist friction raint
	Matrix3f inverseRollingResistance = new Matrix3f(); //!< Matrix K for the rolling resistance raint
	Vector3f frictionVector1 = new Vector3f(0, 0, 0); //!< First friction direction at contact manifold center
	Vector3f frictionvec2 = new Vector3f(0, 0, 0); //!< Second friction direction at contact manifold center
	Vector3f oldFrictionVector1 = new Vector3f(0, 0, 0); //!< Old 1st friction direction at contact manifold center
	Vector3f oldFrictionvec2 = new Vector3f(0, 0, 0); //!< Old 2nd friction direction at contact manifold center
	float friction1Impulse; //!< First friction direction impulse at manifold center
	float friction2Impulse; //!< Second friction direction impulse at manifold center
	float frictionTwistImpulse; //!< Twist friction impulse at contact manifold center
	Vector3f rollingResistanceImpulse = new Vector3f(0, 0, 0); //!< Rolling resistance impulse
	
	public ContactManifoldSolver() {
		for (int iii = 0; iii < ContactManifold.MAX_CONTACT_POINTS_IN_MANIFOLD; iii++) {
			this.contacts[iii] = new ContactPointSolver();
		}
	}
}

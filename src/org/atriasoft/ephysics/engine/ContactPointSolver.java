package org.atriasoft.ephysics.engine;

import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.constraint.ContactPoint;

/**
 * Contact solver internal data structure that to store all the
 * information relative to a contact point
 */
public class ContactPointSolver {
	float penetrationImpulse; //!< Accumulated normal impulse
	float friction1Impulse; //!< Accumulated impulse in the 1st friction direction
	float friction2Impulse; //!< Accumulated impulse in the 2nd friction direction
	float penetrationSplitImpulse; //!< Accumulated split impulse for penetration correction
	Vector3f rollingResistanceImpulse = new Vector3f(0, 0, 0); //!< Accumulated rolling resistance impulse
	Vector3f normal = new Vector3f(0, 0, 0); //!< Normal vector of the contact
	Vector3f frictionVector1 = new Vector3f(0, 0, 0); //!< First friction vector in the tangent plane
	Vector3f frictionvec2 = new Vector3f(0, 0, 0); //!< Second friction vector in the tangent plane
	Vector3f oldFrictionVector1 = new Vector3f(0, 0, 0); //!< Old first friction vector in the tangent plane
	Vector3f oldFrictionvec2 = new Vector3f(0, 0, 0); //!< Old second friction vector in the tangent plane
	Vector3f r1 = new Vector3f(0, 0, 0); //!< Vector from the body 1 center to the contact point
	Vector3f r2 = new Vector3f(0, 0, 0); //!< Vector from the body 2 center to the contact point
	Vector3f r1CrossT1 = new Vector3f(0, 0, 0); //!< Cross product of r1 with 1st friction vector
	Vector3f r1CrossT2 = new Vector3f(0, 0, 0); //!< Cross product of r1 with 2nd friction vector
	Vector3f r2CrossT1 = new Vector3f(0, 0, 0); //!< Cross product of r2 with 1st friction vector
	Vector3f r2CrossT2 = new Vector3f(0, 0, 0); //!< Cross product of r2 with 2nd friction vector
	Vector3f r1CrossN = new Vector3f(0, 0, 0); //!< Cross product of r1 with the contact normal
	Vector3f r2CrossN = new Vector3f(0, 0, 0); //!< Cross product of r2 with the contact normal
	float penetrationDepth; //!< Penetration depth
	float restitutionBias; //!< Velocity restitution bias
	float inversePenetrationMass; //!< Inverse of the matrix K for the penenetration
	float inverseFriction1Mass; //!< Inverse of the matrix K for the 1st friction
	float inverseFriction2Mass; //!< Inverse of the matrix K for the 2nd friction
	boolean isRestingContact; //!< True if the contact was existing last time step
	ContactPoint externalContact; //!< Pointer to the external contact
}

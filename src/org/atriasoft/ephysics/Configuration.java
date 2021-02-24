package org.atriasoft.ephysics;

import org.atriasoft.etk.math.Constant;

public class Configuration {
	
	/// In the broad-phase collision detection (dynamic AABB tree), the AABBs are
	/// inflated with a ant gap to allow the collision shape to move a little bit
	/// without triggering a large modification of the tree which can be costly
	public static final float DYNAMIC_TREE_AABB_GAP = 0.1f;
	
	/// In the broad-phase collision detection (dynamic AABB tree), the AABBs are
	/// also inflated in direction of the linear motion of the body by mutliplying the
	/// followin ant with the linear velocity and the elapsed time between two frames.
	public static final float DYNAMIC_TREE_AABB_LIN_GAP_MULTIPLIER = 1.7f;
	
	/// Distance threshold for two contact points for a valid persistent contact (in meters)
	public static final float PERSISTENT_CONTACT_DIST_THRESHOLD = 0.03f;
	
	/// Default friction coefficient for a rigid body
	public static final float DEFAULT_FRICTION_COEFFICIENT = 0.3f;
	
	/// Default bounciness factor for a rigid body
	public static final float DEFAULT_BOUNCINESS = 0.5f;
	
	/// Default rolling resistance
	public static final float DEFAULT_ROLLING_RESISTANCE = 0.0f;
	
	/// True if the spleeping technique is enabled
	public static final boolean SPLEEPING_ENABLED = true;
	
	/// Object margin for collision detection in meters (for the GJK-EPA Algorithm)
	public static final float OBJECT_MARGIN = 0.04f;
	
	/// Velocity threshold for contact velocity restitution
	public static final float RESTITUTION_VELOCITY_THRESHOLD = 1.0f;
	
	/// Number of iterations when solving the velocity raints of the Sequential Impulse technique
	public static final int DEFAULT_VELOCITY_SOLVER_NB_ITERATIONS = 10;
	
	/// Number of iterations when solving the position raints of the Sequential Impulse technique
	public static final int DEFAULT_POSITION_SOLVER_NB_ITERATIONS = 5;
	
	/// Time (in seconds) that a body must stay still to be idered sleeping
	public static final float DEFAULT_TIME_BEFORE_SLEEP = 0.3f; //(old default : 3.0)
	
	/// A body with a linear velocity smaller than the sleep linear velocity (in m/s)
	/// might enter sleeping mode.
	public static final float DEFAULT_SLEEP_LINEAR_VELOCITY = 0.3f; //(old default : 0.01)
	
	/// A body with angular velocity smaller than the sleep angular velocity (in rad/s)
	/// might enter sleeping mode
	public static final float DEFAULT_SLEEP_ANGULAR_VELOCITY = 5.0f * (Constant.PI / 180.0f); //(old default : 0.3)
}

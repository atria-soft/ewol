package org.atriasoft.ephysics.collision.narrowphase.GJK;

import org.atriasoft.ephysics.RaycastInfo;
import org.atriasoft.ephysics.collision.CollisionDetection;
import org.atriasoft.ephysics.collision.CollisionShapeInfo;
import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.collision.narrowphase.NarrowPhaseAlgorithm;
import org.atriasoft.ephysics.collision.narrowphase.NarrowPhaseCallback;
import org.atriasoft.ephysics.collision.narrowphase.EPA.EPAAlgorithm;
import org.atriasoft.ephysics.collision.shapes.CacheData;
import org.atriasoft.ephysics.collision.shapes.ConvexShape;
import org.atriasoft.ephysics.constraint.ContactPointInfo;
import org.atriasoft.ephysics.mathematics.Ray;
import org.atriasoft.etk.math.Constant;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix3f;
import org.atriasoft.etk.math.Transform3D;
import org.atriasoft.etk.math.Vector3f;

/**
 *  This class implements a narrow-phase collision detection algorithm. This
 * algorithm uses the ISA-GJK algorithm and the EPA algorithm. This
 * implementation is based on the implementation discussed in the book
 * "Collision Detection in Interactive 3D Environments" by Gino van den Bergen.
 * This method implements the Hybrid Technique for calculating the
 * penetration depth. The two objects are enlarged with a small margin. If
 * the object intersects in their margins, the penetration depth is quickly
 * computed using the GJK algorithm on the original objects (without margin).
 * If the original objects (without margin) intersect, we run again the GJK
 * algorithm on the enlarged objects (with margin) to compute simplex
 * polytope that contains the origin and give it to the EPA (Expanding
 * Polytope Algorithm) to compute the correct penetration depth between the
 * enlarged objects.
 */
public class GJKAlgorithm extends NarrowPhaseAlgorithm {
	public static final float REL_ERROR = 0.001f;
	
	public static final float REL_ERROR_SQUARE = REL_ERROR * REL_ERROR;
	public static final int MAX_ITERATIONS_GJK_RAYCAST = 32;
	private final EPAAlgorithm algoEPA; //!< EPA Algorithm
	/// This method runs the GJK algorithm on the two enlarged objects (with margin)
	/// to compute a simplex polytope that contains the origin. The two objects are
	/// assumed to intersect in the original objects (without margin). Therefore such
	/// a polytope must exist. Then, we give that polytope to the EPA algorithm to
	/// compute the correct penetration depth and contact points of the enlarged objects.
	
	public GJKAlgorithm(final CollisionDetection collisionDetection) {
		super(collisionDetection);
		this.algoEPA = new EPAAlgorithm();
		this.algoEPA.init();
	}
	
	private void computePenetrationDepthForEnlargedObjects(final CollisionShapeInfo shape1Info, final Transform3D transform1, final CollisionShapeInfo shape2Info, final Transform3D transform2,
			final NarrowPhaseCallback narrowPhaseCallback, final Vector3f v) {
		//Log.info("computePenetrationDepthForEnlargedObjects...");
		final Simplex simplex = new Simplex();
		float distSquare = Float.MAX_VALUE;
		float prevDistSquare;
		assert (shape1Info.collisionShape.isConvex());
		assert (shape2Info.collisionShape.isConvex());
		final ConvexShape shape1 = (ConvexShape) (shape1Info.collisionShape);
		final ConvexShape shape2 = (ConvexShape) (shape2Info.collisionShape);
		final Object shape1CachedCollisionData = shape1Info.cachedCollisionData;
		final Object shape2CachedCollisionData = shape2Info.cachedCollisionData;
		//Log.info("    transform1=" + transform1);
		//Log.info("    transform2=" + transform2);
		
		// Transform3D a point from local space of body 2 to local space
		// of body 1 (the GJK algorithm is done in local space of body 1)
		final Transform3D body2ToBody1 = transform1.inverseNew().multiplyNew(transform2);
		// Matrix that transform a direction from local space of body 1 into local space of body 2
		final Matrix3f rotateToBody2 = transform2.getOrientation().getMatrix().transposeNew().multiply(transform1.getOrientation().getMatrix());
		//Log.info("    body2ToBody1=" + body2ToBody1);
		//Log.info("    rotateToBody2=" + rotateToBody2);
		//Log.info("    v=" + v);
		do {
			// Compute the support points for the enlarged object A and B
			final Vector3f suppA = shape1.getLocalSupportPointWithMargin(v.multiplyNew(-1), (CacheData) shape1CachedCollisionData);
			//Log.info("    suppA=" + suppA);
			final Vector3f suppB = body2ToBody1.multiplyNew(shape2.getLocalSupportPointWithMargin(rotateToBody2.multiplyNew(v), (CacheData) shape2CachedCollisionData));
			//Log.info("    suppB=" + suppB);
			// Compute the support point for the Minkowski difference A-B
			final Vector3f w = suppA.lessNew(suppB);
			final float vDotw = v.dot(w);
			//Log.info("    vDotw=" + vDotw);
			// If the enlarge objects do not intersect
			if (vDotw > 0.0f) {
				//Log.info("        ==> ret 1");
				// No intersection, we return
				return;
			}
			// Add the new support point to the simplex
			simplex.addPoint(w, suppA, suppB);
			if (simplex.isAffinelyDependent()) {
				//Log.info("        ==> ret 2");
				return;
			}
			if (!simplex.computeClosestPoint(v)) {
				//Log.info("        ==> ret 3");
				return;
			}
			// Store and update the square distance
			prevDistSquare = distSquare;
			//Log.info("    distSquare=" + distSquare);
			distSquare = v.length2();
			//Log.info("    distSquare=" + distSquare);
			if (prevDistSquare - distSquare <= Constant.FLOAT_EPSILON * prevDistSquare) {
				//Log.info("        ==> ret 4");
				return;
			}
		} while (!simplex.isFull() && distSquare > Constant.FLOAT_EPSILON * simplex.getMaxLengthSquareOfAPoint());
		// Give the simplex computed with GJK algorithm to the EPA algorithm
		// which will compute the correct penetration depth and contact points
		// between the two enlarged objects
		//Log.info("        ==> ret 5");
		this.algoEPA.computePenetrationDepthAndContactPoints(simplex, shape1Info, transform1, shape2Info, transform2, v, narrowPhaseCallback);
	}
	
	/// Ray casting algorithm agains a convex collision shape using the GJK Algorithm
	/// This method implements the GJK ray casting algorithm described by Gino Van Den Bergen in
	/// "Ray Casting against General Convex Objects with Application to Continuous Collision Detection".
	public boolean raycast(final Ray ray, final ProxyShape proxyShape, final RaycastInfo raycastInfo) {
		assert (proxyShape.getCollisionShape().isConvex());
		final ConvexShape shape = (ConvexShape) (proxyShape.getCollisionShape());
		final Object shapeCachedCollisionData = proxyShape.getCachedCollisionData();
		Vector3f suppA; // Current lower bound point on the ray (starting at ray's origin)
		Vector3f suppB; // Support point on the collision shape
		final float machineEpsilonSquare = Constant.FLOAT_EPSILON * Constant.FLOAT_EPSILON;
		final float epsilon = 0.0001f;
		// Convert the ray origin and direction into the local-space of the collision shape
		final Vector3f rayDirection = ray.point2.lessNew(ray.point1);
		// If the points of the segment are two close, return no hit
		if (rayDirection.length2() < machineEpsilonSquare) {
			return false;
		}
		Vector3f w;
		// Create a simplex set
		final Simplex simplex = new Simplex();
		
		Vector3f n = new Vector3f(0.0f, 0.0f, 0.0f);
		float lambda = 0.0f;
		suppA = ray.point1; // Current lower bound point on the ray (starting at ray's origin)
		suppB = shape.getLocalSupportPointWithoutMargin(rayDirection, (CacheData) shapeCachedCollisionData);
		final Vector3f v = suppA.lessNew(suppB);
		float vDotW, vDotR;
		float distSquare = v.length2();
		int nbIterations = 0;
		// GJK Algorithm loop
		while (distSquare > epsilon && nbIterations < MAX_ITERATIONS_GJK_RAYCAST) {
			// Compute the support points
			suppB = shape.getLocalSupportPointWithoutMargin(v, (CacheData) shapeCachedCollisionData);
			w = suppA.lessNew(suppB);
			vDotW = v.dot(w);
			if (vDotW > 0.0f) {
				vDotR = v.dot(rayDirection);
				if (vDotR >= -machineEpsilonSquare) {
					return false;
				} else {
					// We have found a better lower bound for the hit point aint the ray
					lambda = lambda - vDotW / vDotR;
					suppA = rayDirection.multiplyNew(lambda).add(ray.point1);
					w = suppA.lessNew(suppB);
					n = v;
				}
			}
			// Add the new support point to the simplex
			if (!simplex.isPointInSimplex(w)) {
				simplex.addPoint(w, suppA, suppB);
			}
			// Compute the closest point
			if (simplex.computeClosestPoint(v)) {
				distSquare = v.length2();
			} else {
				distSquare = 0.0f;
			}
			// If the current lower bound distance is larger than the maximum raycasting distance
			if (lambda > ray.maxFraction) {
				return false;
			}
			nbIterations++;
		}
		// If the origin was inside the shape, we return no hit
		if (lambda < Constant.FLOAT_EPSILON) {
			return false;
		}
		// Compute the closet points of both objects (without the margins)
		final Vector3f pointA = new Vector3f();
		final Vector3f pointB = new Vector3f();
		simplex.computeClosestPointsOfAandB(pointA, pointB);
		// A raycast hit has been found, we fill in the raycast info
		raycastInfo.hitFraction = lambda;
		raycastInfo.worldPoint = pointB;
		raycastInfo.body = proxyShape.getBody();
		raycastInfo.proxyShape = proxyShape;
		if (n.length2() >= machineEpsilonSquare) {
			// The normal vector is valid
			raycastInfo.worldNormal = n;
		} else {
			// Degenerated normal vector, we return a zero normal vector
			raycastInfo.worldNormal = new Vector3f(0.0f, 0.0f, 0.0f);
		}
		return true;
	}
	
	@Override
	public void testCollision(final CollisionShapeInfo shape1Info, final CollisionShapeInfo shape2Info, final NarrowPhaseCallback callback) {
		//Log.error("=================================================");
		//Log.error(" shape1Info=" + shape1Info.shapeToWorldTransform);
		//Log.error(" shape2Info=" + shape2Info.shapeToWorldTransform);
		Vector3f suppA = new Vector3f(); // Support point of object A
		Vector3f suppB = new Vector3f(); // Support point of object B
		Vector3f w = new Vector3f(); // Support point of Minkowski difference A-B
		Vector3f pA = new Vector3f(); // Closest point of object A
		Vector3f pB = new Vector3f(); // Closest point of object B
		float vDotw;
		float prevDistSquare;
		assert (shape1Info.collisionShape.isConvex());
		assert (shape2Info.collisionShape.isConvex());
		final ConvexShape shape1 = (ConvexShape) (shape1Info.collisionShape);
		final ConvexShape shape2 = (ConvexShape) (shape2Info.collisionShape);
		final CacheData shape1CachedCollisionData = (CacheData) shape1Info.cachedCollisionData;
		final CacheData shape2CachedCollisionData = (CacheData) shape2Info.cachedCollisionData;
		// Get the local-space to world-space transforms
		final Transform3D transform1 = shape1Info.shapeToWorldTransform.clone();
		final Transform3D transform2 = shape2Info.shapeToWorldTransform.clone();
		// Transform3D a point from local space of body 2 to local
		// space of body 1 (the GJK algorithm is done in local space of body 1)
		final Transform3D body2Tobody1 = transform1.inverseNew().multiplyNew(transform2);
		// Matrix that transform a direction from local
		// space of body 1 into local space of body 2
		final Matrix3f rotateToBody2 = transform2.getOrientation().getMatrix().transposeNew().multiplyNew(transform1.getOrientation().getMatrix());
		// Initialize the margin (sum of margins of both objects)
		final float margin = shape1.getMargin() + shape2.getMargin();
		final float marginSquare = margin * margin;
		assert (margin > 0.0f);
		// Create a simplex set
		final Simplex simplex = new Simplex();
		// Get the previous point V (last cached separating axis)
		final Vector3f cacheSeparatingAxis = this.currentOverlappingPair.getCachedSeparatingAxis().clone();
		// Initialize the upper bound for the square distance
		float distSquare = Float.MAX_VALUE;
		
		//Log.error(" T1=" + transform1 + "       T2=" + transform2);
		//Log.error(" BT1=" + body2Tobody1 + "    RT2=" + rotateToBody2);
		//Log.error(" M=" + FMath.floatToString(margin) + "             M2=" + FMath.floatToString(marginSquare));
		//Log.error(" v=" + cacheSeparatingAxis);
		
		do {
			//Log.error("------------------");
			// Compute the support points for original objects (without margins) A and B
			suppA = shape1.getLocalSupportPointWithoutMargin(cacheSeparatingAxis.multiplyNew(-1.0f), shape1CachedCollisionData);
			suppB = body2Tobody1.multiplyNew(shape2.getLocalSupportPointWithoutMargin(rotateToBody2.multiplyNew(cacheSeparatingAxis), shape2CachedCollisionData));
			// Compute the support point for the Minkowski difference A-B
			w = suppA.lessNew(suppB);
			vDotw = cacheSeparatingAxis.dot(w);
			//Log.error(" suppA=" + suppA);
			//Log.error(" suppB=" + suppB);
			//Log.error(" w=" + w);
			// If the enlarge objects (with margins) do not intersect
			if (vDotw > 0.0f && vDotw * vDotw > distSquare * marginSquare) {
				// Cache the current separating axis for frame coherence
				this.currentOverlappingPair.setCachedSeparatingAxis(cacheSeparatingAxis);
				// No intersection, we return
				return;
			}
			// If the objects intersect only in the margins
			if (simplex.isPointInSimplex(w) || distSquare - vDotw <= distSquare * REL_ERROR_SQUARE) {
				//Log.error("11111111 ");
				// Compute the closet points of both objects (without the margins)
				simplex.computeClosestPointsOfAandB(pA, pB);
				// Project those two points on the margins to have the closest points of both
				// object with the margins	
				final float dist = FMath.sqrt(distSquare);
				assert (dist > 0.0f);
				pA = pA.lessNew(cacheSeparatingAxis.multiplyNew(shape1.getMargin() / dist));
				pB = body2Tobody1.inverseNew().multiplyNew(cacheSeparatingAxis.multiplyNew(shape2.getMargin() / dist).add(pB));
				// Compute the contact info
				final Vector3f normal = transform1.getOrientation().multiply(cacheSeparatingAxis.safeNormalizeNew().multiply(-1));
				final float penetrationDepth = margin - dist;
				// Reject the contact if the penetration depth is negative (due too numerical errors)
				if (penetrationDepth <= 0.0f) {
					return;
				}
				// Create the contact info object
				final ContactPointInfo contactInfo = new ContactPointInfo(shape1Info.proxyShape, shape2Info.proxyShape, shape1Info.collisionShape, shape2Info.collisionShape, normal, penetrationDepth,
						pA, pB);
				callback.notifyContact(shape1Info.overlappingPair, contactInfo);
				// There is an intersection, therefore we return
				return;
			}
			// Add the new support point to the simplex
			simplex.addPoint(w, suppA, suppB);
			// If the simplex is affinely dependent
			if (simplex.isAffinelyDependent()) {
				//Log.error("222222 ");
				// Compute the closet points of both objects (without the margins)
				simplex.computeClosestPointsOfAandB(pA, pB);
				// Project those two points on the margins to have the closest points of both
				// object with the margins
				final float dist = FMath.sqrt(distSquare);
				assert (dist > 0.0f);
				pA = pA.lessNew(cacheSeparatingAxis.multiplyNew(shape1.getMargin() / dist));
				pB = body2Tobody1.inverseNew().multiply(cacheSeparatingAxis.multiplyNew(shape2.getMargin() / dist).add(pB));
				// Compute the contact info
				final Vector3f normal = transform1.getOrientation().multiply(cacheSeparatingAxis.safeNormalizeNew().multiply(-1));
				final float penetrationDepth = margin - dist;
				
				// Reject the contact if the penetration depth is negative (due too numerical errors)
				if (penetrationDepth <= 0.0f) {
					//Log.info("penetration depth " + penetrationDepth);
					return;
				}
				
				// Create the contact info object
				final ContactPointInfo contactInfo = new ContactPointInfo(shape1Info.proxyShape, shape2Info.proxyShape, shape1Info.collisionShape, shape2Info.collisionShape, normal, penetrationDepth,
						pA, pB);
				callback.notifyContact(shape1Info.overlappingPair, contactInfo);
				// There is an intersection, therefore we return
				return;
			}
			// Compute the point of the simplex closest to the origin
			// If the computation of the closest point fail
			if (!simplex.computeClosestPoint(cacheSeparatingAxis)) {
				//Log.error("3333333333 ");
				// Compute the closet points of both objects (without the margins)
				simplex.computeClosestPointsOfAandB(pA, pB);
				// Project those two points on the margins to have the closest points of both
				// object with the margins
				final float dist = FMath.sqrt(distSquare);
				assert (dist > 0.0f);
				pA = pA.lessNew(cacheSeparatingAxis.multiplyNew(shape1.getMargin() / dist));
				pB = body2Tobody1.inverseNew().multiply(cacheSeparatingAxis.multiplyNew(shape2.getMargin() / dist).add(pB));
				// Compute the contact info
				final Vector3f normal = transform1.getOrientation().multiply(cacheSeparatingAxis.safeNormalizeNew().multiply(-1));
				final float penetrationDepth = margin - dist;
				
				// Reject the contact if the penetration depth is negative (due too numerical errors)
				if (penetrationDepth <= 0.0f) {
					return;
				}
				
				// Create the contact info object
				final ContactPointInfo contactInfo = new ContactPointInfo(shape1Info.proxyShape, shape2Info.proxyShape, shape1Info.collisionShape, shape2Info.collisionShape, normal, penetrationDepth,
						pA, pB);
				callback.notifyContact(shape1Info.overlappingPair, contactInfo);
				// There is an intersection, therefore we return
				return;
			}
			// Store and update the squared distance of the closest point
			prevDistSquare = distSquare;
			distSquare = cacheSeparatingAxis.length2();
			// If the distance to the closest point doesn't improve a lot
			if (prevDistSquare - distSquare <= Constant.FLOAT_EPSILON * prevDistSquare) {
				//Log.error("444444444 ");
				simplex.backupClosestPointInSimplex(cacheSeparatingAxis);
				
				// Get the new squared distance
				distSquare = cacheSeparatingAxis.length2();
				// Compute the closet points of both objects (without the margins)
				simplex.computeClosestPointsOfAandB(pA, pB);
				// Project those two points on the margins to have the closest points of both
				// object with the margins
				final float dist = FMath.sqrt(distSquare);
				assert (dist > 0.0f);
				pA = pA.lessNew(cacheSeparatingAxis.multiplyNew(shape1.getMargin() / dist));
				pB = body2Tobody1.inverseNew().multiply(cacheSeparatingAxis.multiplyNew(shape2.getMargin() / dist).add(pB));
				// Compute the contact info
				final Vector3f normal = transform1.getOrientation().multiply(cacheSeparatingAxis.safeNormalizeNew().multiply(-1));
				final float penetrationDepth = margin - dist;
				
				// Reject the contact if the penetration depth is negative (due too numerical errors)
				if (penetrationDepth <= 0.0f) {
					return;
				}
				
				// Create the contact info object
				final ContactPointInfo contactInfo = new ContactPointInfo(shape1Info.proxyShape, shape2Info.proxyShape, shape1Info.collisionShape, shape2Info.collisionShape, normal, penetrationDepth,
						pA, pB);
				callback.notifyContact(shape1Info.overlappingPair, contactInfo);
				// There is an intersection, therefore we return
				return;
			}
		} while (!simplex.isFull() && distSquare > Constant.FLOAT_EPSILON * simplex.getMaxLengthSquareOfAPoint());
		// The objects (without margins) intersect. Therefore, we run the GJK algorithm
		// again but on the enlarged objects to compute a simplex polytope that contains
		// the origin. Then, we give that simplex polytope to the EPA algorithm to compute
		// the correct penetration depth and contact points between the enlarged objects.
		computePenetrationDepthForEnlargedObjects(shape1Info, transform1, shape2Info, transform2, callback, cacheSeparatingAxis);
	}
	
	/// Use the GJK Algorithm to find if a point is inside a convex collision shape
	public boolean testPointInside(final Vector3f localPoint, final ProxyShape proxyShape) {
		Vector3f suppA = new Vector3f(); // Support point of object A
		Vector3f w = new Vector3f(); // Support point of Minkowski difference A-B
		float prevDistSquare;
		assert (proxyShape.getCollisionShape().isConvex());
		final ConvexShape shape = (ConvexShape) (proxyShape.getCollisionShape());
		final CacheData shapeCachedCollisionData = (CacheData) proxyShape.getCachedCollisionData();
		// Support point of object B (object B is a single point)
		final Vector3f suppB = new Vector3f(localPoint);
		// Create a simplex set
		final Simplex simplex = new Simplex();
		
		// Initial supporting direction
		final Vector3f v = new Vector3f(1, 1, 1);
		// Initialize the upper bound for the square distance
		float distSquare = Float.MAX_VALUE;
		do {
			// Compute the support points for original objects (without margins) A and B
			suppA = shape.getLocalSupportPointWithoutMargin(v.multiplyNew(-1), shapeCachedCollisionData);
			// Compute the support point for the Minkowski difference A-B
			w = suppA.lessNew(suppB);
			// Add the new support point to the simplex
			simplex.addPoint(w, suppA, suppB);
			// If the simplex is affinely dependent
			if (simplex.isAffinelyDependent()) {
				return false;
			}
			// Compute the point of the simplex closest to the origin
			// If the computation of the closest point fail
			if (!simplex.computeClosestPoint(v)) {
				return false;
			}
			// Store and update the squared distance of the closest point
			prevDistSquare = distSquare;
			distSquare = v.length2();
			// If the distance to the closest point doesn't improve a lot
			if (prevDistSquare - distSquare <= Constant.FLOAT_EPSILON * prevDistSquare) {
				return false;
			}
		} while (!simplex.isFull() && distSquare > Constant.FLOAT_EPSILON * simplex.getMaxLengthSquareOfAPoint());
		// The point is inside the collision shape
		return true;
	}
}

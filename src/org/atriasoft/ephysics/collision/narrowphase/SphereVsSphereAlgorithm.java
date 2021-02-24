package org.atriasoft.ephysics.collision.narrowphase;

import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Transform3D;
import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.collision.CollisionDetection;
import org.atriasoft.ephysics.collision.CollisionShapeInfo;
import org.atriasoft.ephysics.collision.shapes.SphereShape;
import org.atriasoft.ephysics.constraint.ContactPointInfo;

/**
 *  It is used to compute the narrow-phase collision detection
 * between two sphere collision shapes.
 */
public class SphereVsSphereAlgorithm extends NarrowPhaseAlgorithm {
	
	public SphereVsSphereAlgorithm(final CollisionDetection collisionDetection) {
		super(collisionDetection);
	}
	
	@Override
	public void testCollision(final CollisionShapeInfo _shape1Info, final CollisionShapeInfo _shape2Info, final NarrowPhaseCallback _narrowPhaseCallback) {
		// Get the sphere collision shapes
		final SphereShape sphereShape1 = (SphereShape) _shape1Info.collisionShape;
		final SphereShape sphereShape2 = (SphereShape) _shape2Info.collisionShape;
		// Get the local-space to world-space transforms
		final Transform3D transform1 = _shape1Info.shapeToWorldTransform;
		final Transform3D transform2 = _shape2Info.shapeToWorldTransform;
		// Compute the distance between the centers
		final Vector3f vectorBetweenCenters = transform2.getPosition().lessNew(transform1.getPosition());
		final float squaredDistanceBetweenCenters = vectorBetweenCenters.length2();
		// Compute the sum of the radius
		final float sumRadius = sphereShape1.getRadius() + sphereShape2.getRadius();
		// If the sphere collision shapes intersect
		if (squaredDistanceBetweenCenters <= sumRadius * sumRadius) {
			final Vector3f centerSphere2InBody1LocalSpace = transform1.inverseNew().multiply(transform2.getPosition());
			final Vector3f centerSphere1InBody2LocalSpace = transform2.inverseNew().multiply(transform1.getPosition());
			final Vector3f intersectionOnBody1 = centerSphere2InBody1LocalSpace.safeNormalizeNew().multiply(sphereShape1.getRadius());
			final Vector3f intersectionOnBody2 = centerSphere1InBody2LocalSpace.safeNormalizeNew().multiply(sphereShape2.getRadius());
			final float penetrationDepth = sumRadius - FMath.sqrt(squaredDistanceBetweenCenters);
			
			// Create the contact info object
			final ContactPointInfo contactInfo = new ContactPointInfo(_shape1Info.proxyShape, _shape2Info.proxyShape, _shape1Info.collisionShape, _shape2Info.collisionShape,
					vectorBetweenCenters.safeNormalizeNew(), penetrationDepth, intersectionOnBody1, intersectionOnBody2);
			// Notify about the new contact
			_narrowPhaseCallback.notifyContact(_shape1Info.overlappingPair, contactInfo);
		}
	}
}

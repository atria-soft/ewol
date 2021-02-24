package org.atriasoft.ephysics.collision.narrowphase.EPA;

import java.util.Comparator;
import java.util.SortedSet;
import java.util.TreeSet;

import org.atriasoft.ephysics.collision.CollisionShapeInfo;
import org.atriasoft.ephysics.collision.narrowphase.NarrowPhaseCallback;
import org.atriasoft.ephysics.collision.narrowphase.GJK.GJKAlgorithm;
import org.atriasoft.ephysics.collision.narrowphase.GJK.Simplex;
import org.atriasoft.ephysics.collision.shapes.CacheData;
import org.atriasoft.ephysics.collision.shapes.ConvexShape;
import org.atriasoft.ephysics.constraint.ContactPointInfo;
import org.atriasoft.etk.math.Constant;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Quaternion;
import org.atriasoft.etk.math.Transform3D;
import org.atriasoft.etk.math.Vector3f;

/**
 *  Class EPAAlgorithm
 * This class is the implementation of the Expanding Polytope Algorithm (EPA).
 * The EPA algorithm computes the penetration depth and contact points between
 * two enlarged objects (with margin) where the original objects (without margin)
 * intersect. The penetration depth of a pair of intersecting objects A and B is
 * the length of a point on the boundary of the Minkowski sum (A-B) closest to the
 * origin. The goal of the EPA algorithm is to start with an initial simplex polytope
 * that contains the origin and expend it in order to find the point on the boundary
 * of (A-B) that is closest to the origin. An initial simplex that contains origin
 * has been computed wit GJK algorithm. The EPA Algorithm will extend this simplex
 * polytope to find the correct penetration depth. The implementation of the EPA
 * algorithm is based on the book "Collision Detection in 3D Environments".
 */
public class EPAAlgorithm {
	/// Add a triangle face in the candidate triangle heap
	private void addFaceCandidate(final TriangleEPA _triangle, final SortedSet<TriangleEPA> _heap, final float _upperBoundSquarePenDepth) {
		////Log.info("addFaceCandidate: " + _triangle.get(0) + ", " + _triangle.get(1) + ", " + _triangle.get(2) + "    " + _upperBoundSquarePenDepth);
		// If the closest point of the affine hull of triangle
		// points is internal to the triangle and if the distance
		// of the closest point from the origin is at most the
		// penetration depth upper bound
		////Log.info("    _triangle.isClosestPointInternalToTriangle(): " + _triangle.isClosestPointInternalToTriangle());
		////Log.info("    _triangle.getDistSquare(): " + _triangle.getDistSquare());
		if (_triangle.isClosestPointInternalToTriangle() && _triangle.getDistSquare() <= _upperBoundSquarePenDepth) {
			// Add the triangle face to the list of candidates
			_heap.add(_triangle);
			////Log.info("add in heap:");
			//int iii = 0;
			//for (final TriangleEPA elem : _heap) {
			//	////Log.info("    [" + iii + "] " + elem.getDistSquare());
			//	++iii;
			//}
		}
	}
	
	// Compute the penetration depth with the EPA algorithm.
	/// This method computes the penetration depth and contact points between two
	/// enlarged objects (with margin) where the original objects (without margin)
	/// intersect. An initial simplex that contains origin has been computed with
	/// GJK algorithm. The EPA Algorithm will extend this simplex polytope to find
	/// the correct penetration depth
	public void computePenetrationDepthAndContactPoints(final Simplex _simplex, final CollisionShapeInfo _shape1Info, final Transform3D _transform1, final CollisionShapeInfo _shape2Info,
			final Transform3D _transform2, final Vector3f _vector, final NarrowPhaseCallback _narrowPhaseCallback) {
		////Log.info("computePenetrationDepthAndContactPoints()");
		assert (_shape1Info.collisionShape.isConvex());
		assert (_shape2Info.collisionShape.isConvex());
		final ConvexShape shape1 = (ConvexShape) (_shape1Info.collisionShape);
		final ConvexShape shape2 = (ConvexShape) (_shape2Info.collisionShape);
		final CacheData shape1CachedCollisionData = (CacheData) _shape1Info.cachedCollisionData;
		final CacheData shape2CachedCollisionData = (CacheData) _shape2Info.cachedCollisionData;
		final Vector3f suppPointsA[] = new Vector3f[EdgeEPA.MAX_SUPPORT_POINTS]; // Support points of object A in local coordinates
		final Vector3f suppPointsB[] = new Vector3f[EdgeEPA.MAX_SUPPORT_POINTS]; // Support points of object B in local coordinates
		final Vector3f points[] = new Vector3f[EdgeEPA.MAX_SUPPORT_POINTS]; // Current points
		final TrianglesStore triangleStore = new TrianglesStore(); // Store the triangles
		
		//https://docs.oracle.com/en/java/javase/11/docs/api/java.base/java/util/SortedSet.html
		//	https://stackoverflow.com/questions/38066291/how-to-define-comparator-on-sortedset-like-treeset
		final SortedSet<TriangleEPA> triangleHeap = new TreeSet<>(new Comparator<TriangleEPA>() {
			@Override
			public int compare(final TriangleEPA _face1, final TriangleEPA _face2) {
				final float d1 = _face1.getDistSquare();
				final float d2 = _face2.getDistSquare();
				if (d1 < d2) {
					return -1;
				}
				if (d1 > d2) {
					return 1;
				}
				return 0;
			}
		});
		// Transform3D a point from local space of body 2 to local
		// space of body 1 (the GJK algorithm is done in local space of body 1)
		final Transform3D body2Tobody1 = _transform1.inverseNew().multiplyNew(_transform2);
		// Matrix that transform a direction from local
		// space of body 1 into local space of body 2
		final Quaternion rotateToBody2 = _transform2.getOrientation().inverseNew().multiplyNew(_transform1.getOrientation());
		// Get the simplex computed previously by the GJK algorithm
		int nbVertices = _simplex.getSimplex(suppPointsA, suppPointsB, points);
		// Compute the tolerance
		final float tolerance = Constant.FLOAT_EPSILON * _simplex.getMaxLengthSquareOfAPoint();
		// Clear the storing of triangles
		triangleStore.clear();
		// Select an action according to the number of points in the simplex
		// computed with GJK algorithm in order to obtain an initial polytope for
		// The EPA algorithm.
		////Log.info(">>>>>>>>>>>>>>>>>> *** " + nbVertices);
		switch (nbVertices) {
			case 1:
				// Only one point in the simplex (which should be the origin).
				// We have a touching contact with zero penetration depth.
				// We drop that kind of contact. Therefore, we return false
				return;
			case 2: {
				// The simplex returned by GJK is a line segment d containing the origin.
				// We add two additional support points to ruct a hexahedron (two tetrahedron
				// glued together with triangle faces. The idea is to compute three different vectors
				// v1, v2 and v3 that are orthogonal to the segment d. The three vectors are relatively
				// rotated of 120 degree around the d segment. The the three new points to
				// ruct the polytope are the three support points in those three directions
				// v1, v2 and v3.
				// Direction of the segment
				final Vector3f d = points[1].lessNew(points[0]).safeNormalizeNew();
				// Choose the coordinate axis from the minimal absolute component of the vector d
				final int minAxis = d.abs().getMinAxis();
				// Compute sin(60)
				final float sin60 = FMath.sqrt(3.0f) * 0.5f;
				// Create a rotation quaternion to rotate the vector v1 to get the vectors
				// v2 and v3
				final Quaternion rotationQuat = new Quaternion(d.x * sin60, d.y * sin60, d.z * sin60, 0.5f);
				// Compute the vector v1, v2, v3
				final Vector3f v1 = d.cross(new Vector3f(minAxis == 0 ? 1.0f : 0.0f, minAxis == 1 ? 1.0f : 0.0f, minAxis == 2 ? 1.0f : 0.0f));
				final Vector3f v2 = rotationQuat.multiply(v1);
				final Vector3f v3 = rotationQuat.multiply(v2);
				// Compute the support point in the direction of v1
				suppPointsA[2] = shape1.getLocalSupportPointWithMargin(v1, shape1CachedCollisionData);
				suppPointsB[2] = body2Tobody1.multiplyNew(shape2.getLocalSupportPointWithMargin(rotateToBody2.multiply(v1.multiplyNew(-1)), shape2CachedCollisionData));
				points[2] = suppPointsA[2].lessNew(suppPointsB[2]);
				// Compute the support point in the direction of v2
				suppPointsA[3] = shape1.getLocalSupportPointWithMargin(v2, shape1CachedCollisionData);
				suppPointsB[3] = body2Tobody1.multiplyNew(shape2.getLocalSupportPointWithMargin(rotateToBody2.multiply(v2.multiplyNew(-1)), shape2CachedCollisionData));
				points[3] = suppPointsA[3].lessNew(suppPointsB[3]);
				// Compute the support point in the direction of v3
				suppPointsA[4] = shape1.getLocalSupportPointWithMargin(v3, shape1CachedCollisionData);
				suppPointsB[4] = body2Tobody1.multiplyNew(shape2.getLocalSupportPointWithMargin(rotateToBody2.multiply(v3.multiplyNew(-1)), shape2CachedCollisionData));
				points[4] = suppPointsA[4].lessNew(suppPointsB[4]);
				// Now we have an hexahedron (two tetrahedron glued together). We can simply keep the
				// tetrahedron that contains the origin in order that the initial polytope of the
				// EPA algorithm is a tetrahedron, which is simpler to deal with.
				// If the origin is in the tetrahedron of points 0, 2, 3, 4
				if (isOriginInTetrahedron(points[0], points[2], points[3], points[4]) == 0) {
					// We use the point 4 instead of point 1 for the initial tetrahedron
					suppPointsA[1] = suppPointsA[4];
					suppPointsB[1] = suppPointsB[4];
					points[1] = points[4];
				}
				// If the origin is in the tetrahedron of points 1, 2, 3, 4
				else if (isOriginInTetrahedron(points[1], points[2], points[3], points[4]) == 0) {
					// We use the point 4 instead of point 0 for the initial tetrahedron
					suppPointsA[0] = suppPointsA[4];
					suppPointsB[0] = suppPointsB[4];
					points[0] = points[4];
				} else {
					// The origin is not in the initial polytope
					return;
				}
				// The polytope contains now 4 vertices
				nbVertices = 4;
			}
			case 4: {
				// The simplex computed by the GJK algorithm is a tetrahedron. Here we check
				// if this tetrahedron contains the origin. If it is the case, we keep it and
				// otherwise we remove the wrong vertex of the tetrahedron and go in the case
				// where the GJK algorithm compute a simplex of three vertices.
				// Check if the tetrahedron contains the origin (or wich is the wrong vertex otherwise)
				final int badVertex = isOriginInTetrahedron(points[0], points[1], points[2], points[3]);
				// If the origin is in the tetrahedron
				if (badVertex == 0) {
					// The tetrahedron is a correct initial polytope for the EPA algorithm.
					// Therefore, we ruct the tetrahedron.
					// Comstruct the 4 triangle faces of the tetrahedron
					////Log.error("befor call: (points, 0, 1, 2)");
					final TriangleEPA face0 = triangleStore.newTriangle(points, 0, 1, 2);
					////Log.error("befor call: (points, 0, 3, 1)");
					final TriangleEPA face1 = triangleStore.newTriangle(points, 0, 3, 1);
					////Log.error("befor call: (points, 0, 2, 3)");
					final TriangleEPA face2 = triangleStore.newTriangle(points, 0, 2, 3);
					////Log.error("befor call: (points, 1, 3, 2)");
					final TriangleEPA face3 = triangleStore.newTriangle(points, 1, 3, 2);
					// If the ructed tetrahedron is not correct
					if (!((face0 != null) && (face1 != null) && (face2 != null) && (face3 != null) && face0.getDistSquare() > 0.0f && face1.getDistSquare() > 0.0 && face2.getDistSquare() > 0.0f
							&& face3.getDistSquare() > 0.0)) {
						return;
					}
					// Associate the edges of neighbouring triangle faces
					TriangleEPA.link(new EdgeEPA(face0, 0), new EdgeEPA(face1, 2));
					TriangleEPA.link(new EdgeEPA(face0, 1), new EdgeEPA(face3, 2));
					TriangleEPA.link(new EdgeEPA(face0, 2), new EdgeEPA(face2, 0));
					TriangleEPA.link(new EdgeEPA(face1, 0), new EdgeEPA(face2, 2));
					TriangleEPA.link(new EdgeEPA(face1, 1), new EdgeEPA(face3, 0));
					TriangleEPA.link(new EdgeEPA(face2, 1), new EdgeEPA(face3, 1));
					// Add the triangle faces in the candidate heap
					addFaceCandidate(face0, triangleHeap, Float.MAX_VALUE);
					addFaceCandidate(face1, triangleHeap, Float.MAX_VALUE);
					addFaceCandidate(face2, triangleHeap, Float.MAX_VALUE);
					addFaceCandidate(face3, triangleHeap, Float.MAX_VALUE);
					break;
				}
				// The tetrahedron contains a wrong vertex (the origin is not inside the tetrahedron)
				// Remove the wrong vertex and continue to the next case with the
				// three remaining vertices
				if (badVertex < 4) {
					suppPointsA[badVertex - 1] = suppPointsA[3];
					suppPointsB[badVertex - 1] = suppPointsB[3];
					points[badVertex - 1] = points[3];
				}
				// We have removed the wrong vertex
				nbVertices = 3;
			}
			case 3: {
				// The GJK algorithm returned a triangle that contains the origin.
				// We need two new vertices to create two tetrahedron. The two new
				// vertices are the support points in the "n" and "-n" direction
				// where "n" is the normal of the triangle. Then, we use only the
				// tetrahedron that contains the origin.
				// Compute the normal of the triangle
				final Vector3f v1 = points[1].lessNew(points[0]);
				final Vector3f v2 = points[2].lessNew(points[0]);
				final Vector3f n = v1.cross(v2);
				////Log.info(">>>>>>>>>>>>>>>>>>");
				////Log.info("    v1 = " + v1);
				////Log.info("    v2 = " + v2);
				////Log.info("    n = " + n);
				// Compute the two new vertices to obtain a hexahedron
				suppPointsA[3] = shape1.getLocalSupportPointWithMargin(n, shape1CachedCollisionData);
				suppPointsB[3] = body2Tobody1.multiplyNew(shape2.getLocalSupportPointWithMargin(rotateToBody2.multiply(n.multiplyNew(-1)), shape2CachedCollisionData));
				points[3] = suppPointsA[3].lessNew(suppPointsB[3]);
				////Log.info("    suppPointsA[3]= " + suppPointsA[3]);
				////Log.info("    suppPointsB[3]= " + suppPointsB[3]);
				////Log.info("    points[3] = " + points[3]);
				suppPointsA[4] = shape1.getLocalSupportPointWithMargin(n.multiplyNew(-1), shape1CachedCollisionData);
				suppPointsB[4] = body2Tobody1.multiplyNew(shape2.getLocalSupportPointWithMargin(rotateToBody2.multiply(n), shape2CachedCollisionData));
				points[4] = suppPointsA[4].lessNew(suppPointsB[4]);
				////Log.info("    suppPointsA[4]= " + suppPointsA[4]);
				////Log.info("    suppPointsB[4]= " + suppPointsB[4]);
				////Log.info("    points[4]= " + points[4]);
				TriangleEPA face0 = null;
				TriangleEPA face1 = null;
				TriangleEPA face2 = null;
				TriangleEPA face3 = null;
				// If the origin is in the first tetrahedron
				if (isOriginInTetrahedron(points[0], points[1], points[2], points[3]) == 0) {
					// The tetrahedron is a correct initial polytope for the EPA algorithm.
					// Therefore, we ruct the tetrahedron.
					// Comstruct the 4 triangle faces of the tetrahedron
					////Log.error("befor call: (points, 0, 1, 2)");
					face0 = triangleStore.newTriangle(points, 0, 1, 2);
					////Log.error("befor call: (points, 0, 2, 1)");
					face1 = triangleStore.newTriangle(points, 0, 3, 1);
					////Log.error("befor call: (points, 0, 2, 3)");
					face2 = triangleStore.newTriangle(points, 0, 2, 3);
					////Log.error("befor call: (points, 1, 3, 2)");
					face3 = triangleStore.newTriangle(points, 1, 3, 2);
				} else if (isOriginInTetrahedron(points[0], points[1], points[2], points[4]) == 0) {
					// The tetrahedron is a correct initial polytope for the EPA algorithm.
					// Therefore, we ruct the tetrahedron.
					// Comstruct the 4 triangle faces of the tetrahedron
					////Log.error("befor call: (points, 0, 1, 2)");
					face0 = triangleStore.newTriangle(points, 0, 1, 2);
					////Log.error("befor call: (points, 0, 4, 1)");
					face1 = triangleStore.newTriangle(points, 0, 4, 1);
					////Log.error("befor call: (points, 0, 2, 4)");
					face2 = triangleStore.newTriangle(points, 0, 2, 4);
					////Log.error("befor call: (points, 1, 4, 2)");
					face3 = triangleStore.newTriangle(points, 1, 4, 2);
				} else {
					return;
				}
				// If the ructed tetrahedron is not correct
				if (!(face0 != null && face1 != null && face2 != null && face3 != null && face0.getDistSquare() > 0.0f && face1.getDistSquare() > 0.0f && face2.getDistSquare() > 0.0f
						&& face3.getDistSquare() > 0.0f)) {
					return;
				}
				// Associate the edges of neighbouring triangle faces
				TriangleEPA.link(new EdgeEPA(face0, 0), new EdgeEPA(face1, 2));
				TriangleEPA.link(new EdgeEPA(face0, 1), new EdgeEPA(face3, 2));
				TriangleEPA.link(new EdgeEPA(face0, 2), new EdgeEPA(face2, 0));
				TriangleEPA.link(new EdgeEPA(face1, 0), new EdgeEPA(face2, 2));
				TriangleEPA.link(new EdgeEPA(face1, 1), new EdgeEPA(face3, 0));
				TriangleEPA.link(new EdgeEPA(face2, 1), new EdgeEPA(face3, 1));
				// Add the triangle faces in the candidate heap
				addFaceCandidate(face0, triangleHeap, Float.MAX_VALUE);
				addFaceCandidate(face1, triangleHeap, Float.MAX_VALUE);
				addFaceCandidate(face2, triangleHeap, Float.MAX_VALUE);
				addFaceCandidate(face3, triangleHeap, Float.MAX_VALUE);
				nbVertices = 4;
			}
				break;
		}
		// At this point, we have a polytope that contains the origin. Therefore, we
		// can run the EPA algorithm.
		if (triangleHeap.size() == 0) {
			return;
		}
		TriangleEPA triangle = null;
		float upperBoundSquarePenDepth = Float.MAX_VALUE;
		do {
			triangle = triangleHeap.first();
			triangleHeap.remove(triangle);
			////Log.info("rm from heap:");
			int iii = 0;
			for (final TriangleEPA elem : triangleHeap) {
				////Log.info("    [" + iii + "] " + elem.getDistSquare());
				++iii;
			}
			// If the candidate face in the heap is not obsolete
			if (!triangle.getIsObsolete()) {
				// If we have reached the maximum number of support points
				if (nbVertices == EdgeEPA.MAX_SUPPORT_POINTS) {
					assert (false);
					break;
				}
				// Compute the support point of the Minkowski
				// difference (A-B) in the closest point direction
				suppPointsA[nbVertices] = shape1.getLocalSupportPointWithMargin(triangle.getClosestPoint(), shape1CachedCollisionData);
				suppPointsB[nbVertices] = body2Tobody1
						.multiplyNew(shape2.getLocalSupportPointWithMargin(rotateToBody2.multiply(triangle.getClosestPoint().multiplyNew(-1)), shape2CachedCollisionData));
				points[nbVertices] = suppPointsA[nbVertices].lessNew(suppPointsB[nbVertices]);
				final int indexNewVertex = nbVertices;
				nbVertices++;
				// Update the upper bound of the penetration depth
				final float wDotv = points[indexNewVertex].dot(triangle.getClosestPoint());
				////Log.info("      point=" + points[indexNewVertex]);
				////Log.info("close point=" + triangle.getClosestPoint());
				////Log.info("         ==>" + wDotv);
				if (wDotv < 0.0f) {
					////Log.error("depth penetration error " + wDotv);
					continue;
				}
				assert (wDotv >= 0.0f);
				final float wDotVSquare = wDotv * wDotv / triangle.getDistSquare();
				if (wDotVSquare < upperBoundSquarePenDepth) {
					upperBoundSquarePenDepth = wDotVSquare;
				}
				// Compute the error
				final float error = wDotv - triangle.getDistSquare();
				if (error <= FMath.max(tolerance, GJKAlgorithm.REL_ERROR_SQUARE * wDotv) || points[indexNewVertex].isEqual(points[triangle.get(0)])
						|| points[indexNewVertex].isEqual(points[triangle.get(1)]) || points[indexNewVertex].isEqual(points[triangle.get(2)])) {
					break;
				}
				// Now, we compute the silhouette cast by the new vertex. The current triangle
				// face will not be in the convex hull. We start the local recursive silhouette
				// algorithm from the current triangle face.
				int i = triangleStore.getNbTriangles();
				if (!triangle.computeSilhouette(points, indexNewVertex, triangleStore)) {
					break;
				}
				// Add all the new triangle faces computed with the silhouette algorithm
				// to the candidates list of faces of the current polytope
				while (i != triangleStore.getNbTriangles()) {
					final TriangleEPA newTriangle = triangleStore.get(i);
					addFaceCandidate(newTriangle, triangleHeap, upperBoundSquarePenDepth);
					i++;
				}
			}
		} while (triangleHeap.size() > 0 && triangleHeap.first().getDistSquare() <= upperBoundSquarePenDepth);
		// Compute the contact info
		final Vector3f tmp = _transform1.getOrientation().multiply(triangle.getClosestPoint());
		_vector.set(tmp);
		final Vector3f pALocal = triangle.computeClosestPointOfObject(suppPointsA);
		final Vector3f pBLocal = body2Tobody1.inverseNew().multiply(triangle.computeClosestPointOfObject(suppPointsB));
		final Vector3f normal = _vector.safeNormalizeNew();
		final float penetrationDepth = _vector.length();
		assert (penetrationDepth >= 0.0f);
		if (normal.length2() < Constant.FLOAT_EPSILON) {
			return;
		}
		// Create the contact info object
		final ContactPointInfo contactInfo = new ContactPointInfo(_shape1Info.proxyShape, _shape2Info.proxyShape, _shape1Info.collisionShape, _shape2Info.collisionShape, normal, penetrationDepth,
				pALocal, pBLocal);
		_narrowPhaseCallback.notifyContact(_shape1Info.overlappingPair, contactInfo);
	}
	
	/// Initalize the algorithm
	public void init() {
		
	}
	
	// Decide if the origin is in the tetrahedron.
	/// Return 0 if the origin is in the tetrahedron and return the number (1,2,3 or 4) of
	/// the vertex that is wrong if the origin is not in the tetrahedron
	private int isOriginInTetrahedron(final Vector3f _p1, final Vector3f _p2, final Vector3f _p3, final Vector3f _p4) {
		////Log.error("isOriginInTetrahedron(" + _p1 + ", " + _p2 + ", " + _p3 + ", " + _p4 + ")");
		// Check vertex 1
		final Vector3f normal1 = _p2.lessNew(_p1).cross(_p3.lessNew(_p1));
		if ((normal1.dot(_p1) > 0.0f) == (normal1.dot(_p4) > 0.0)) {
			////Log.error("    ==> 4");
			return 4;
		}
		// Check vertex 2
		final Vector3f normal2 = _p4.lessNew(_p2).cross(_p3.lessNew(_p2));
		if ((normal2.dot(_p2) > 0.0f) == (normal2.dot(_p1) > 0.0)) {
			////Log.error("    ==> 1");
			return 1;
		}
		// Check vertex 3
		final Vector3f normal3 = _p4.lessNew(_p3).cross(_p1.lessNew(_p3));
		if ((normal3.dot(_p3) > 0.0f) == (normal3.dot(_p2) > 0.0)) {
			////Log.error("    ==> 2");
			return 2;
		}
		// Check vertex 4
		final Vector3f normal4 = _p2.lessNew(_p4).cross(_p1.lessNew(_p4));
		if ((normal4.dot(_p4) > 0.0f) == (normal4.dot(_p3) > 0.0)) {
			////Log.error("    ==> 3");
			return 3;
		}
		////Log.error("    ==> 0");
		// The origin is in the tetrahedron, we return 0
		return 0;
	}
	
}

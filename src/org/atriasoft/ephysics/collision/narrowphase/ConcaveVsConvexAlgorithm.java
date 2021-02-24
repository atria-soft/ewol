package org.atriasoft.ephysics.collision.narrowphase;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

import org.atriasoft.etk.math.Transform3D;
import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.collision.CollisionDetection;
import org.atriasoft.ephysics.collision.CollisionShapeInfo;
import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.collision.shapes.AABB;
import org.atriasoft.ephysics.collision.shapes.CollisionShapeType;
import org.atriasoft.ephysics.collision.shapes.ConcaveShape;
import org.atriasoft.ephysics.collision.shapes.ConvexShape;
import org.atriasoft.ephysics.collision.shapes.TriangleShape;
import org.atriasoft.ephysics.constraint.ContactPointInfo;
import org.atriasoft.ephysics.engine.OverlappingPair;
import org.atriasoft.ephysics.mathematics.Mathematics;
import org.atriasoft.ephysics.mathematics.PairIntVector3f;

//static boolean sortFunction(SmoothMeshContactInfo&_contact1,SmoothMeshContactInfo&_contact2){return _contact1.contactInfo.penetrationDepth<=_contact2.contactInfo.penetrationDepth;}

/**
 *  This class is used to compute the narrow-phase collision detection
 * between a concave collision shape and a convex collision shape. The idea is
 * to use the GJK collision detection algorithm to compute the collision between
 * the convex shape and each of the triangles in the concave shape.
 */
public class ConcaveVsConvexAlgorithm extends NarrowPhaseAlgorithm {
	/**
	 *  This class is used to encapsulate a callback method for
	 * collision detection between the triangle of a concave mesh shape
	 * and a convex shape.
	 */
	class ConvexVsTriangleCallback implements ConcaveShape.TriangleCallback {
		
		protected CollisionDetection collisionDetection; //!< Pointer to the collision detection object
		protected NarrowPhaseCallback narrowPhaseCallback; //!< Narrow-phase collision callback
		protected ConvexShape convexShape; //!< Convex collision shape to test collision with
		protected ConcaveShape concaveShape; //!< Concave collision shape
		protected ProxyShape convexProxyShape; //!< Proxy shape of the convex collision shape
		protected ProxyShape concaveProxyShape; //!< Proxy shape of the concave collision shape
		protected OverlappingPair overlappingPair; //!< Broadphase overlapping pair
		
		//  protected static boolean contactsDepthCompare(ContactPointInfo _contact1, ContactPointInfo _contact2);
		
		/// Set the collision detection pointer
		public void setCollisionDetection(final CollisionDetection _collisionDetection) {
			this.collisionDetection = _collisionDetection;
		}
		
		/// Set the concave collision shape
		public void setConcaveShape(final ConcaveShape _concaveShape) {
			this.concaveShape = _concaveShape;
		}
		
		/// Set the convex collision shape to test collision with
		public void setConvexShape(final ConvexShape _convexShape) {
			this.convexShape = _convexShape;
		}
		
		/// Set the narrow-phase collision callback
		public void setNarrowPhaseCallback(final NarrowPhaseCallback _callback) {
			this.narrowPhaseCallback = _callback;
		}
		
		/// Set the broadphase overlapping pair
		public void setOverlappingPair(final OverlappingPair _overlappingPair) {
			this.overlappingPair = _overlappingPair;
		}
		
		/// Set the proxy shapes of the two collision shapes
		public void setProxyShapes(final ProxyShape _convexProxyShape, final ProxyShape _concaveProxyShape) {
			this.convexProxyShape = _convexProxyShape;
			this.concaveProxyShape = _concaveProxyShape;
		}
		
		/// Test collision between a triangle and the convex mesh shape
		@Override
		public void testTriangle(final Vector3f[] _trianglePoints) {
			// Create a triangle collision shape
			final float margin = this.concaveShape.getTriangleMargin();
			
			final TriangleShape triangleShape = new TriangleShape(_trianglePoints[0], _trianglePoints[1], _trianglePoints[2], margin);
			// Select the collision algorithm to use between the triangle and the convex shape
			final NarrowPhaseAlgorithm algo = this.collisionDetection.getCollisionAlgorithm(triangleShape.getType(), this.convexShape.getType());
			// If there is no collision algorithm between those two kinds of shapes
			if (algo == null) {
				return;
			}
			// Notify the narrow-phase algorithm about the overlapping pair we are going to test
			algo.setCurrentOverlappingPair(this.overlappingPair);
			// Create the CollisionShapeInfo objects
			final CollisionShapeInfo shapeConvexInfo = new CollisionShapeInfo(this.convexProxyShape, this.convexShape, this.convexProxyShape.getLocalToWorldTransform(), this.overlappingPair,
					this.convexProxyShape.getCachedCollisionData());
			
			final CollisionShapeInfo shapeConcaveInfo = new CollisionShapeInfo(this.concaveProxyShape, triangleShape, this.concaveProxyShape.getLocalToWorldTransform(), this.overlappingPair,
					this.concaveProxyShape.getCachedCollisionData());
			// Use the collision algorithm to test collision between the triangle and the other convex shape
			algo.testCollision(shapeConvexInfo, shapeConcaveInfo, this.narrowPhaseCallback);
		}
		
	}
	
	/**
	 *  This class is used as a narrow-phase callback to get narrow-phase contacts
	 * of the concave triangle mesh to temporary store them in order to be used in
	 * the smooth mesh collision algorithm if this one is enabled.
	 */
	class SmoothCollisionNarrowPhaseCallback implements NarrowPhaseCallback {
		private final List<SmoothMeshContactInfo> contactPoints;
		
		// Constructor
		public SmoothCollisionNarrowPhaseCallback(final List<SmoothMeshContactInfo> _contactPoints) {
			this.contactPoints = new ArrayList<>(_contactPoints);
		}
		
		/// Called by a narrow-phase collision algorithm when a new contact has been found
		@Override
		public void notifyContact(final OverlappingPair _overlappingPair, final ContactPointInfo _contactInfo) {
			final Vector3f[] triangleVertices = new Vector3f[3];
			boolean isFirstShapeTriangle;
			// If the collision shape 1 is the triangle
			if (_contactInfo.collisionShape1.getType() == CollisionShapeType.TRIANGLE) {
				assert (_contactInfo.collisionShape2.getType() != CollisionShapeType.TRIANGLE);
				final TriangleShape triangleShape = (TriangleShape) _contactInfo.collisionShape1;
				triangleVertices[0] = triangleShape.getVertex(0);
				triangleVertices[1] = triangleShape.getVertex(1);
				triangleVertices[2] = triangleShape.getVertex(2);
				isFirstShapeTriangle = true;
			} else { // If the collision shape 2 is the triangle
				assert (_contactInfo.collisionShape2.getType() == CollisionShapeType.TRIANGLE);
				final TriangleShape triangleShape = (TriangleShape) _contactInfo.collisionShape2;
				triangleVertices[0] = triangleShape.getVertex(0);
				triangleVertices[1] = triangleShape.getVertex(1);
				triangleVertices[2] = triangleShape.getVertex(2);
				isFirstShapeTriangle = false;
			}
			
			final SmoothMeshContactInfo smoothContactInfo = new SmoothMeshContactInfo(_contactInfo, isFirstShapeTriangle, triangleVertices[0], triangleVertices[1], triangleVertices[2]);
			// Add the narrow-phase contact into the list of contact to process for
			// smooth mesh collision
			this.contactPoints.add(smoothContactInfo);
		}
		
	}
	
	/**
	 *  This class is used to store data about a contact with a triangle for the smooth
	 * mesh algorithm.
	 */
	class SmoothMeshContactInfo {
		public ContactPointInfo contactInfo;
		public boolean isFirstShapeTriangle;
		
		public Vector3f[] triangleVertices = new Vector3f[3];
		
		public SmoothMeshContactInfo() {
			// TODO: add it for List
		}
		
		/// Constructor
		public SmoothMeshContactInfo(final ContactPointInfo _contact, final boolean _firstShapeTriangle, final Vector3f _trianglePoint1, final Vector3f _trianglePoint2,
				final Vector3f _trianglePoint3) {
			this.contactInfo = new ContactPointInfo(_contact);
			this.isFirstShapeTriangle = _firstShapeTriangle;
			this.triangleVertices[0] = _trianglePoint1;
			this.triangleVertices[1] = _trianglePoint2;
			this.triangleVertices[2] = _trianglePoint3;
		}
	}
	
	/// Constructor
	public ConcaveVsConvexAlgorithm(final CollisionDetection collisionDetection) {
		super(collisionDetection);
	}
	
	/// Add a triangle vertex into the set of processed triangles
	protected void addProcessedVertex(final List<PairIntVector3f> _processTriangleVertices, final Vector3f _vertex) {
		_processTriangleVertices.add(new PairIntVector3f((int) (_vertex.x * _vertex.y * _vertex.z), _vertex));
	}
	
	/// Return true if the vertex is in the set of already processed vertices
	protected boolean hasVertexBeenProcessed(final List<PairIntVector3f> _processTriangleVertices, final Vector3f _vertex) {
		/* TODO : List<etk::Pair<int, Vector3f>> was an unordered map ... ==> stupid idee... I replace code because I do not have enouth time to do something good...
		int key = int(_vertex.x * _vertex.y * _vertex.z);
		auto range = _processTriangleVertices.equal_range(key);
		for (auto it = range.first; it != range.second; ++it) {
			if (    _vertex.x == it.second.x
			     && _vertex.y == it.second.y
			     && _vertex.z == it.second.z) {
				return true;
			}
		}
		return false;
		*/
		// TODO : This is not really the same ...
		for (final PairIntVector3f it : _processTriangleVertices) {
			if (_vertex.x == it.second.x && _vertex.y == it.second.y && _vertex.z == it.second.z) {
				return true;
			}
		}
		return false;
	}
	
	/// Process the concave triangle mesh collision using the smooth mesh collision algorithm
	protected void processSmoothMeshCollision(final OverlappingPair _overlappingPair, final List<SmoothMeshContactInfo> _contactPoints, final NarrowPhaseCallback _callback) {
		// Set with the triangle vertices already processed to void further contacts with same triangle
		final List<PairIntVector3f> processTriangleVertices = new ArrayList<>();
		// Sort the list of narrow-phase contacts according to their penetration depth
		_contactPoints.sort(new Comparator<SmoothMeshContactInfo>() {
			@Override
			public int compare(final SmoothMeshContactInfo _pair1, final SmoothMeshContactInfo _pair2) {
				if (_pair1.contactInfo.penetrationDepth < _pair2.contactInfo.penetrationDepth) {
					return -1;
				}
				if (_pair1.contactInfo.penetrationDepth == _pair2.contactInfo.penetrationDepth) {
					return 0;
				}
				return +1;
			}
		});
		
		// For each contact point (from smaller penetration depth to larger)
		for (final SmoothMeshContactInfo info : _contactPoints) {
			final Vector3f contactPoint = info.isFirstShapeTriangle ? info.contactInfo.localPoint1 : info.contactInfo.localPoint2;
			// Compute the barycentric coordinates of the point in the triangle
			final Float u = 0.0f, v = 0.0f, w = 0.0f;
			Mathematics.computeBarycentricCoordinatesInTriangle(info.triangleVertices[0], info.triangleVertices[1], info.triangleVertices[2], contactPoint, u, v, w);
			int nbZeros = 0;
			final boolean isUZero = Mathematics.ApproxEqual(u, 0.0f, 0.0001f);
			final boolean isVZero = Mathematics.ApproxEqual(v, 0.0f, 0.0001f);
			final boolean isWZero = Mathematics.ApproxEqual(w, 0.0f, 0.0001f);
			if (isUZero) {
				nbZeros++;
			}
			if (isVZero) {
				nbZeros++;
			}
			if (isWZero) {
				nbZeros++;
			}
			// If it is a vertex contact
			if (nbZeros == 2) {
				final Vector3f contactVertex = !isUZero ? info.triangleVertices[0] : (!isVZero ? info.triangleVertices[1] : info.triangleVertices[2]);
				// Check that this triangle vertex has not been processed yet
				if (!hasVertexBeenProcessed(processTriangleVertices, contactVertex)) {
					// Keep the contact as it is and report it
					_callback.notifyContact(_overlappingPair, info.contactInfo);
				}
			} else if (nbZeros == 1) {
				// If it is an edge contact
				final Vector3f contactVertex1 = isUZero ? info.triangleVertices[1] : (isVZero ? info.triangleVertices[0] : info.triangleVertices[0]);
				final Vector3f contactVertex2 = isUZero ? info.triangleVertices[2] : (isVZero ? info.triangleVertices[2] : info.triangleVertices[1]);
				// Check that this triangle edge has not been processed yet
				if (!hasVertexBeenProcessed(processTriangleVertices, contactVertex1) && !hasVertexBeenProcessed(processTriangleVertices, contactVertex2)) {
					// Keep the contact as it is and report it
					_callback.notifyContact(_overlappingPair, info.contactInfo);
				}
			} else {
				// If it is a face contact
				final ContactPointInfo newContactInfo = new ContactPointInfo(info.contactInfo);
				ProxyShape firstShape;
				ProxyShape secondShape;
				if (info.isFirstShapeTriangle) {
					firstShape = _overlappingPair.getShape1();
					secondShape = _overlappingPair.getShape2();
				} else {
					firstShape = _overlappingPair.getShape2();
					secondShape = _overlappingPair.getShape1();
				}
				// We use the triangle normal as the contact normal
				final Vector3f a = info.triangleVertices[1].lessNew(info.triangleVertices[0]);
				final Vector3f b = info.triangleVertices[2].lessNew(info.triangleVertices[0]);
				final Vector3f localNormal = a.cross(b);
				newContactInfo.normal = firstShape.getLocalToWorldTransform().getOrientation().multiply(localNormal);
				final Vector3f firstLocalPoint = info.isFirstShapeTriangle ? info.contactInfo.localPoint1 : info.contactInfo.localPoint2;
				final Vector3f firstWorldPoint = firstShape.getLocalToWorldTransform().multiplyNew(firstLocalPoint);
				newContactInfo.normal.normalize();
				if (newContactInfo.normal.dot(info.contactInfo.normal) < 0) {
					newContactInfo.normal.multiply(-1);
				}
				// We recompute the contact point on the second body with the new normal as described in
				// the Smooth Mesh Contacts with GJK of the Game Physics Pearls book (from Gino van Den Bergen and
				// Dirk Gregorius) to avoid adding torque
				final Transform3D worldToLocalSecondPoint = secondShape.getLocalToWorldTransform().inverseNew();
				if (info.isFirstShapeTriangle) {
					final Vector3f newSecondWorldPoint = firstWorldPoint.addNew(newContactInfo.normal);
					newContactInfo.localPoint2 = worldToLocalSecondPoint.multiplyNew(newSecondWorldPoint);
				} else {
					final Vector3f newSecondWorldPoint = firstWorldPoint.lessNew(newContactInfo.normal);
					newContactInfo.localPoint1 = worldToLocalSecondPoint.multiplyNew(newSecondWorldPoint);
				}
				// Report the contact
				_callback.notifyContact(_overlappingPair, newContactInfo);
			}
			
			// Add the three vertices of the triangle to the set of processed
			// triangle vertices
			addProcessedVertex(processTriangleVertices, info.triangleVertices[0]);
			addProcessedVertex(processTriangleVertices, info.triangleVertices[1]);
			addProcessedVertex(processTriangleVertices, info.triangleVertices[2]);
		}
		
	}
	
	/// Compute a contact info if the two bounding volume collide
	@Override
	public void testCollision(final CollisionShapeInfo _shape1Info, final CollisionShapeInfo _shape2Info, final NarrowPhaseCallback _callback) {
		ProxyShape convexProxyShape;
		ProxyShape concaveProxyShape;
		ConvexShape convexShape;
		ConcaveShape concaveShape;
		// Collision shape 1 is convex, collision shape 2 is concave
		if (_shape1Info.collisionShape.isConvex()) {
			convexProxyShape = _shape1Info.proxyShape;
			convexShape = (ConvexShape) _shape1Info.collisionShape;
			concaveProxyShape = _shape2Info.proxyShape;
			concaveShape = (ConcaveShape) _shape2Info.collisionShape;
		} else {
			// Collision shape 2 is convex, collision shape 1 is concave
			convexProxyShape = _shape2Info.proxyShape;
			convexShape = (ConvexShape) _shape2Info.collisionShape;
			concaveProxyShape = _shape1Info.proxyShape;
			concaveShape = (ConcaveShape) _shape1Info.collisionShape;
		}
		// Set the parameters of the callback object
		final ConvexVsTriangleCallback convexVsTriangleCallback = new ConvexVsTriangleCallback();
		convexVsTriangleCallback.setCollisionDetection(this.collisionDetection);
		convexVsTriangleCallback.setConvexShape(convexShape);
		convexVsTriangleCallback.setConcaveShape(concaveShape);
		convexVsTriangleCallback.setProxyShapes(convexProxyShape, concaveProxyShape);
		convexVsTriangleCallback.setOverlappingPair(_shape1Info.overlappingPair);
		// Compute the convex shape AABB in the local-space of the convex shape
		final AABB aabb = new AABB();
		convexShape.computeAABB(aabb, convexProxyShape.getLocalToWorldTransform());
		// If smooth mesh collision is enabled for the concave mesh
		if (concaveShape.getIsSmoothMeshCollisionEnabled()) {
			final List<SmoothMeshContactInfo> contactPoints = new ArrayList<>();
			
			final SmoothCollisionNarrowPhaseCallback smoothNarrowPhaseCallback = new SmoothCollisionNarrowPhaseCallback(contactPoints);
			convexVsTriangleCallback.setNarrowPhaseCallback(smoothNarrowPhaseCallback);
			// Call the convex vs triangle callback for each triangle of the concave shape
			concaveShape.testAllTriangles(convexVsTriangleCallback, aabb);
			// Run the smooth mesh collision algorithm
			processSmoothMeshCollision(_shape1Info.overlappingPair, contactPoints, _callback);
		} else {
			convexVsTriangleCallback.setNarrowPhaseCallback(_callback);
			// Call the convex vs triangle callback for each triangle of the concave shape
			concaveShape.testAllTriangles(convexVsTriangleCallback, aabb);
		}
	}
}

package org.atriasoft.ephysics.collision.shapes;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.ephysics.RaycastInfo;
import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.collision.Triangle;
import org.atriasoft.ephysics.collision.TriangleMesh;
import org.atriasoft.ephysics.collision.TriangleVertexArray;
import org.atriasoft.ephysics.collision.broadphase.CallbackOverlapping;
import org.atriasoft.ephysics.collision.broadphase.CallbackRaycast;
import org.atriasoft.ephysics.collision.broadphase.DTree;
import org.atriasoft.ephysics.collision.broadphase.DynamicAABBTree;
import org.atriasoft.ephysics.collision.shapes.TriangleShape.TriangleRaycastSide;
import org.atriasoft.ephysics.internal.Log;
import org.atriasoft.ephysics.mathematics.Ray;
import org.atriasoft.etk.math.Matrix3f;
import org.atriasoft.etk.math.Vector3f;

/**
 *  Represents a static concave mesh shape. Note that collision detection
 * with a concave mesh shape can be very expensive. You should use only use
 * this shape for a static mesh.
 */
public class ConcaveMeshShape extends ConcaveShape {
	class ConcaveMeshRaycastCallback {
		private final List<DTree> hitAABBNodes = new ArrayList<>();
		private final DynamicAABBTree dynamicAABBTree;
		private final ConcaveMeshShape concaveMeshShape;
		private final ProxyShape proxyShape;
		private final RaycastInfo raycastInfo;
		private final Ray ray;
		private boolean isHit;
		
		// Constructor
		ConcaveMeshRaycastCallback(final DynamicAABBTree _dynamicAABBTree, final ConcaveMeshShape _concaveMeshShape, final ProxyShape _proxyShape, final RaycastInfo _raycastInfo, final Ray _ray) {
			this.dynamicAABBTree = _dynamicAABBTree;
			this.concaveMeshShape = _concaveMeshShape;
			this.proxyShape = _proxyShape;
			this.raycastInfo = _raycastInfo;
			this.ray = _ray;
			this.isHit = false;
			
		}
		
		/// Return true if a raycast hit has been found
		public boolean getIsHit() {
			return this.isHit;
		}
		
		/// Collect all the AABB nodes that are hit by the ray in the Dynamic AABB Tree
		public float operator__parenthese(final DTree _node, final Ray _ray) {
			// Add the id of the hit AABB node longo
			this.hitAABBNodes.add(_node);
			return _ray.maxFraction;
		}
		
		/// Raycast all collision shapes that have been collected
		public void raycastTriangles() {
			float smallestHitFraction = this.ray.maxFraction;
			for (final DTree it : this.hitAABBNodes) {
				// Get the node data (triangle index and mesh subpart index)
				final int data_0 = this.dynamicAABBTree.getNodeDataInt_0(it);
				final int data_1 = this.dynamicAABBTree.getNodeDataInt_1(it);
				// Get the triangle vertices for this node from the concave mesh shape
				final Vector3f[] trianglePoints = new Vector3f[3];
				this.concaveMeshShape.getTriangleVerticesWithIndexPointer(data_0, data_1, trianglePoints);
				// Create a triangle collision shape
				final float margin = this.concaveMeshShape.getTriangleMargin();
				
				final TriangleShape triangleShape = new TriangleShape(trianglePoints[0], trianglePoints[1], trianglePoints[2], margin);
				triangleShape.setRaycastTestType(this.concaveMeshShape.getRaycastTestType());
				// Ray casting test against the collision shape
				final RaycastInfo raycastInfo = new RaycastInfo();
				final boolean isTriangleHit = triangleShape.raycast(this.ray, raycastInfo, this.proxyShape);
				// If the ray hit the collision shape
				if (isTriangleHit && raycastInfo.hitFraction <= smallestHitFraction) {
					assert (raycastInfo.hitFraction >= 0.0f);
					this.raycastInfo.body = raycastInfo.body;
					this.raycastInfo.proxyShape = raycastInfo.proxyShape;
					this.raycastInfo.hitFraction = raycastInfo.hitFraction;
					this.raycastInfo.worldPoint = raycastInfo.worldPoint;
					this.raycastInfo.worldNormal = raycastInfo.worldNormal;
					this.raycastInfo.meshSubpart = data_0;
					this.raycastInfo.triangleIndex = data_1;
					smallestHitFraction = raycastInfo.hitFraction;
					this.isHit = true;
				}
			}
		}
		
	};
	
	protected TriangleMesh triangleMesh; //!< Triangle mesh
	
	protected DynamicAABBTree dynamicAABBTree; //!< Dynamic AABB tree to accelerate collision with the triangles
	
	public ConcaveMeshShape(final TriangleMesh _triangleMesh) {
		super(CollisionShapeType.CONCAVE_MESH);
		this.triangleMesh = _triangleMesh;
		this.raycastTestType = TriangleRaycastSide.FRONT;
		initBVHTree();
	}
	
	@Override
	public void computeLocalInertiaTensor(final Matrix3f _tensor, final float _mass) {
		// Default inertia tensor
		// Note that this is not very realistic for a concave triangle mesh.
		// However, in most cases, it will only be used static bodies and therefore,
		// the inertia tensor is not used.
		_tensor.set(_mass, 0.0f, 0.0f, 0.0f, _mass, 0.0f, 0.0f, 0.0f, _mass);
	}
	
	@Override
	public void getLocalBounds(final Vector3f _min, final Vector3f _max) {
		// Get the AABB of the whole tree
		final AABB treeAABB = this.dynamicAABBTree.getRootAABB();
		_min.set(treeAABB.getMin());
		_max.set(treeAABB.getMax());
	}
	
	/// Return the three vertices coordinates (in the array outTriangleVertices) of a triangle
	/// given the start vertex index pointer of the triangle.
	protected void getTriangleVerticesWithIndexPointer(final int _subPart, final int _triangleIndex, final Vector3f[] _outTriangleVertices) {
		assert (_outTriangleVertices != null);
		// Get the triangle vertex array of the current sub-part
		final TriangleVertexArray triangleVertexArray = this.triangleMesh.getSubpart(_subPart);
		if (triangleVertexArray == null) {
			Log.error("get null ...");
		}
		final Triangle trianglePoints = triangleVertexArray.getTriangle(_triangleIndex);
		_outTriangleVertices[0] = trianglePoints.get(0).multiplyNew(this.scaling);
		_outTriangleVertices[1] = trianglePoints.get(1).multiplyNew(this.scaling);
		_outTriangleVertices[2] = trianglePoints.get(2).multiplyNew(this.scaling);
	}
	
	/// Insert all the triangles longo the dynamic AABB tree
	protected void initBVHTree() {
		// TODO : Try to randomly add the triangles into the tree to obtain a better tree
		// For each sub-part of the mesh
		for (int subPart = 0; subPart < this.triangleMesh.getNbSubparts(); subPart++) {
			// Get the triangle vertex array of the current sub-part
			final TriangleVertexArray triangleVertexArray = this.triangleMesh.getSubpart(subPart);
			// For each triangle of the concave mesh
			for (int iii = 0; iii < triangleVertexArray.getNbTriangles(); ++iii) {
				final Triangle trianglePoints = triangleVertexArray.getTriangle(iii);
				final Vector3f[] trianglePoints2 = new Vector3f[3];
				trianglePoints2[0] = trianglePoints.get(0);
				trianglePoints2[1] = trianglePoints.get(1);
				trianglePoints2[2] = trianglePoints.get(2);
				// Create the AABB for the triangle
				final AABB aabb = AABB.createAABBForTriangle(trianglePoints2);
				aabb.inflate(this.triangleMargin, this.triangleMargin, this.triangleMargin);
				// Add the AABB with the index of the triangle longo the dynamic AABB tree
				this.dynamicAABBTree.addObject(aabb, subPart, iii);
			}
		}
	}
	
	@Override
	public boolean isConvex() {
		return false;
	}
	
	@Override
	public boolean raycast(final Ray _ray, final RaycastInfo _raycastInfo, final ProxyShape _proxyShape) {
		// Create the callback object that will compute ray casting against triangles
		final ConcaveMeshRaycastCallback raycastCallback = new ConcaveMeshRaycastCallback(this.dynamicAABBTree, this, _proxyShape, _raycastInfo, _ray);
		// Ask the Dynamic AABB Tree to report all AABB nodes that are hit by the ray.
		// The raycastCallback object will then compute ray casting against the triangles
		// in the hit AABBs.
		this.dynamicAABBTree.raycast(_ray, new CallbackRaycast() {
			
			@Override
			public float callback(final DTree _node, final Ray _ray) {
				return raycastCallback.operator__parenthese(_node, _ray);
			}
		});
		raycastCallback.raycastTriangles();
		return raycastCallback.getIsHit();
	}
	
	@Override
	public void setLocalScaling(final Vector3f _scaling) {
		super.setLocalScaling(_scaling);
		this.dynamicAABBTree.reset();
		initBVHTree();
	}
	
	@Override
	public void testAllTriangles(final ConcaveShape.TriangleCallback _callback, final AABB _localAABB) {
		// Ask the Dynamic AABB Tree to report all the triangles that are overlapping
		// with the AABB of the convex shape.
		final ConcaveMeshShape self = this;
		this.dynamicAABBTree.reportAllShapesOverlappingWithAABB(_localAABB, new CallbackOverlapping() {
			@Override
			public void callback(final DTree _node) {
				// Get the node data (triangle index and mesh subpart index)
				final int data_0 = self.dynamicAABBTree.getNodeDataInt_0(_node);
				final int data_1 = self.dynamicAABBTree.getNodeDataInt_1(_node);
				// Get the triangle vertices for this node from the concave mesh shape
				final Vector3f[] trianglePoints = new Vector3f[3];
				getTriangleVerticesWithIndexPointer(data_0, data_1, trianglePoints);
				// Call the callback to test narrow-phase collision with this triangle
				_callback.testTriangle(trianglePoints);
			}
		});
	}
}

package org.atriasoft.ephysics.collision.shapes;

import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix3f;
import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.RaycastInfo;
import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.mathematics.Ray;

/**
 *  This class represents a static height field that can be used to represent
 * a terrain. The height field is made of a grid with rows and columns with a
 * height value at each grid point. Note that the height values are not copied longo the shape
 * but are shared instead. The height values can be of type longeger, float or double.
 * When creating a HeightFieldShape, you need to specify the minimum and maximum height value of
 * your height field. Note that the HeightFieldShape will be re-centered based on its AABB. It means
 * that for instance, if the minimum height value is -200 and the maximum value is 400, the final
 * minimum height of the field in the simulation will be -300 and the maximum height will be 300.
 */
public class HeightFieldShape extends ConcaveShape {
	
	/**
	 *  This class is used for testing AABB and triangle overlap for raycasting
	 */
	public class TriangleOverlapCallback implements TriangleCallback {
		protected Ray ray;
		protected ProxyShape proxyShape;
		protected RaycastInfo raycastInfo;
		protected boolean isHit;
		protected float smallestHitFraction;
		protected HeightFieldShape heightFieldShape;
		
		public TriangleOverlapCallback(final Ray _ray, final ProxyShape _proxyShape, final RaycastInfo _raycastInfo, final HeightFieldShape _heightFieldShape) {
			this.ray = _ray;
			this.proxyShape = _proxyShape;
			this.raycastInfo = _raycastInfo;
			this.heightFieldShape = _heightFieldShape;
			this.isHit = false;
			this.smallestHitFraction = this.ray.maxFraction;
		}
		
		public boolean getIsHit() {
			return this.isHit;
		}
		
		/// Raycast test between a ray and a triangle of the heightfield
		@Override
		public void testTriangle(final Vector3f[] _trianglePoints) {
			// Create a triangle collision shape
			final float margin = this.heightFieldShape.getTriangleMargin();
			final TriangleShape triangleShape = new TriangleShape(_trianglePoints[0], _trianglePoints[1], _trianglePoints[2], margin);
			triangleShape.setRaycastTestType(this.heightFieldShape.getRaycastTestType());
			// Ray casting test against the collision shape
			final RaycastInfo raycastInfo = new RaycastInfo();
			final boolean isTriangleHit = triangleShape.raycast(this.ray, raycastInfo, this.proxyShape);
			// If the ray hit the collision shape
			if (isTriangleHit && raycastInfo.hitFraction <= this.smallestHitFraction) {
				assert (raycastInfo.hitFraction >= 0.0f);
				this.raycastInfo.body = raycastInfo.body;
				this.raycastInfo.proxyShape = raycastInfo.proxyShape;
				this.raycastInfo.hitFraction = raycastInfo.hitFraction;
				this.raycastInfo.worldPoint = raycastInfo.worldPoint;
				this.raycastInfo.worldNormal = raycastInfo.worldNormal;
				this.raycastInfo.meshSubpart = -1;
				this.raycastInfo.triangleIndex = -1;
				this.smallestHitFraction = raycastInfo.hitFraction;
				this.isHit = true;
			}
		}
	};
	
	protected int numberColumns; //!< Number of columns in the grid of the height field
	protected int numberRows; //!< Number of rows in the grid of the height field
	
	protected float width; //!< Height field width
	protected float length; //!< Height field length
	protected float minHeight; //!< Minimum height of the height field
	protected float maxHeight; //!< Maximum height of the height field
	protected int upAxis; //!< Up axis direction (0 => x, 1 => y, 2 => z)
	protected float[] heightFieldData; //!< Array of data with all the height values of the height field
	protected AABB AABB; //!< Local AABB of the height field (without scaling)
	
	/**
		 *  Contructor
		 * @param nbGridColumns Number of columns in the grid of the height field
		 * @param nbGridRows Number of rows in the grid of the height field
		 * @param minHeight Minimum height value of the height field
		 * @param maxHeight Maximum height value of the height field
		 * @param heightFieldData Pointer to the first height value data (note that values are shared and not copied)
		 * @param dataType Data type for the height values (long, float, double)
		 * @param upAxis Integer representing the up axis direction (0 for x, 1 for y and 2 for z)
		 * @param longegerHeightScale Scaling factor used to scale the height values (only when height values type is longeger)
		 */
	public HeightFieldShape(final int _nbGridColumns, final int _nbGridRows, final float _minHeight, final float _maxHeight, final float[] _heightFieldData) {
		this(_nbGridColumns, _nbGridRows, _minHeight, _maxHeight, _heightFieldData, 1);
	}
	
	/// Insert all the triangles longo the dynamic AABB tree
	//protected void initBVHTree();
	
	public HeightFieldShape(final int _nbGridColumns, final int _nbGridRows, final float _minHeight, final float _maxHeight, final float[] _heightFieldData, final int _upAxis) {
		super(CollisionShapeType.HEIGHTFIELD);
		this.numberColumns = _nbGridColumns;
		this.numberRows = _nbGridRows;
		this.width = _nbGridColumns - 1;
		this.length = _nbGridRows - 1;
		this.minHeight = _minHeight;
		this.maxHeight = _maxHeight;
		this.upAxis = _upAxis;
		assert (_nbGridColumns >= 2);
		assert (_nbGridRows >= 2);
		assert (this.width >= 1);
		assert (this.length >= 1);
		assert (_minHeight <= _maxHeight);
		assert (_upAxis == 0 || _upAxis == 1 || _upAxis == 2);
		this.heightFieldData = _heightFieldData;
		final float halfHeight = (this.maxHeight - this.minHeight) * 0.5f;
		assert (halfHeight >= 0);
		// Compute the local AABB of the height field
		if (this.upAxis == 0) {
			this.AABB.setMin(new Vector3f(-halfHeight, -this.width * 0.5f, -this.length * 0.5f));
			this.AABB.setMax(new Vector3f(halfHeight, this.width * 0.5f, this.length * 0.5f));
		} else if (this.upAxis == 1) {
			this.AABB.setMin(new Vector3f(-this.width * 0.5f, -halfHeight, -this.length * 0.5f));
			this.AABB.setMax(new Vector3f(this.width * 0.5f, halfHeight, this.length * 0.5f));
		} else if (this.upAxis == 2) {
			this.AABB.setMin(new Vector3f(-this.width * 0.5f, -this.length * 0.5f, -halfHeight));
			this.AABB.setMax(new Vector3f(this.width * 0.5f, this.length * 0.5f, halfHeight));
		}
	}
	
	/// Return the closest inside longeger grid value of a given floating grid value
	protected int computeIntegerGridValue(final float _value) {
		return (int) ((_value < 0.0f) ? _value - 0.5f : _value + 0.5f);
	}
	
	@Override
	public void computeLocalInertiaTensor(final Matrix3f _tensor, final float _mass) {
		// Default inertia tensor
		// Note that this is not very realistic for a concave triangle mesh.
		// However, in most cases, it will only be used static bodies and therefore,
		// the inertia tensor is not used.
		_tensor.set(_mass, 0.0f, 0.0f, 0.0f, _mass, 0.0f, 0.0f, 0.0f, _mass);
	}
	
	/// Compute the min/max grid coords corresponding to the longersection of the AABB of the height field and the AABB to collide
	protected void computeMinMaxGridCoordinates(final Vector3f _minCoords, final Vector3f _maxCoords, final AABB _aabbToCollide) {
		// Clamp the min/max coords of the AABB to collide inside the height field AABB
		Vector3f minPoint = FMath.max(_aabbToCollide.getMin(), this.AABB.getMin());
		minPoint = FMath.min(minPoint, this.AABB.getMax());
		Vector3f maxPoint = FMath.min(_aabbToCollide.getMax(), this.AABB.getMax());
		maxPoint = FMath.max(maxPoint, this.AABB.getMin());
		// Translate the min/max points such that the we compute grid points from [0 ... mNbWidthGridPoints]
		// and from [0 ... mNbLengthGridPoints] because the AABB coordinates range are [-mWdith/2 ... this.width/2]
		// and [-this.length/2 ... this.length/2]
		final Vector3f translateVec = this.AABB.getExtent().multiplyNew(0.5f);
		minPoint.add(translateVec);
		maxPoint.add(translateVec);
		// Convert the floating min/max coords of the AABB longo closest longeger
		// grid values (note that we use the closest grid coordinate that is out
		// of the AABB)
		_minCoords.set(computeIntegerGridValue(minPoint.x) - 1, computeIntegerGridValue(minPoint.y) - 1, computeIntegerGridValue(minPoint.z) - 1);
		_maxCoords.set(computeIntegerGridValue(maxPoint.x) + 1, computeIntegerGridValue(maxPoint.y) + 1, computeIntegerGridValue(maxPoint.z) + 1);
	}
	
	/// Return the height of a given (x,y) point in the height field
	protected float getHeightAt(final int _xxx, final int _yyy) {
		return this.heightFieldData[_yyy * this.numberColumns + _xxx];
	}
	
	@Override
	public void getLocalBounds(final Vector3f _min, final Vector3f _max) {
		_min.set(this.AABB.getMin().multiplyNew(this.scaling));
		_max.set(this.AABB.getMax().multiplyNew(this.scaling));
	}
	
	/// Return the number of columns in the height field
	public long getNbColumns() {
		return this.numberColumns;
	}
	
	/// Return the number of rows in the height field
	public long getNbRows() {
		return this.numberRows;
	}
	
	/// Return the three vertices coordinates (in the array outTriangleVertices) of a triangle
	/// given the start vertex index pointer of the triangle.
	/*
	protected		void getTriangleVerticesWithIndexPointer(final long _subPart,
		                                         final long _triangleIndex,
		                                         Vector3f* _outTriangleVertices) ;
		                                         */
	/// Return the vertex (local-coordinates) of the height field at a given (x,y) position
	protected Vector3f getVertexAt(final int _xxx, final int _yyy) {
		// Get the height value
		final float height = getHeightAt(_xxx, _yyy);
		// Height values origin
		final float heightOrigin = -(this.maxHeight - this.minHeight) * 0.5f - this.minHeight;
		Vector3f vertex = null;
		switch (this.upAxis) {
			case 0:
				vertex = new Vector3f(heightOrigin + height, -this.width * 0.5f + _xxx, -this.length * 0.5f + _yyy);
				break;
			case 1:
				vertex = new Vector3f(-this.width * 0.5f + _xxx, heightOrigin + height, -this.length * 0.5f + _yyy);
				break;
			case 2:
				vertex = new Vector3f(-this.width * 0.5f + _xxx, -this.length * 0.5f + _yyy, heightOrigin + height);
				break;
			default:
				assert (false);
		}
		assert (this.AABB.contains(vertex));
		return vertex.multiply(this.scaling);
	}
	
	@Override
	public boolean isConvex() {
		return false;
	}
	
	@Override
	public boolean raycast(final Ray _ray, final RaycastInfo _raycastInfo, final ProxyShape _proxyShape) {
		// TODO : Implement raycasting without using an AABB for the ray
		//		but using a dynamic AABB tree or octree instead
		
		final TriangleOverlapCallback triangleCallback = new TriangleOverlapCallback(_ray, _proxyShape, _raycastInfo, this);
		// Compute the AABB for the ray
		final Vector3f rayEnd = _ray.point2.lessNew(_ray.point1).multiply(_ray.maxFraction).add(_ray.point1);
		
		final AABB rayAABB = new AABB(FMath.min(_ray.point1, rayEnd), FMath.max(_ray.point1, rayEnd));
		testAllTriangles(triangleCallback, rayAABB);
		return triangleCallback.getIsHit();
	}
	
	@Override
	public void setLocalScaling(final Vector3f _scaling) {
		super.setLocalScaling(_scaling);
	}
	
	@Override
	public void testAllTriangles(final TriangleCallback _callback, final AABB _localAABB) {
		
		// Compute the non-scaled AABB
		final Vector3f inverseScaling = new Vector3f(1.0f / this.scaling.x, 1.0f / this.scaling.y, 1.0f / this.scaling.z);
		
		final AABB aabb = new AABB(_localAABB.getMin().multiplyNew(inverseScaling), _localAABB.getMax().multiplyNew(inverseScaling));
		// Compute the longeger grid coordinates inside the area we need to test for collision
		final Vector3f minGridCoords = new Vector3f();
		final Vector3f maxGridCoords = new Vector3f();
		computeMinMaxGridCoordinates(minGridCoords, maxGridCoords, aabb);
		// Compute the starting and ending coords of the sub-grid according to the up axis
		int iMin = 0;
		int iMax = 0;
		int jMin = 0;
		int jMax = 0;
		switch (this.upAxis) {
			case 0:
				iMin = FMath.clamp((int) minGridCoords.y, 0, this.numberColumns - 1);
				iMax = FMath.clamp((int) maxGridCoords.y, 0, this.numberColumns - 1);
				jMin = FMath.clamp((int) minGridCoords.z, 0, this.numberRows - 1);
				jMax = FMath.clamp((int) maxGridCoords.z, 0, this.numberRows - 1);
				break;
			case 1:
				iMin = FMath.clamp((int) minGridCoords.x, 0, this.numberColumns - 1);
				iMax = FMath.clamp((int) maxGridCoords.x, 0, this.numberColumns - 1);
				jMin = FMath.clamp((int) minGridCoords.z, 0, this.numberRows - 1);
				jMax = FMath.clamp((int) maxGridCoords.z, 0, this.numberRows - 1);
				break;
			case 2:
				iMin = FMath.clamp((int) minGridCoords.x, 0, this.numberColumns - 1);
				iMax = FMath.clamp((int) maxGridCoords.x, 0, this.numberColumns - 1);
				jMin = FMath.clamp((int) minGridCoords.y, 0, this.numberRows - 1);
				jMax = FMath.clamp((int) maxGridCoords.y, 0, this.numberRows - 1);
				break;
		}
		assert (iMin >= 0 && iMin < this.numberColumns);
		assert (iMax >= 0 && iMax < this.numberColumns);
		assert (jMin >= 0 && jMin < this.numberRows);
		assert (jMax >= 0 && jMax < this.numberRows);
		// For each sub-grid points (except the last ones one each dimension)
		for (int i = iMin; i < iMax; i++) {
			for (int j = jMin; j < jMax; j++) {
				// Compute the four point of the current quad
				final Vector3f p1 = getVertexAt(i, j);
				final Vector3f p2 = getVertexAt(i, j + 1);
				final Vector3f p3 = getVertexAt(i + 1, j);
				final Vector3f p4 = getVertexAt(i + 1, j + 1);
				// Generate the first triangle for the current grid rectangle
				final Vector3f[] trianglePoints = { p1, p2, p3 };
				// Test collision against the first triangle
				_callback.testTriangle(trianglePoints);
				// Generate the second triangle for the current grid rectangle
				trianglePoints[0] = p3;
				trianglePoints[1] = p2;
				trianglePoints[2] = p4;
				// Test collision against the second triangle
				_callback.testTriangle(trianglePoints);
			}
		}
	}
	
}

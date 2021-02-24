/*
 * ReactPhysics3D physics library, http://code.google.com/p/reactphysics3d/
 * Copyright (c) 2010-2013 Daniel Chappuis
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 *    that you wrote the original software. If you use this software in a
 *    product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * This file has been modified during the port to Java and differ from the source versions.
 */
package org.atriasoft.ephysics.collision.shapes;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.atriasoft.etk.math.Matrix3f;
import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.RaycastInfo;
import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.collision.TriangleVertexArray;
import org.atriasoft.ephysics.configuration.Defaults;
import org.atriasoft.ephysics.mathematics.Ray;
import org.atriasoft.ephysics.mathematics.SetInteger;

/**
 *  It represents a convex mesh shape. In order to create a convex mesh shape, you
 * need to indicate the local-space position of the mesh vertices. You do it either by
 * passing a vertices array to the ructor or using the addVertex method. Make sure
 * that the set of vertices that you use to create the shape are indeed part of a convex
 * mesh. The center of mass of the shape will be at the origin of the local-space geometry
 * that you use to create the mesh. The method used for collision detection with a convex
 * mesh shape has an O(n) running time with "n" beeing the number of vertices in the mesh.
 * Therefore, you should try not to use too many vertices. However, it is possible to speed
 * up the collision detection by using the edges information of your mesh. The running time
 * of the collision detection that uses the edges is almost O(1) ant time at the cost
 * of additional memory used to store the vertices. You can indicate edges information
 * with the addEdge() method. Then, you must use the setIsEdgesInformationUsed(true) method
 * in order to use the edges information for collision detection.
 */
public class ConvexMeshShape extends ConvexShape {
	protected List<Vector3f> vertices = new ArrayList<>(); //!< Array with the vertices of the mesh
	protected int numberVertices = 0; //!< Number of vertices in the mesh
	protected Vector3f minBounds = new Vector3f(); //!< Mesh minimum bounds in the three local x, y and z directions
	protected Vector3f maxBounds = new Vector3f(); //!< Mesh maximum bounds in the three local x, y and z directions
	protected boolean isEdgesInformationUsed; //!< True if the shape contains the edges of the convex mesh in order to make the collision detection faster
	
	protected Map<Integer, SetInteger> edgesAdjacencyList = new HashMap<>(); //!< Adjacency list representing the edges of the mesh
	
	/**
	 *  Constructor.
	 * If you use this ructor, you will need to set the vertices manually one by one using the addVertex method.
	 */
	public ConvexMeshShape() {
		this(Defaults.OBJECT_MARGIN);
	}
	
	public ConvexMeshShape(final float _margin) {
		super(CollisionShapeType.CONVEX_MESH, _margin);
		this.minBounds = new Vector3f(0, 0, 0);
		this.maxBounds = new Vector3f(0, 0, 0);
		this.numberVertices = 0;
		this.isEdgesInformationUsed = false;
	}
	
	/**
	 *  Constructor to initialize with an array of 3D vertices.
	 * This method creates an longernal copy of the input vertices.
	 * @param _arrayVertices Array with the vertices of the convex mesh
	 * @param _nbVertices Number of vertices in the convex mesh
	 * @param _stride Stride between the beginning of two elements in the vertices array
	 * @param _margin Collision margin (in meters) around the collision shape
	 */
	public ConvexMeshShape(final float[] _arrayVertices, final int _nbVertices, final int _stride) {
		this(_arrayVertices, _nbVertices, _stride, Defaults.OBJECT_MARGIN);
	}
	
	public ConvexMeshShape(final float[] _arrayVertices, final int _nbVertices, final int _stride, final float _margin) {
		super(CollisionShapeType.CONVEX_MESH, _margin);
		this.numberVertices = _nbVertices;
		this.minBounds = new Vector3f(0, 0, 0);
		this.maxBounds = new Vector3f(0, 0, 0);
		this.isEdgesInformationUsed = false;
		int offset = 0;
		// Copy all the vertices longo the longernal array
		for (long iii = 0; iii < this.numberVertices; iii++) {
			this.vertices.add(new Vector3f(_arrayVertices[offset], _arrayVertices[offset + 1], _arrayVertices[offset + 2]));
			offset += _stride;
		}
		// Recalculate the bounds of the mesh
		recalculateBounds();
	}
	
	/**
	 *  Constructor to initialize with a triangle mesh
	 * This method creates an internal copy of the input vertices.
	 * @param _triangleVertexArray Array with the vertices and indices of the vertices and triangles of the mesh
	 * @param _isEdgesInformationUsed True if you want to use edges information for collision detection (faster but requires more memory)
	 * @param _margin Collision margin (in meters) around the collision shape
	 */
	public ConvexMeshShape(final TriangleVertexArray _triangleVertexArray) {
		this(_triangleVertexArray, true);
	}
	
	public ConvexMeshShape(final TriangleVertexArray _triangleVertexArray, final boolean _isEdgesInformationUsed) {
		this(_triangleVertexArray, true, Defaults.OBJECT_MARGIN);
	}
	
	public ConvexMeshShape(final TriangleVertexArray _triangleVertexArray, final boolean _isEdgesInformationUsed, final float _margin) {
		super(CollisionShapeType.CONVEX_MESH, _margin);
		this.minBounds = new Vector3f(0, 0, 0);
		this.maxBounds = new Vector3f(0, 0, 0);
		this.isEdgesInformationUsed = _isEdgesInformationUsed;
		// For each vertex of the mesh
		for (final Vector3f it : _triangleVertexArray.getVertices()) {
			this.vertices.add(it.multiplyNew(this.scaling));
		}
		// If we need to use the edges information of the mesh
		if (this.isEdgesInformationUsed) {
			// For each triangle of the mesh
			for (int iii = 0; iii < _triangleVertexArray.getNbTriangles(); iii++) {
				final int vertexIndex[] = { 0, 0, 0 };
				vertexIndex[0] = _triangleVertexArray.getIndices()[iii * 3];
				vertexIndex[1] = _triangleVertexArray.getIndices()[iii * 3 + 1];
				vertexIndex[2] = _triangleVertexArray.getIndices()[iii * 3 + 2];
				// Add information about the edges
				addEdge(vertexIndex[0], vertexIndex[1]);
				addEdge(vertexIndex[0], vertexIndex[2]);
				addEdge(vertexIndex[1], vertexIndex[2]);
			}
		}
		this.numberVertices = this.vertices.size();
		recalculateBounds();
	}
	
	/**
	 *  Add an edge longo the convex mesh by specifying the two vertex indices of the edge.
	 * @note that the vertex indices start at zero and need to correspond to the order of
	 * the vertices in the vertices array in the ructor or the order of the calls
	 * of the addVertex methods that you use to add vertices longo the convex mesh.
	 * @param _v1 Index of the first vertex of the edge to add
	 * @param _v2 Index of the second vertex of the edge to add
	 */
	public void addEdge(final int _v1, final int _v2) {
		// If the entry for vertex v1 does not exist in the adjacency list
		if (!this.edgesAdjacencyList.containsKey(_v1)) {
			this.edgesAdjacencyList.put(_v1, new SetInteger());
		}
		// If the entry for vertex v2 does not exist in the adjacency list
		if (!this.edgesAdjacencyList.containsKey(_v2)) {
			this.edgesAdjacencyList.put(_v2, new SetInteger());
		}
		// Add the edge in the adjacency list
		this.edgesAdjacencyList.get(_v1).add(_v2);
		this.edgesAdjacencyList.get(_v2).add(_v1);
	}
	
	/**
	 *  Add a vertex longo the convex mesh
	 * @param vertex Vertex to be added
	 */
	public void addVertex(final Vector3f _vertex) {
		// Add the vertex in to vertices array
		this.vertices.add(_vertex);
		this.numberVertices++;
		// Update the bounds of the mesh
		if (_vertex.x * this.scaling.x > this.maxBounds.x) {
			this.maxBounds.setX(_vertex.x * this.scaling.x);
		}
		if (_vertex.x * this.scaling.x < this.minBounds.x) {
			this.minBounds.setX(_vertex.x * this.scaling.x);
		}
		if (_vertex.y * this.scaling.y > this.maxBounds.y) {
			this.maxBounds.setY(_vertex.y * this.scaling.y);
		}
		if (_vertex.y * this.scaling.y < this.minBounds.y) {
			this.minBounds.setY(_vertex.y * this.scaling.y);
		}
		if (_vertex.z * this.scaling.z > this.maxBounds.z) {
			this.maxBounds.setZ(_vertex.z * this.scaling.z);
		}
		if (_vertex.z * this.scaling.z < this.minBounds.z) {
			this.minBounds.setZ(_vertex.z * this.scaling.z);
		}
	}
	
	@Override
	public void computeLocalInertiaTensor(final Matrix3f _tensor, final float _mass) {
		final float factor = (1.0f / 3.0f) * _mass;
		final Vector3f realExtent = this.maxBounds.lessNew(this.minBounds).multiply(0.5f);
		assert (realExtent.x > 0 && realExtent.y > 0 && realExtent.z > 0);
		final float xSquare = realExtent.x * realExtent.x;
		final float ySquare = realExtent.y * realExtent.y;
		final float zSquare = realExtent.z * realExtent.z;
		_tensor.set(factor * (ySquare + zSquare), 0.0f, 0.0f, 0.0f, factor * (xSquare + zSquare), 0.0f, 0.0f, 0.0f, factor * (xSquare + ySquare));
	}
	
	@Override
	public void getLocalBounds(final Vector3f _min, final Vector3f _max) {
		_min.set(this.minBounds);
		_max.set(this.maxBounds);
	}
	
	@Override
	public Vector3f getLocalSupportPointWithoutMargin(final Vector3f _direction, final CacheData _cachedCollisionData) {
		assert (this.numberVertices == this.vertices.size());
		assert (_cachedCollisionData != null);
		// Allocate memory for the cached collision data if not allocated yet
		if (_cachedCollisionData.data == null) {
			// TODO the data is nort set outside ==> find how ...  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			_cachedCollisionData.data = 0;
		}
		// If the edges information is used to speed up the collision detection
		if (this.isEdgesInformationUsed) {
			assert (this.edgesAdjacencyList.size() == this.numberVertices);
			int maxVertex = (Integer) (_cachedCollisionData.data);
			float maxDotProduct = _direction.dot(this.vertices.get(maxVertex));
			boolean isOptimal;
			// Perform hill-climbing (local search)
			do {
				isOptimal = true;
				assert (this.edgesAdjacencyList.get(maxVertex).size() > 0);
				// For all neighbors of the current vertex
				for (final Integer it : this.edgesAdjacencyList.get(maxVertex).getRaw()) {
					// Compute the dot product
					final float dotProduct = _direction.dot(this.vertices.get(it));
					// If the current vertex is a better vertex (larger dot product)
					if (dotProduct > maxDotProduct) {
						maxVertex = it;
						maxDotProduct = dotProduct;
						isOptimal = false;
					}
				}
			} while (!isOptimal);
			// Cache the support vertex
			_cachedCollisionData.data = maxVertex;
			// Return the support vertex
			return this.vertices.get(maxVertex).multiplyNew(this.scaling);
		} else {
			// If the edges information is not used
			double maxDotProduct = Float.MIN_VALUE;
			int indexMaxDotProduct = 0;
			// For each vertex of the mesh
			for (int i = 0; i < this.numberVertices; i++) {
				// Compute the dot product of the current vertex
				final double dotProduct = _direction.dot(this.vertices.get(i));
				// If the current dot product is larger than the maximum one
				if (dotProduct > maxDotProduct) {
					indexMaxDotProduct = i;
					maxDotProduct = dotProduct;
				}
			}
			assert (maxDotProduct >= 0.0f);
			// Return the vertex with the largest dot product in the support direction
			return this.vertices.get(indexMaxDotProduct).multiplyNew(this.scaling);
		}
	}
	
	/**
	 *  Return true if the edges information is used to speed up the collision detection
	 * @return True if the edges information is used and false otherwise
	 */
	public boolean isEdgesInformationUsed() {
		return this.isEdgesInformationUsed;
	}
	
	@Override
	public boolean raycast(final Ray _ray, final RaycastInfo _raycastInfo, final ProxyShape _proxyShape) {
		return _proxyShape.getBody().getWorld().getCollisionDetection().getNarrowPhaseGJKAlgorithm().raycast(_ray, _proxyShape, _raycastInfo);
	}
	
	/// Recompute the bounds of the mesh
	protected void recalculateBounds() {
		// TODO : Only works if the local origin is inside the mesh
		//		=> Make it more robust (init with first vertex of mesh instead)
		this.minBounds.setZero();
		this.maxBounds.setZero();
		// For each vertex of the mesh
		for (int i = 0; i < this.numberVertices; i++) {
			if (this.vertices.get(i).x > this.maxBounds.x) {
				this.maxBounds.setX(this.vertices.get(i).x);
			}
			if (this.vertices.get(i).x < this.minBounds.x) {
				this.minBounds.setX(this.vertices.get(i).x);
			}
			if (this.vertices.get(i).y > this.maxBounds.y) {
				this.maxBounds.setY(this.vertices.get(i).y);
			}
			if (this.vertices.get(i).y < this.minBounds.y) {
				this.minBounds.setY(this.vertices.get(i).y);
			}
			if (this.vertices.get(i).z > this.maxBounds.z) {
				this.maxBounds.setZ(this.vertices.get(i).z);
			}
			if (this.vertices.get(i).z < this.minBounds.z) {
				this.minBounds.setZ(this.vertices.get(i).z);
			}
		}
		// Apply the local scaling factor
		this.maxBounds.multiply(this.scaling);
		this.minBounds.multiply(this.scaling);
		// Add the object margin to the bounds
		this.maxBounds.add(this.margin);
		this.minBounds.less(this.margin);
	}
	
	/**
	 *  Set the variable to know if the edges information is used to speed up the
	 * collision detection
	 * @param isEdgesUsed True if you want to use the edges information to speed up the collision detection with the convex mesh shape
	 */
	public void setIsEdgesInformationUsed(final boolean _isEdgesUsed) {
		this.isEdgesInformationUsed = _isEdgesUsed;
	}
	
	@Override
	public void setLocalScaling(final Vector3f _scaling) {
		super.setLocalScaling(_scaling);
		recalculateBounds();
	}
	
	@Override
	public boolean testPointInside(final Vector3f _localPoint, final ProxyShape _proxyShape) {
		// Use the GJK algorithm to test if the point is inside the convex mesh
		return _proxyShape.getBody().getWorld().getCollisionDetection().getNarrowPhaseGJKAlgorithm().testPointInside(_localPoint, _proxyShape);
	}
};

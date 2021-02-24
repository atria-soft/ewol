package org.atriasoft.ephysics.collision.narrowphase.EPA;

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
public class EdgeEPA implements Cloneable {
	/// Maximum number of support points of the polytope
	static public final int MAX_SUPPORT_POINTS = 100;
	/// Maximum number of facets of the polytope
	static public final int MAX_FACETS = 200;
	
	// Return the index of the next counter-clockwise edge of the ownver triangle
	static public int indexOfNextCounterClockwiseEdge(final int iii) {
		return (iii + 1) % 3;
	}
	
	// Return the index of the previous counter-clockwise edge of the ownver triangle
	static public int indexOfPreviousCounterClockwiseEdge(final int iii) {
		return (iii + 2) % 3;
	}
	
	/// Pointer to the triangle that contains this edge
	private TriangleEPA ownerTriangle;
	/// Index of the edge in the triangle (between 0 and 2).
	/// The edge with index i connect triangle vertices i and (i+1 % 3)
	private int index;
	
	/// Constructor
	public EdgeEPA() {
		this.ownerTriangle = null;
	};
	
	/// Copy-ructor
	public EdgeEPA(final EdgeEPA obj) {
		this.ownerTriangle = obj.ownerTriangle;
		this.index = obj.index;
	}
	
	/// Constructor
	public EdgeEPA(final TriangleEPA ownerTriangle, final int index) {
		this.ownerTriangle = ownerTriangle;
		this.index = index;
		assert (index >= 0 && index < 3);
	}
	
	@Override
	protected EdgeEPA clone() throws CloneNotSupportedException {
		return new EdgeEPA(this);
	}
	
	/// Execute the recursive silhouette algorithm from this edge
	public boolean computeSilhouette(final Vector3f[] vertices, final int indexNewVertex, final TrianglesStore triangleStore) {
		//Log.error("EdgeEPA computeSilhouette ...");
		// If the edge has not already been visited
		if (!this.ownerTriangle.getIsObsolete()) {
			// If the triangle of this edge is not visible from the given point
			if (!this.ownerTriangle.isVisibleFromVertex(vertices, indexNewVertex)) {
				//Log.error("EdgeEPA 1 befor call: " + indexNewVertex + ", " + getTargetVertexIndex() + ", " + getSourceVertexIndex());
				final TriangleEPA triangle = triangleStore.newTriangle(vertices, indexNewVertex, getTargetVertexIndex(), getSourceVertexIndex());
				// If the triangle has been created
				if (triangle != null) {
					TriangleEPA.halfLink(new EdgeEPA(triangle, 1), this);
					return true;
				}
				return false;
			} else {
				// The current triangle is visible and therefore obsolete
				this.ownerTriangle.setIsObsolete(true);
				final int backup = triangleStore.getNbTriangles();
				if (!this.ownerTriangle.getAdjacentEdge(indexOfNextCounterClockwiseEdge(this.index)).computeSilhouette(vertices, indexNewVertex, triangleStore)) {
					this.ownerTriangle.setIsObsolete(false);
					//Log.error("EdgeEPA 2 befor call: " + indexNewVertex + ", " + getTargetVertexIndex() + ", " + getSourceVertexIndex());
					final TriangleEPA triangle = triangleStore.newTriangle(vertices, indexNewVertex, getTargetVertexIndex(), getSourceVertexIndex());
					// If the triangle has been created
					if (triangle != null) {
						TriangleEPA.halfLink(new EdgeEPA(triangle, 1), this);
						return true;
					}
					return false;
				} else if (!this.ownerTriangle.getAdjacentEdge(indexOfPreviousCounterClockwiseEdge(this.index)).computeSilhouette(vertices, indexNewVertex, triangleStore)) {
					this.ownerTriangle.setIsObsolete(false);
					triangleStore.resize(backup);
					//Log.error("EdgeEPA 3 befor call: " + indexNewVertex + ", " + getTargetVertexIndex() + ", " + getSourceVertexIndex());
					final TriangleEPA triangle = triangleStore.newTriangle(vertices, indexNewVertex, getTargetVertexIndex(), getSourceVertexIndex());
					if (triangle != null) {
						TriangleEPA.halfLink(new EdgeEPA(triangle, 1), this);
						return true;
					}
					return false;
				}
			}
		}
		return true;
	}
	
	/// Return the index of the edge in the triangle
	public int getIndex() {
		return this.index;
	}
	
	/// Return the pointer to the owner triangle
	public TriangleEPA getOwnerTriangle() {
		return this.ownerTriangle;
	}
	
	/// Return index of the source vertex of the edge
	public int getSourceVertexIndex() {
		//return this.ownerTriangle[this.index];
		return this.ownerTriangle.get(this.index);
	}
	
	/// Return the index of the target vertex of the edge
	public int getTargetVertexIndex() {
		return this.ownerTriangle.get(indexOfNextCounterClockwiseEdge(this.index));
		//return this.ownerTriangle[indexOfNextCounterClockwiseEdge(this.index)];
	}
	
	/// Assignment
	public EdgeEPA set(final EdgeEPA obj) {
		this.ownerTriangle = obj.ownerTriangle;
		this.index = obj.index;
		return this;
	}
	
}

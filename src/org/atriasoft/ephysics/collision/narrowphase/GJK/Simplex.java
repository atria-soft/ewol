package org.atriasoft.ephysics.collision.narrowphase.GJK;

import org.atriasoft.etk.math.Vector3f;

/**
 * This class represents a simplex which is a set of 3D points. This
 * class is used in the GJK algorithm. This implementation is based on
 * the implementation discussed in the book "Collision Detection in 3D
 * Environments". This class implements the Johnson's algorithm for
 * computing the point of a simplex that is closest to the origin and also
 * the smallest simplex needed to represent that closest point.
 */
public class Simplex {
	
	private class Array2f {
		private final float[] data;
		private final int sizeX;
		private final int sizeY;
		
		public Array2f(final int sizeX, final int sizeY) {
			this.sizeX = sizeX;
			this.sizeY = sizeY;
			this.data = new float[sizeX * sizeY];
		}
		
		public float get(final int xxx, final int yyy) {
			return this.data[yyy * this.sizeX + xxx];
		}
		
		public int getSizeX() {
			return this.sizeX;
		}
		
		public int getSizeY() {
			return this.sizeY;
		}
		
		public void set(final int xxx, final int yyy, final float data) {
			this.data[yyy * this.sizeX + xxx] = data;
		}
	}
	
	private class Array2Vector3f {
		private final Vector3f[] data;
		private final int sizeX;
		private final int sizeY;
		
		public Array2Vector3f(final int sizeX, final int sizeY) {
			this.sizeX = sizeX;
			this.sizeY = sizeY;
			this.data = new Vector3f[sizeX * sizeY];
		}
		
		public Vector3f get(final int xxx, final int yyy) {
			return this.data[yyy * this.sizeX + xxx];
		}
		
		public int getSizeX() {
			return this.sizeX;
		}
		
		public int getSizeY() {
			return this.sizeY;
		}
		
		public void set(final int xxx, final int yyy, final Vector3f data) {
			this.data[yyy * this.sizeX + xxx] = data;
		}
	}
	
	/// Current points
	private final Vector3f[] points = new Vector3f[4];
	
	/// pointsLengthSquare[i] = (points[i].length)^2
	private final float[] pointsLengthSquare = new float[4];
	
	/// Maximum length of pointsLengthSquare[i]
	private float maxLengthSquare;
	
	/// Support points of object A in local coordinates
	private final Vector3f[] suppPointsA = new Vector3f[4];
	
	/// Support points of object B in local coordinates
	private final Vector3f[] suppPointsB = new Vector3f[4];
	
	/// diff[i][j] contains points[i] - points[j]
	private final Array2Vector3f diffLength = new Array2Vector3f(4, 4);
	
	/// Cached determinant values
	private final Array2f det = new Array2f(16, 4);
	
	/// norm[i][j] = (diff[i][j].length())^2
	private final Array2f normalSquare = new Array2f(4, 4);
	
	/// 4 bits that identify the current points of the simplex
	/// For instance, 0101 means that points[1] and points[3] are in the simplex
	private int bitsCurrentSimplex = 0;
	
	/// Number between 1 and 4 that identify the last found support point
	private int lastFound = 0;
	
	/// Position of the last found support point (lastFoundBit = 0x1 << lastFound)
	private int lastFoundBit = 0;
	
	/// allint = bitsCurrentSimplex | lastFoundBit;
	private int allBits = 0;
	
	// -------------------- Methods -------------------- //
	
	/// Constructor
	public Simplex() {
		
	}
	
	/// Add a new support point of (A-B) into the simplex.
	/// suppPointA : support point of object A in a direction -v
	/// suppPointB : support point of object B in a direction v
	/// point	  : support point of object (A-B) => point = suppPointA - suppPointB
	public void addPoint(final Vector3f point, final Vector3f suppPointA, final Vector3f suppPointB) {
		assert (!isFull());
		//Log.warning("simplex: addPoint(" + point + ", " + suppPointA + ", " + suppPointA + ")");
		this.lastFound = 0;
		this.lastFoundBit = 0x1;
		
		// Look for the bit corresponding to one of the four point that is not in
		// the current simplex
		while (overlap(this.bitsCurrentSimplex, this.lastFoundBit)) {
			this.lastFound++;
			this.lastFoundBit <<= 1;
		}
		//Log.warning("     this.lastFound " + this.lastFound);
		//Log.warning("     this.lastFoundBit " + this.lastFoundBit);
		
		assert (this.lastFound < 4);
		
		// Add the point into the simplex
		this.points[this.lastFound] = point;
		this.pointsLengthSquare[this.lastFound] = point.dot(point);
		this.allBits = this.bitsCurrentSimplex | this.lastFoundBit;
		//Log.warning("     this.allBits " + this.allBits);
		
		// Update the cached values
		updateCache();
		
		// Compute the cached determinant values
		computeDeterminants();
		
		// Add the support points of objects A and B
		this.suppPointsA[this.lastFound] = suppPointA;
		this.suppPointsB[this.lastFound] = suppPointB;
	}
	
	/// Backup the closest point
	public void backupClosestPointInSimplex(final Vector3f point) {
		float minDistSquare = Float.MAX_VALUE;
		for (int bit = this.allBits; bit != 0x0; bit--) {
			if (isSubset(bit, this.allBits) && isProperSubset(bit)) {
				final Vector3f u = computeClosestPointForSubset(bit);
				final float distSquare = u.dot(u);
				if (distSquare < minDistSquare) {
					minDistSquare = distSquare;
					this.bitsCurrentSimplex = bit;
					point.set(u);
				}
			}
		}
	}
	
	/// Compute the closest point to the origin of the current simplex.
	/// This method executes the Jonhnson's algorithm for computing the point
	/// "v" of simplex that is closest to the origin. The method returns true
	/// if a closest point has been found.
	public boolean computeClosestPoint(final Vector3f vvv) {
		// For each possible simplex set
		for (int subset = this.bitsCurrentSimplex; subset != 0x0; subset--) {
			// If the simplex is a subset of the current simplex and is valid for the Johnson's
			// algorithm test
			if (isSubset(subset, this.bitsCurrentSimplex) && isValidSubset(subset | this.lastFoundBit)) {
				this.bitsCurrentSimplex = subset | this.lastFoundBit; // Add the last added point to the current simplex
				vvv.set(computeClosestPointForSubset(this.bitsCurrentSimplex)); // Compute the closest point in the simplex
				return true;
			}
		}
		
		// If the simplex that contains only the last added point is valid for the Johnson's algorithm test
		if (isValidSubset(this.lastFoundBit)) {
			this.bitsCurrentSimplex = this.lastFoundBit; // Set the current simplex to the set that contains only the last added point
			this.maxLengthSquare = this.pointsLengthSquare[this.lastFound]; // Update the maximum square length
			vvv.set(this.points[this.lastFound]); // The closest point of the simplex "v" is the last added point
			return true;
		}
		
		// The algorithm failed to found a point
		return false;
	}
	
	/// Return the closest point "v" in the convex hull of a subset of points
	private Vector3f computeClosestPointForSubset(final int subset) {
		final Vector3f vvv = new Vector3f(0.0f, 0.0f, 0.0f); // Closet point v = sum(lambda_i * points[i])
		this.maxLengthSquare = 0.0f;
		float deltaX = 0.0f; // deltaX = sum of all det[subset][i]
		// For each four point in the possible simplex set
		for (int iii = 0, bit = 0x1; iii < 4; iii++, bit <<= 1) {
			// If the current point is in the subset
			if (overlap(subset, bit)) {
				// deltaX = sum of all det[subset][i]
				deltaX += this.det.get(subset, iii);
				if (this.maxLengthSquare < this.pointsLengthSquare[iii]) {
					this.maxLengthSquare = this.pointsLengthSquare[iii];
				}
				// Closest point v = sum(lambda_i * points[i])
				vvv.add(this.points[iii].multiplyNew(this.det.get(subset, iii)));
			}
		}
		assert (deltaX > 0.0f);
		// Return the closet point "v" in the convex hull for the given subset
		return vvv.multiply(1.0f / deltaX);
	}
	
	/// Compute the closest points "pA" and "pB" of object A and B.
	/// The points are computed as follows :
	///	  pA = sum(lambda_i * a_i)	where "a_i" are the support points of object A
	///	  pB = sum(lambda_i * b_i)	where "b_i" are the support points of object B
	///	  with lambda_i = deltaX_i / deltaX
	public void computeClosestPointsOfAandB(final Vector3f pA, final Vector3f pB) {
		float deltaX = 0.0f;
		pA.set(0.0f, 0.0f, 0.0f);
		pB.set(0.0f, 0.0f, 0.0f);
		// For each four points in the possible simplex set
		for (int iii = 0, bit = 0x1; iii < 4; iii++, bit <<= 1) {
			// If the current point is part of the simplex
			if (overlap(this.bitsCurrentSimplex, bit)) {
				deltaX += this.det.get(this.bitsCurrentSimplex, iii);
				pA.add(this.suppPointsA[iii].multiplyNew(this.det.get(this.bitsCurrentSimplex, iii)));
				pB.add(this.suppPointsB[iii].multiplyNew(this.det.get(this.bitsCurrentSimplex, iii)));
			}
		}
		
		assert (deltaX > 0.0f);
		final float factor = 1.0f / deltaX;
		pA.multiply(factor);
		pB.multiply(factor);
	}
	
	/// Compute the cached determinant values
	private void computeDeterminants() {
		this.det.set(this.lastFoundBit, this.lastFound, 1.0f);
		//Log.warning("simplex:     computeDeterminants() " + this.det.get(this.lastFoundBit, this.lastFound));
		// If the current simplex is not empty
		if (!isEmpty()) {
			// For each possible four points in the simplex set
			for (int iii = 0, bitI = 0x1; iii < 4; iii++, bitI <<= 1) {
				// If the current point is in the simplex
				if (overlap(this.bitsCurrentSimplex, bitI)) {
					final int bit2 = bitI | this.lastFoundBit;
					float tmpp = this.diffLength.get(this.lastFound, iii).dot(this.points[this.lastFound]);
					this.det.set(bit2, iii, tmpp);
					tmpp = this.diffLength.get(iii, this.lastFound).dot(this.points[iii]);
					this.det.set(bit2, this.lastFound, tmpp);
					for (int jjj = 0, bitJ = 0x1; jjj < iii; jjj++, bitJ <<= 1) {
						if (overlap(this.bitsCurrentSimplex, bitJ)) {
							final int bit3 = bitJ | bit2;
							int kkk = this.normalSquare.get(iii, jjj) < this.normalSquare.get(this.lastFound, jjj) ? iii : this.lastFound;
							float tmp2 = this.det.get(bit2, iii) * this.diffLength.get(kkk, jjj).dot(this.points[iii])
									+ this.det.get(bit2, this.lastFound) * this.diffLength.get(kkk, jjj).dot(this.points[this.lastFound]);
							this.det.set(bit3, jjj, tmp2);
							kkk = this.normalSquare.get(jjj, iii) < this.normalSquare.get(this.lastFound, iii) ? jjj : this.lastFound;
							tmp2 = this.det.get(bitJ | this.lastFoundBit, jjj) * this.diffLength.get(kkk, iii).dot(this.points[jjj])
									+ this.det.get(bitJ | this.lastFoundBit, this.lastFound) * this.diffLength.get(kkk, iii).dot(this.points[this.lastFound]);
							this.det.set(bit3, iii, tmp2);
							kkk = this.normalSquare.get(iii, this.lastFound) < this.normalSquare.get(jjj, this.lastFound) ? iii : jjj;
							tmp2 = this.det.get(bitJ | bitI, jjj) * this.diffLength.get(kkk, this.lastFound).dot(this.points[jjj])
									+ this.det.get(bitJ | bitI, iii) * this.diffLength.get(kkk, this.lastFound).dot(this.points[iii]);
							this.det.set(bit3, this.lastFound, tmp2);
						}
					}
				}
			}
			
			if (this.allBits == 0xf) {
				int kkk;
				
				kkk = this.normalSquare.get(1, 0) < this.normalSquare.get(2, 0) ? (this.normalSquare.get(1, 0) < this.normalSquare.get(3, 0) ? 1 : 3)
						: (this.normalSquare.get(2, 0) < this.normalSquare.get(3, 0) ? 2 : 3);
				float tmp3 = this.det.get(0xe, 1) * this.diffLength.get(kkk, 0).dot(this.points[1]) + this.det.get(0xe, 2) * this.diffLength.get(kkk, 0).dot(this.points[2])
						+ this.det.get(0xe, 3) * this.diffLength.get(kkk, 0).dot(this.points[3]);
				this.det.set(0xf, 0, tmp3);
				
				kkk = this.normalSquare.get(0, 1) < this.normalSquare.get(2, 1) ? (this.normalSquare.get(0, 1) < this.normalSquare.get(3, 1) ? 0 : 3)
						: (this.normalSquare.get(2, 1) < this.normalSquare.get(3, 1) ? 2 : 3);
				tmp3 = this.det.get(0xd, 0) * this.diffLength.get(kkk, 1).dot(this.points[0]) + this.det.get(0xd, 2) * this.diffLength.get(kkk, 1).dot(this.points[2])
						+ this.det.get(0xd, 3) * this.diffLength.get(kkk, 1).dot(this.points[3]);
				this.det.set(0xf, 1, tmp3);
				
				kkk = this.normalSquare.get(0, 2) < this.normalSquare.get(1, 2) ? (this.normalSquare.get(0, 2) < this.normalSquare.get(3, 2) ? 0 : 3)
						: (this.normalSquare.get(1, 2) < this.normalSquare.get(3, 2) ? 1 : 3);
				tmp3 = this.det.get(0xb, 0) * this.diffLength.get(kkk, 2).dot(this.points[0]) + this.det.get(0xb, 1) * this.diffLength.get(kkk, 2).dot(this.points[1])
						+ this.det.get(0xb, 3) * this.diffLength.get(kkk, 2).dot(this.points[3]);
				this.det.set(0xf, 2, tmp3);
				
				kkk = this.normalSquare.get(0, 3) < this.normalSquare.get(1, 3) ? (this.normalSquare.get(0, 3) < this.normalSquare.get(2, 3) ? 0 : 2)
						: (this.normalSquare.get(1, 3) < this.normalSquare.get(2, 3) ? 1 : 2);
				tmp3 = this.det.get(0x7, 0) * this.diffLength.get(kkk, 3).dot(this.points[0]) + this.det.get(0x7, 1) * this.diffLength.get(kkk, 3).dot(this.points[1])
						+ this.det.get(0x7, 2) * this.diffLength.get(kkk, 3).dot(this.points[2]);
				this.det.set(0xf, 3, tmp3);
			}
		}
	}
	
	/// Return the maximum squared length of a point
	public float getMaxLengthSquareOfAPoint() {
		return this.maxLengthSquare;
	}
	
	/// Return the points of the simplex
	public int getSimplex(final Vector3f[] suppPointsA, final Vector3f[] suppPointsB, final Vector3f[] points) {
		int nbVertices = 0;
		// For each four point in the possible simplex
		for (int iii = 0, bit = 0x1; iii < 4; iii++, bit <<= 1) {
			// If the current point is in the simplex
			if (overlap(this.bitsCurrentSimplex, bit)) {
				// Store the points
				suppPointsA[nbVertices] = this.suppPointsA[nbVertices].clone();
				suppPointsB[nbVertices] = this.suppPointsB[nbVertices].clone();
				points[nbVertices] = this.points[nbVertices].clone();
				nbVertices++;
			}
		}
		// Return the number of points in the simplex
		return nbVertices;
	}
	
	/// Return true if the set is affinely dependent
	/// A set if affinely dependent if a point of the set
	/// is an affine combination of other points in the set
	public boolean isAffinelyDependent() {
		float sum = 0.0f;
		// For each four point of the possible simplex set
		for (int iii = 0, bit = 0x1; iii < 4; iii++, bit <<= 1) {
			if (overlap(this.allBits, bit)) {
				sum += this.det.get(this.allBits, iii);
			}
		}
		return (sum <= 0.0f);
	}
	
	/// Return true if the simplex is empty
	public boolean isEmpty() {
		return (this.bitsCurrentSimplex == 0x0);
	}
	
	/// Return true if the simplex contains 4 points
	public boolean isFull() {
		return (this.bitsCurrentSimplex == 0xf);
	}
	
	/// Return true if the point is in the simplex
	public boolean isPointInSimplex(final Vector3f point) {
		// For each four possible points in the simplex
		for (int iii = 0, bit = 0x1; iii < 4; iii++, bit <<= 1) {
			// Check if the current point is in the simplex
			if (overlap(this.allBits, bit) && point == this.points[iii]) {
				return true;
			}
		}
		return false;
	}
	
	/// Return true if the subset is a proper subset.
	/// A proper subset X is a subset where for all point "y_i" in X, we have
	/// detX_i value bigger than zero
	private boolean isProperSubset(final int subset) {
		// For each four point of the possible simplex set
		for (int iii = 0, bit = 0x1; iii < 4; iii++, bit <<= 1) {
			if (overlap(subset, bit) && this.det.get(subset, iii) <= 0.0f) {
				return false;
			}
		}
		return true;
	}
	
	/// Return true if the bits of "b" is a subset of the bits of "a"
	private boolean isSubset(final int a, final int b) {
		return ((a & b) == a);
	}
	
	/// Return true if the subset is a valid one for the closest point computation.
	/// A subset X is valid if :
	///	1. delta(X)_i > 0 for each "i" in I_x and
	///	2. delta(X U {y_j})_j <= 0 for each "j" not in I_x_
	private boolean isValidSubset(final int subset) {
		// For each four point in the possible simplex set
		for (int iii = 0, bit = 0x1; iii < 4; iii++, bit <<= 1) {
			if (overlap(this.allBits, bit)) {
				// If the current point is in the subset
				if (overlap(subset, bit)) {
					// If one delta(X)_i is smaller or equal to zero
					if (this.det.get(subset, iii) <= 0.0f) {
						// The subset is not valid
						return false;
					}
				}
				// If the point is not in the subset and the value delta(X U {y_j})_j
				// is bigger than zero
				else if (this.det.get(subset | bit, iii) > 0.0f) {
					// The subset is not valid
					return false;
				}
			}
		}
		return true;
	}
	
	/// Return true if some bits of "a" overlap with bits of "b"
	private boolean overlap(final int a, final int b) {
		return ((a & b) != 0x0);
	}
	
	/// Update the cached values used during the GJK algorithm
	private void updateCache() {
		//Log.warning("simplex:     update Cache()");
		// For each of the four possible points of the simplex
		for (int iii = 0, bit = 0x1; iii < 4; iii++, bit <<= 1) {
			//Log.warning("simplex:         iii=" + iii);
			//Log.warning("simplex:         bit=" + bit);
			
			// If the current points is in the simplex
			if (overlap(this.bitsCurrentSimplex, bit)) {
				//Log.warning("simplex:         ==> overlap");
				// Compute the distance between two points in the possible simplex set
				final Vector3f tmp = this.points[iii].lessNew(this.points[this.lastFound]);
				this.diffLength.set(iii, this.lastFound, tmp);
				final Vector3f tmp2 = tmp.multiplyNew(-1);
				this.diffLength.set(this.lastFound, iii, tmp2);
				// Compute the squared length of the vector
				// distances from points in the possible simplex set
				final float normal = tmp.dot(tmp);
				this.normalSquare.set(iii, this.lastFound, normal);
				this.normalSquare.set(this.lastFound, iii, normal);
			}
		}
	}
	
}

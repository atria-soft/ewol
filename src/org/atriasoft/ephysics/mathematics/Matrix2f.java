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
package org.atriasoft.ephysics.mathematics;

import org.atriasoft.etk.math.Constant;
import org.atriasoft.etk.math.Vector2f;

/**
 * This class represents a 2x2 matrix.
 *
 * @author Jason Sorensen <sorensenj@smert.net>
 */
public class Matrix2f {
	
	// Rows of the matrix (m[row][column])
	float m00;
	float m01;
	float m10;
	float m11;
	
	// Constructor
	public Matrix2f() {
		setZero();
	}
	
	// Constructor
	public Matrix2f(final float value) {
		set(value, value, value, value);
	}
	
	// Constructor with arguments
	public Matrix2f(final float a1, final float a2, final float b1, final float b2) {
		set(a1, a2, b1, b2);
	}
	
	// Copy-constructor
	public Matrix2f(final Matrix2f matrix) {
		set(matrix);
	}
	
	// Return the matrix with absolute values
	public Matrix2f abs() {
		this.m00 = Math.abs(this.m00);
		this.m01 = Math.abs(this.m01);
		this.m10 = Math.abs(this.m10);
		this.m11 = Math.abs(this.m11);
		return this;
	}
	
	// Overloaded operator for addition with assignment
	public Matrix2f add(final Matrix2f matrix) {
		this.m00 += matrix.m00;
		this.m01 += matrix.m01;
		this.m10 += matrix.m10;
		this.m11 += matrix.m11;
		return this;
	}
	
	@Override
	public boolean equals(final Object obj) {
		
		if (obj == null) {
			return false;
		}
		if (getClass() != obj.getClass()) {
			return false;
		}
		final Matrix2f other = (Matrix2f) obj;
		if (Float.floatToIntBits(this.m00) != Float.floatToIntBits(other.m00)) {
			return false;
		}
		if (Float.floatToIntBits(this.m10) != Float.floatToIntBits(other.m10)) {
			return false;
		}
		if (Float.floatToIntBits(this.m01) != Float.floatToIntBits(other.m01)) {
			return false;
		}
		return Float.floatToIntBits(this.m11) == Float.floatToIntBits(other.m11);
	}
	
	// Return a column
	public Vector2f getColumn(final int index) {
		if (index == 0) {
			return new Vector2f(this.m00, this.m10);
		} else if (index == 1) {
			return new Vector2f(this.m01, this.m11);
		}
		throw new IllegalArgumentException("Unknown column index: " + index);
	}
	
	// Return the determinant of the matrix
	public float getDeterminant() {
		return this.m00 * this.m11 - this.m10 * this.m01;
	}
	
	// Return a row
	public Vector2f getRow(final int index) {
		if (index == 0) {
			return new Vector2f(this.m00, this.m01);
		} else if (index == 1) {
			return new Vector2f(this.m10, this.m11);
		}
		throw new IllegalArgumentException("Unknown row index: " + index);
	}
	
	// Return the trace of the matrix
	public float getTrace() {
		return this.m00 + this.m11;
	}
	
	@Override
	public int hashCode() {
		int hash = 5;
		hash = 23 * hash + Float.floatToIntBits(this.m00);
		hash = 23 * hash + Float.floatToIntBits(this.m10);
		hash = 23 * hash + Float.floatToIntBits(this.m01);
		hash = 23 * hash + Float.floatToIntBits(this.m11);
		return hash;
	}
	
	// Set the matrix to the identity matrix
	public Matrix2f identity() {
		this.m00 = 1.0f;
		this.m01 = 0.0f;
		this.m10 = 0.0f;
		this.m11 = 1.0f;
		return this;
	}
	
	// Return the inverse matrix
	public Matrix2f inverse() {
		
		// Compute the determinant of the matrix
		final float determinant = getDeterminant();
		
		// Check if the determinant is equal to zero
		assert (Math.abs(determinant) > Constant.FLOAT_EPSILON);
		final float invDeterminant = 1.0f / determinant;
		
		set(this.m11, -this.m01, -this.m10, this.m00);
		
		// Return the inverse matrix
		return multiply(invDeterminant);
	}
	
	public Matrix2f inverseNew() {
		final Matrix2f tmp = new Matrix2f(this);
		tmp.inverse();
		return tmp;
	}
	
	// Overloaded operator for the negative of the matrix
	public Matrix2f invert() {
		this.m00 = -this.m00;
		this.m01 = -this.m01;
		this.m10 = -this.m10;
		this.m11 = -this.m11;
		return this;
	}
	
	// Overloaded operator for substraction with assignment
	public Matrix2f less(final Matrix2f matrix) {
		this.m00 -= matrix.m00;
		this.m01 -= matrix.m01;
		this.m10 -= matrix.m10;
		this.m11 -= matrix.m11;
		return this;
	}
	
	// Overloaded operator for multiplication with a number with assignment
	public Matrix2f multiply(final float number) {
		this.m00 *= number;
		this.m01 *= number;
		this.m10 *= number;
		this.m11 *= number;
		return this;
	}
	
	// Overloaded operator for matrix multiplication
	public Matrix2f multiply(final Matrix2f matrix) {
		set(this.m00 * matrix.m00 + this.m01 * matrix.m10, this.m00 * matrix.m01 + this.m01 * matrix.m11, this.m10 * matrix.m00 + this.m11 * matrix.m10, this.m10 * matrix.m01 + this.m11 * matrix.m11);
		return this;
	}
	
	// Overloaded operator for multiplication with a vector
	public Vector2f multiplyNew(final Vector2f vector) {
		return new Vector2f(this.m00 * vector.x + this.m01 * vector.y, this.m10 * vector.x + this.m11 * vector.y);
	}
	
	// Method to set all the values in the matrix
	public final Matrix2f set(final float a1, final float a2, final float b1, final float b2) {
		this.m00 = a1;
		this.m01 = a2;
		this.m10 = b1;
		this.m11 = b2;
		return this;
	}
	
	public final Matrix2f set(final Matrix2f matrix) {
		this.m00 = matrix.m00;
		this.m01 = matrix.m01;
		this.m10 = matrix.m10;
		this.m11 = matrix.m11;
		return this;
	}
	
	// Set the matrix to zero
	public final Matrix2f setZero() {
		this.m00 = 0.0f;
		this.m01 = 0.0f;
		this.m10 = 0.0f;
		this.m11 = 0.0f;
		return this;
	}
	
	@Override
	public String toString() {
		return "(00= " + this.m00 + ", 01= " + this.m01 + ", 10= " + this.m10 + ", 11= " + this.m11 + ")";
	}
	
	// Return the transpose matrix
	public Matrix2f transpose() {
		set(this.m00, this.m10, this.m01, this.m11);
		return this;
	}
	
}

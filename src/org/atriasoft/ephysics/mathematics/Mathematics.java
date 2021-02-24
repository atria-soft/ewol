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

import org.atriasoft.etk.math.Vector3f;

/**
 *
 * @author Jason Sorensen <sorensenj@smert.net>
 */
public class Mathematics {
	
	// Function to test if two real numbers are (almost) equal
	// We test if two numbers a and b are such that (a-b) are in [-EPSILON; EPSILON]
	public static boolean ApproxEqual(final float a, final float b, final float epsilon) {
		final float difference = a - b;
		return (difference < epsilon && difference > -epsilon);
	}
	
	public static float ArcCos(final float radians) {
		return (float) StrictMath.acos(radians);
	}
	
	public static float ArcSin(final float radians) {
		return (float) StrictMath.asin(radians);
	}
	
	public static float ArcTan2(final float a, final float b) {
		return (float) StrictMath.atan2(a, b);
	}
	
	// Function that returns the result of the "value" clamped by
	// two others values "lowerLimit" and "upperLimit"
	public static float Clamp(final float value, final float lowerLimit, final float upperLimit) {
		assert (lowerLimit <= upperLimit);
		return Math.min(Math.max(value, lowerLimit), upperLimit);
	}
	
	/// Compute the barycentric coordinates u, v, w of a point p inside the triangle (a, b, c)
	/// This method uses the technique described in the book Real-Time collision detection by
	/// Christer Ericson.
	public static void computeBarycentricCoordinatesInTriangle(final Vector3f a, final Vector3f b, final Vector3f c, final Vector3f p, Float u, Float v, Float w) {
		final Vector3f v0 = b.lessNew(a);
		final Vector3f v1 = c.lessNew(a);
		final Vector3f v2 = p.lessNew(a);
		
		final float d00 = v0.dot(v0);
		final float d01 = v0.dot(v1);
		final float d11 = v1.dot(v1);
		final float d20 = v2.dot(v0);
		final float d21 = v2.dot(v1);
		
		final float denom = d00 * d11 - d01 * d01;
		v = (d11 * d20 - d01 * d21) / denom;
		w = (d00 * d21 - d01 * d20) / denom;
		u = 1.0f - v - w;
	}
	
	public static float Cos(final float radians) {
		return (float) StrictMath.cos(radians);
	}
	
	public static float Sin(final float radians) {
		return (float) StrictMath.sin(radians);
	}
	
	public static float Sqrt(final float a) {
		return (float) StrictMath.sqrt(a);
	}
	
	public static float Tan(final float radians) {
		return (float) StrictMath.tan(radians);
	}
}

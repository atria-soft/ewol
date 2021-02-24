package org.atriasoft.ephysics;

public interface RaycastCallback {
	/**
	 * @brief This method will be called for each ProxyShape that is hit by the
	 * ray. You cannot make any assumptions about the order of the
	 * calls. You should use the return value to control the continuation
	 * of the ray. The returned value is the next maxFraction value to use.
	 * If you return a fraction of 0.0, it means that the raycast should
	 * terminate. If you return a fraction of 1.0, it indicates that the
	 * ray is not clipped and the ray cast should continue as if no hit
	 * occurred. If you return the fraction in the parameter (hitFraction
	 * value in the RaycastInfo object), the current ray will be clipped
	 * to this fraction in the next queries. If you return -1.0, it will
	 * ignore this ProxyShape and continue the ray cast.
	 * @param[in] _raycastInfo Information about the raycast hit
	 * @return Value that controls the continuation of the ray after a hit
	 */
	float notifyRaycastHit(RaycastInfo _raycastInfo);
}

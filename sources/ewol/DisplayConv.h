/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __DISPLAY_CONV_H__
#define __DISPLAY_CONV_H__

#include <etk/types.h>
#include <etk/UString.h>
#include <etk/math/Vector2D.h>

namespace ewol
{
	/**
	 * @brief basic init
	 */
	void DC_Init(void);
	/**
	 * @brief convert a dimention in Inch to a pixel dimension
	 * @param[in] inch Dimention in Inch
	 * @return dimention in Pixel
	 */
	vec2 InchToPixel(const vec2& inch);
	/**
	 * @brief convert a dimention in millimeter to a pixel dimension
	 * @param[in] millimeter Dimention in millimeter
	 * @return dimention in Pixel
	 */
	vec2 MillemeterToPixel(const vec2& millimeter);
	/**
	 * @brief convert a dimention in pourcent to a pixel dimension
	 * @param[in] pourcent Dimention in pourcent
	 * @return dimention in Pixel
	 */
	vec2 PourcentToPixel(const vec2& pourcent);
	/**
	 * @brief convert a dimention in Pixel to a inch dimension
	 * @param[in] pixel Dimention in pixel
	 * @return dimention in Inch
	 */
	vec2 PixelToInch(const vec2& pixel);
	/**
	 * @brief convert a dimention in Pixel to a Millemeter dimension
	 * @param[in] pixel Dimention in pixel
	 * @return dimention in Millemeter
	 */
	vec2 PixelToMillemeter(const vec2& pixel);
	/**
	 * @brief convert a dimention in Pixel to a pourcent dimension
	 * @param[in] pixel Dimention in pixel
	 * @return dimention in pourcent
	 */
	vec2 PixelToPourcent(const vec2& pixel);
	/**
	 * @brief Set the Inch ratio for calculation
	 * @param[in] Ratio Inch ration for the screen calculation interpolation
	 * @note: same as @ref SetPixelPerMillimeter (internal manage convertion)
	 */
	void SetPixelPerInch(const vec2& ratio);
	/**
	 * @brief Set the Milimeter ratio for calculation
	 * @param[in] Ratio Milimeter ration for the screen calculation interpolation
	 * @note: same as @ref SetPixelPerInch (internal manage convertion)
	 */
	void SetPixelPerMillimeter(const vec2& ratio);
	/**
	 * @brief Set the current Windows Size
	 * @param[in] size Size of the current windows in pixel.
	 */
	void SetPixelWindowsSize(const vec2& size);
	/**
	 * @brief Get the Windows Size in the request unit
	 * @return the requested size
	 */
	vec2 GetWindowsSizeMilimeter(void);
	/**
	 * @brief Get the Windows Size in the request unit
	 * @return the requested size
	 */
	vec2 GetWindowsSizeInch(void);
	/**
	 * @brief Get the Windows diagonal size in the request unit
	 * @return the requested size
	 */
	float GetWindowsDiagSizeMilimeter(void);
	/**
	 * @brief Get the Windows diagonal size in the request unit
	 * @return the requested size
	 */
	float GetWindowsDiagSizeInch(void);
};

#endif


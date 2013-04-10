/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_DIMENSION_H__
#define __EWOL_DIMENSION_H__

#include <etk/types.h>
#include <etk/UString.h>
#include <etk/math/Vector2D.h>

namespace ewol
{
	/**
	 * @brief in the dimention class we store the data as the more usefull unit (pixel) 
	 * but one case need to be dynamic the %, then when requested in % the register the % value
	 */
	class Dimension
	{
		public:
			typedef enum {
				Pourcent=0,
				Pixel,
				Meter,
				Centimeter,
				Millimeter,
				Kilometer,
				Inch,
				foot,
			} distance_te;
		private:
			vec2 m_data;
			distance_te m_type;
		public:
			Dimension(const vec2& size, ewol::Dimension::distance_te type=ewol::Dimension::Pixel);
			~Dimension(void);
			/**
			 * @brief Get the current dimention in requested type
			 * @param[in] type Type of unit requested.
			 * @return dimention requested.
			 */
			vec2 Get(ewol::Dimension::distance_te type);
			/**
			 * @brief Set the current dimention in requested type
			 * @param[in] size Dimention to set
			 * @param[in] type Type of unit requested.
			 */
			void Set(const vec2& size, ewol::Dimension::distance_te type);
			/**
			 * @brief Get the current dimention in pixel
			 * @return dimention in Pixel
			 */
			vec2 GetPixel(void);
			/**
			 * @brief Get the current dimention in Pourcent
			 * @return dimention in Pourcent
			 */
			vec2 GetPourcent(void);
			/**
			 * @brief Get the current dimention in Meter
			 * @return dimention in Meter
			 */
			vec2 GetMeter(void);
			/**
			 * @brief Get the current dimention in Centimeter
			 * @return dimention in Centimeter
			 */
			vec2 GetCentimeter(void);
			/**
			 * @brief Get the current dimention in Millimeter
			 * @return dimention in Millimeter
			 */
			vec2 GetMillimeter(void);
			/**
			 * @brief Get the current dimention in Kilometer
			 * @return dimention in Kilometer
			 */
			vec2 GetKilometer(void);
			/**
			 * @brief Get the current dimention in Inch
			 * @return dimention in Inch
			 */
			vec2 GetInch(void);
			/**
			 * @brief Get the current dimention in Foot
			 * @return dimention in Foot
			 */
			vec2 GetFoot(void);
			
	};
	
	namespace dimension
	{
		
		/**
		 * @brief basic init
		 */
		void Init(void);
		/**
		 * @brief basic un-init
		 */
		void UnInit(void);
		/**
		 * @brief Set the Milimeter ratio for calculation
		 * @param[in] Ratio Milimeter ration for the screen calculation interpolation
		 * @param[in] type Unit type requested.
		 * @note: same as @ref SetPixelPerInch (internal manage convertion)
		 */
		void SetPixelRatio(const vec2& ratio, ewol::Dimension::distance_te type);
		/**
		 * @brief Set the current Windows Size
		 * @param[in] size Size of the current windows in pixel.
		 */
		void SetPixelWindowsSize(const vec2& size);
		/**
		 * @brief Get the Windows Size in the request unit
		 * @param[in] type Unit type requested.
		 * @return the requested size
		 */
		vec2 GetWindowsSize(ewol::Dimension::distance_te type);
		/**
		 * @brief Get the Windows diagonal size in the request unit
		 * @param[in] type Unit type requested.
		 * @return the requested size
		 */
		float GetWindowsDiag(ewol::Dimension::distance_te type);
	};
};

#endif


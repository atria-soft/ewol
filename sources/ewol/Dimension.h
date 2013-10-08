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

namespace ewol {
	/**
	 * @brief in the dimention class we store the data as the more usefull unit (pixel) 
	 * but one case need to be dynamic the %, then when requested in % the register the % value
	 */
	class Dimension {
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
			/**
			 * @brief Constructor (default :0,0 mode pixel)
			 */
			Dimension(void);
			/**
			 * @brief Constructor
			 * @param[in] _size Requested dimention
			 * @param[in] _type Unit of the Dimention
			 */
			Dimension(const vec2& _size, ewol::Dimension::distance_te _type=ewol::Dimension::Pixel);
			/**
			 * @brief Constructor
			 * @param[in] _config dimension configuration.
			 */
			Dimension(const etk::UString& _config) : m_data(0,0),m_type(ewol::Dimension::Pixel) { set(_config); };
			/**
			 * @brief Constructor
			 * @param[in] _config dimension configuration.
			 */
			Dimension(const char* _config) : m_data(0,0),m_type(ewol::Dimension::Pixel) { set(_config); };
			/**
			 * @brief Destructor
			 */
			~Dimension(void);
			
			/**
			 * @brief string cast :
			 */
			operator etk::UString(void) const;
			
			/**
			 * @brief get the current dimention in requested type
			 * @param[in] _type Type of unit requested.
			 * @return dimention requested.
			 */
			vec2 get(ewol::Dimension::distance_te _type) const;
			/**
			 * @brief set the current dimention in requested type
			 * @param[in] _size Dimention to set
			 * @param[in] _type Type of unit requested.
			 */
			void set(const vec2& _size, ewol::Dimension::distance_te _type);
			
		private:
			/**
			 * @brief set the current dimention in requested type
			 * @param[in] _config dimension configuration.
			 */
			void set(etk::UString _config);
		public:
			/**
			 * @brief get the current dimention in pixel
			 * @return dimention in Pixel
			 */
			vec2 getPixel(void) const;
			/**
			 * @brief get the current dimention in Pourcent
			 * @return dimention in Pourcent
			 */
			vec2 getPourcent(void) const;
			/**
			 * @brief get the current dimention in Meter
			 * @return dimention in Meter
			 */
			vec2 getMeter(void) const;
			/**
			 * @brief get the current dimention in Centimeter
			 * @return dimention in Centimeter
			 */
			vec2 getCentimeter(void) const;
			/**
			 * @brief get the current dimention in Millimeter
			 * @return dimention in Millimeter
			 */
			vec2 getMillimeter(void) const;
			/**
			 * @brief get the current dimention in Kilometer
			 * @return dimention in Kilometer
			 */
			vec2 getKilometer(void) const;
			/**
			 * @brief get the current dimention in Inch
			 * @return dimention in Inch
			 */
			vec2 getInch(void) const;
			/**
			 * @brief get the current dimention in Foot
			 * @return dimention in Foot
			 */
			vec2 getFoot(void) const;
			/*****************************************************
			 *    = assigment
			 *****************************************************/
			const Dimension& operator= (const Dimension& _obj ) {
				if (this!=&_obj) {
					m_data = _obj.m_data;
					m_type = _obj.m_type;
				}
				return *this;
			}
			/*****************************************************
			 *    == operator
			 *****************************************************/
			bool operator ==  (const Dimension& _obj) const {
				if(    m_data == _obj.m_data
				    && m_type == _obj.m_type) {
					return true;
				}
				return false;
			}
			/*****************************************************
			 *    != operator
			 *****************************************************/
			bool operator!= (const Dimension& _obj) const {
				if(    m_data != _obj.m_data
				    || m_type != _obj.m_type) {
					return true;
				}
				return false;
			}
			/**
			 * @breif get the dimension type
			 * @return the type
			 */
			ewol::Dimension::distance_te getType(void) const { return m_type; };
	};
	etk::CCout& operator <<(etk::CCout& _os, const ewol::Dimension::distance_te& _obj);
	etk::CCout& operator <<(etk::CCout& _os, const ewol::Dimension& _obj);
	
	namespace dimension {
		
		/**
		 * @brief basic init
		 */
		void init(void);
		/**
		 * @brief basic un-init
		 */
		void unInit(void);
		/**
		 * @brief set the Milimeter ratio for calculation
		 * @param[in] Ratio Milimeter ration for the screen calculation interpolation
		 * @param[in] type Unit type requested.
		 * @note: same as @ref setPixelPerInch (internal manage convertion)
		 */
		void setPixelRatio(const vec2& _ratio, ewol::Dimension::distance_te _type);
		/**
		 * @brief set the current Windows size
		 * @param[in] size size of the current windows in pixel.
		 */
		void setPixelWindowsSize(const vec2& _size);
		/**
		 * @brief get the Windows size in the request unit
		 * @param[in] type Unit type requested.
		 * @return the requested size
		 */
		vec2 getWindowsSize(ewol::Dimension::distance_te _type);
		/**
		 * @brief get the Windows diagonal size in the request unit
		 * @param[in] type Unit type requested.
		 * @return the requested size
		 */
		float getWindowsDiag(ewol::Dimension::distance_te _type);
	};
};

#endif


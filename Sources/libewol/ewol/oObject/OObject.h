/**
 *******************************************************************************
 * @file ewol/oObject/OObject.h
 * @brief ewol OpenGl Object system (header)
 * @author Edouard DUPIN
 * @date 24/10/2011
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#ifndef __EWOL_O_OBJECT_H__
#define __EWOL_O_OBJECT_H__

#include <etk/Types.h>
#include <draw/Color.h>
#include <etk/File.h>
#include <ewol/Debug.h>
#include <ewol/font/Font.h>
#include <etk/Vector.h>

namespace ewol {
	extern "C" {
		typedef struct {
			int32_t f;
			int32_t s;
			int32_t t;
		}linkCoord_ts;
	};
	
	class OObject
	{
		protected:
			int32_t     m_nbLoadedTime;     //!< specific in case of non multiple allocation
			bool        m_hasClipping;
			clipping_ts m_clipping;
			Vector2D<float>   m_scaling;           //!< scaling ol the object
		public:
			OObject(void);
			virtual ~OObject(void);
			void clippingSet(clipping_ts clip) {m_clipping = clip; m_hasClipping = true;};
			void clippingDisable(void) {m_hasClipping = false;};
			void clippingEnable(void) {m_hasClipping = true;};
			void scalingSet(Vector2D<float>  scale) {m_scaling = scale;};
			virtual void Draw(void) = 0;
			/**
			 * @brief Increase the number of element registered on this class ==> this is specific to decrese the memory usage in special case (scene)
			 */
			void IncreaseLoadedTime(void) { m_nbLoadedTime++;};
			/**
			 * @brief Decrease the number of element registered on this class ==> this is specific to decrese the memory usage in special case (scene)
			 * @return true, if no more element registered on it ...
			 */
			bool DecreaseLoadedTime(void) { m_nbLoadedTime--; if (m_nbLoadedTime <= 0) { return true;} else { return false;} };
	};
};

#endif


#include <ewol/oObject/2DTextured.h>
#include <ewol/oObject/2DColored.h>
#include <ewol/oObject/2DTextColored.h>

#ifdef __VIDEO__OPENGL_ES_2
#include <ewol/oObject/2DTextShader.h>
#endif

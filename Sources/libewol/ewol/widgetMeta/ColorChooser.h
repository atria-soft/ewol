/**
 *******************************************************************************
 * @file ewol/widgetMeta/ColorChooser.h
 * @brief ewol Color chooser meta widget system (header)
 * @author Edouard DUPIN
 * @date 05/03/2012
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

#ifndef __EWOL_COLOR_CHOOSER_H__
#define __EWOL_COLOR_CHOOSER_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/SizerVert.h>
#include <ewol/widget/ColorBar.h>
#include <ewol/widget/Slider.h>

extern const char * const ewolEventColorChooserChange;

namespace ewol {
	class ColorChooser : public ewol::SizerVert
	{
		public:
			ColorChooser(void);
			~ColorChooser(void);
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EwolColorChooser"; };
			/**
			 * @brief Receive a message from an other EObject with a specific eventId and data
			 * @param[in] CallerObject Pointer on the EObject that information came from
			 * @param[in] eventId Message registered by this class
			 * @param[in] data Data registered by this class
			 * @return ---
			 */
			virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
			/**
			 * @brief Inform object that an other object is removed ...
			 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
			 * @note : Sub classes must call this class
			 * @return ---
			 */
			virtual void OnObjectRemove(ewol::EObject * removeObject);
			
			void     SetColor(color_ts newColor);
			color_ts GetColor(void);
		private:;
			ewol::ColorBar*     m_widgetColorBar;
			ewol::Slider*       m_widgetRed;
			ewol::Slider*       m_widgetGreen;
			ewol::Slider*       m_widgetBlue;
			ewol::Slider*       m_widgetAlpha;
			color_ts            m_currentColor;
	};
	
};

#endif

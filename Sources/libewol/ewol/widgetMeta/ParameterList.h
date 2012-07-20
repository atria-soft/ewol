/**
 *******************************************************************************
 * @file ewol/widget/ListFile.h
 * @brief ewol File lister widget system (header)
 * @author Edouard DUPIN
 * @date 12/07/2012
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

#ifndef __EWOL_WIDGET_PARAMETER_LIST_H__
#define __EWOL_WIDGET_PARAMETER_LIST_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/widget/WidgetScrolled.h>
#include <ewol/widget/Drawable.h>

extern const char * const ewolEventParameterListSelect;


namespace ewol {
	
	extern const char * const TYPE_EOBJECT_WIDGET_PARAMETER_LIST;
	
	class elementPL
	{
		public :
			bool            m_group;
			etk::UString    m_label;
			int32_t         m_refId;
			etk::UString    m_image;
			elementPL(etk::UString& label, int32_t refId, etk::UString& image, bool isGroup) :
				m_group(isGroup),
				m_label(label),
				m_refId(refId),
				m_image(image)
			{ };
			~elementPL(void) {};
	};
	
	class ParameterList :public ewol::WidgetScrooled
	{
		private:
			int32_t                            m_idSelected;
			etk::VectorType<ewol::elementPL *> m_list;
		public:
			ParameterList(void);
			/**
			 * @brief Check if the object has the specific type.
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type of the object we want to check
			 * @return true if the object is compatible, otherwise false
			 */
			virtual bool CheckObjectType(const char * const objectType);
			
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void);
			virtual ~ParameterList(void);
			virtual bool   CalculateMinSize(void);
			void           SetLabel(etk::UString newLabel);
		// Drawing capabilities ....
		private:
			etk::VectorType<ewol::OObject*> m_listOObject[NB_BOUBLE_BUFFER];   //!< generic element to display...
		public:
			void    AddOObject(ewol::OObject* newObject, int32_t pos=-1);
			void    ClearOObjectList(void);
		protected:
			void OnDraw(DrawProperty& displayProp);
		// list properties ...
		private:
			int32_t        m_paddingSizeX;
			int32_t        m_paddingSizeY;
			int32_t        m_displayStartRaw;           //!< Current starting diaplayed raw
			int32_t        m_displayCurrentNbLine;      //!< Number of line in the display
		public:
			void   OnRegenerateDisplay(void);
			/**
			 * @brief Event on an input of this Widget
			 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
			 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
			 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
			 * @param[in] pos Absolute position of the event
			 * @return true the event is used
			 * @return false the event is not used
			 */
			bool   OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, Vector2D<float>  pos);
		protected:
			void OnGetFocus(void);
			void OnLostFocus(void);
		public:
			void MenuAdd(etk::UString& label, int32_t refId, etk::UString& image);
			void MenuAddGroup(etk::UString& label);
			void MenuClear(void);
			void MenuSeparator(void);
	};
};

#define EWOL_CAST_WIDGET_PARAMETER_LIST(curentPointer) EWOL_CAST(ewol::TYPE_EOBJECT_WIDGET_PARAMETER_LIST,ParameterList,curentPointer)

#endif


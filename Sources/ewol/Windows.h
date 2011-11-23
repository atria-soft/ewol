/**
 *******************************************************************************
 * @file ewolWindows.h
 * @brief ewol window system (header)
 * @author Edouard DUPIN
 * @date 20/10/2011
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

#ifndef __EWOL_WINDOWS_H__
#define __EWOL_WINDOWS_H__

#include <etkTypes.h>
#include <ewolDebug.h>
#include <etkVectorType.h>
#include <etkSingleton.h>
#include <ewolWidget.h>

namespace ewol {
	class Windows :public ewol::Widget
	{
		public:
			Windows(void);
			virtual ~Windows(void);
		// internal event at ewol system : 
		public:
			void SysDraw(void);
			void SysOnShow(void) {};
			void SysOnHide(void) {};
			void SysOnKill(void) {};
			void SysOnExpose(void) {};
		public:
			virtual void OnShow(void) { };
			virtual void OnHide(void) { };
			virtual bool OnKill(void) { return true; };
			virtual void OnReduce(void) { };
			virtual void On(void) { };
		// from Widget management : 
			virtual bool CalculateSize(etkFloat_t availlableX, etkFloat_t availlableY);
			virtual bool OnEventArea(const char * generateEventId, etkFloat_t x, etkFloat_t y);
		// Widget overwrite function
		public:
			virtual bool OnEventInput(int32_t IdInput, eventInputType_te typeEvent, etkFloat_t x, etkFloat_t y);
		private:
			bool m_hasDecoration;
		public:
			void SetDecorationDisable(void)
			{
				m_hasDecoration = false;
			}
			
			void SetDecorationEnable(void)
			{
				m_hasDecoration = true;
			}
		private:
			ewol::Widget *  m_subWidget;
		public:
			void SetSubWidget(ewol::Widget * widget);
		protected:
			virtual bool   OnDraw(void);
		public:
			virtual void OnRegenerateDisplay(void);
	};
};

#endif


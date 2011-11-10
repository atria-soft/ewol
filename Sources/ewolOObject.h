/**
 *******************************************************************************
 * @file ewolOObject.h
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

#include <etkTypes.h>
#include <etkFile.h>
#include <ewolDebug.h>
#include <ewolFont.h>
#include <etkVectorType.h>

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
		public:
			OObject(void);
			virtual ~OObject(void);
		public:
			virtual void Draw(void) = 0;
			void SetName(etk::String & name);
			void SetName(const char * name);
			etk::String GetName(void);
		public:
			// use to crop element outside the display
			virtual void UpdateSize(etkFloat_t sizeX, etkFloat_t sizeY) { };
			// Move to the correct position display
			virtual void UpdateOrigin(etkFloat_t x, etkFloat_t y) { };
		private:
			etk::String m_name;
	};
	
	/*
	class OObjectFile :public ewol::OObject
	{
		public:
			OObjectFile(etk::File fileName) {};
			virtual ~OObject2DColored(void) {};
		public:
			void Draw(void);
			bool Save(etk::File fileName) { return false; };
		protected:
			etk::VectorType<OObject*>   m_listsubObject;   //!< an object file contain more than one object...
			bool                        m_isBinaryFile;    //!< to know th mode of saving the file
	};
	*/
};

#endif


#include <ewolOObject2DTextured.h>
#include <ewolOObject2DColored.h>
#include <ewolOObject2DText.h>


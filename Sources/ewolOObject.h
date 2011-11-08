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
			OObject(void) { m_name=""; };
			virtual ~OObject(void) {};
		public:
			virtual void Draw(void) = 0;
			void SetName(etk::String & name)
			{
				m_name = name;
			}
			void SetName(const char * name)
			{
				if (NULL != name) {
					m_name = name;
				}
			}
			etk::String GetName(void)
			{
				return m_name;
			}
		public:
			virtual void UpdateOrigin(float x, float y) {};
		private:
			etk::String m_name;
	};
	
	class OObject2DColored :public ewol::OObject
	{
		public:
			OObject2DColored(void) {};
			virtual ~OObject2DColored(void) {};
		public:
			virtual void Draw(void);
		protected:
			etk::VectorType<coord2D_ts>   m_coord;       //!< internal coord of the object
			etk::VectorType<color_ts>     m_coordColor;  //!< internal color of the different point
			//etk::VectorType<linkCoord_ts> m_linkCoord;   //!< internal link between point to generate triangle
		public:
			void Rectangle(float x, float y, float w, float h, float red, float green, float blue, float alpha);
		public:
			virtual void UpdateOrigin(float x, float y);
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
	
	class OObject2DTextured :public ewol::OObject
	{
		public:
			OObject2DTextured(etk::File textureName);
			virtual ~OObject2DTextured(void);
		public:
			virtual void Draw(void);
			void Rectangle(float x, float y, float w, float h, float texX=0.0, float texY=0.0, float texSX=1.0, float texSY=1.0);
		protected:
			uint32_t                      m_textureId;   //!< texture internal ID
			etk::VectorType<coord2D_ts>   m_coord;       //!< internal coord of the object
			etk::VectorType<texCoord_ts>  m_coordTex;    //!< internal texture coordinate for every point
		public:
			virtual void UpdateOrigin(float x, float y);
	};
	
	
	class OObject2DText :public ewol::OObject
	{
		public:
			OObject2DText(float x, float y, etk::String FontName, int32_t size, fontMode_te mode, color_ts textColorFg, const char* utf8String);
			virtual ~OObject2DText(void);
		public:
			virtual void Draw(void);
			// set a specific text
			void Text(float x, float y, etk::String FontName, int32_t size, fontMode_te mode, color_ts textColorFg, const char* utf8String);
		protected:
			int32_t                       m_FontId;        //!< font internal ID
			color_ts                      m_textColorFg;   //!< text color ...
			uint32_t                      m_FontTextureId; //!< font internal Texture ID
			etk::VectorType<coord2D_ts>   m_coord;         //!< internal coord of the object
			etk::VectorType<texCoord_ts>  m_coordTex;      //!< internal texture coordinate for every point
		public:
			virtual void UpdateOrigin(float x, float y);
	};
	
	/*
	class OObject2DTextMultiple :public ewol::OObject
	{
		public:
			OObject2DText(etk::String FontName);
			virtual ~OObject2DText(void);
		public:
			virtual void Draw(void);
			// set a specific text
			void TextAdd(float x, float y, int32_t size, fontMode_te mode, const char* utf8String);
			void Clear(void);
		protected:
			int32_t                       m_FontId;                          //!< font internal ID
			uint32_t                      m_FontTextureId[FONT_MODE_NUMBER]; //!< font internal Texture ID
			etk::VectorType<coord2D_ts>   m_coord[FONT_MODE_NUMBER];         //!< internal coord of the object
			etk::VectorType<texCoord_ts>  m_coordTex[FONT_MODE_NUMBER];      //!< internal texture coordinate for every point
	};
	*/
};

#endif


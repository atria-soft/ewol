/**
 *******************************************************************************
 * @file ewol/OObject/e2d.h
 * @brief ewol OpenGl e2d Object system (header)
 * @author Edouard DUPIN
 * @date 13/03/2012
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

#ifndef __EWOL_O_OBJECT_E2D_H__
#define __EWOL_O_OBJECT_E2D_H__

#include <ewol/OObject.h>
#include <ewol/OObject/2DColored.h>


typedef struct {
	int32_t  dot[3];
	color_ts color[3];
} link_ts;


namespace ewol {
	namespace oobject {
		class e2d :public ewol::OObject2DColored
		{
			protected:
				etk::VectorType<coord2D_ts>   m_dotList;       //!< list of all point in the buffer
				etk::VectorType<link_ts>      m_linkList;      //!< List of all triangle in the mesh
				etk::File                     m_fileName;      //!< e2d file name ...
				coord2D_ts                    m_size;          //!< User requested Size ...
			public:
				e2d(void);
				e2d(etk::File file);
				virtual ~e2d(void);
				void LoadFile(etk::File file);
				void SetSize(coord2D_ts newSize) { m_size = newSize; m_size.x=etk_max(m_size.x,0); m_size.y=etk_max(m_size.y,0);};
				virtual void Draw(void);
		};
	};
};

#endif


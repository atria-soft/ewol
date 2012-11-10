/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_TEXTURE_SVG_H__
#define __EWOL_TEXTURE_SVG_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <parserSVG/parserSVG.h>

namespace ewol
{
	namespace texture {
		class TextureSVG
		{
			private:
				svg::Parser         m_elementParsed;
				int32_t             m_width;
				int32_t             m_height;
				int32_t             m_size;
				uint8_t *           m_data;
				bool                m_loadOK;
			public:
				TextureSVG(etk::UString & fileName, int32_t width, int32_t height);
				~TextureSVG(void);
				bool LoadOK(void);
				int32_t Width(void);
				int32_t Height(void);
				uint8_t * Data(void);
				uint32_t DataSize(void);
				void Display(void);
		};
	};
};

#endif


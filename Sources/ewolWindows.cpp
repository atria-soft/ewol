/**
 *******************************************************************************
 * @file ewolWindows.cpp
 * @brief ewol window system (sources)
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

#include <etkTypes.h>
#include <etkString.h>
#include <ewolWidget.h>
#include <ewolWindows.h>
#include <ewolOObject.h>
#include <ewolTexture.h>
#include <ewolFont.h>
#include <ewol.h>
#include <GL/gl.h>



#undef __class__
#define __class__	"ewol::Windows"


//list of local events : 
const char * ewolEventWindowsClose    = "ewol Windows close";
const char * ewolEventWindowsMinimize = "ewol Windows minimize";
const char * ewolEventWindowsExpend   = "ewol Windows expend/unExpend";


ewol::Windows::Windows(void)
{
	ewol::OObject2DColored * myOObject = new ewol::OObject2DColored();
	myOObject->Rectangle( 0, 0, 20, 20,  1.0, 0.0, 0.0, 1.0); // Close
	myOObject->Rectangle(20, 0, 20, 20,  0.0, 1.0, 0.0, 1.0); // Reduce
	myOObject->Rectangle(40, 0, 20, 20,  0.0, 0.0, 1.0, 1.0); // Expend - Un-expend
	
	AddEventArea({ 0.0,0.0}, {20, 20}, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED_ALL, ewolEventWindowsClose);
	AddEventArea({20.0,0.0}, {20, 20}, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED_ALL, ewolEventWindowsMinimize);
	AddEventArea({40.0,0.0}, {20, 20}, FLAG_EVENT_INPUT_1 | FLAG_EVENT_INPUT_CLICKED_ALL, ewolEventWindowsExpend);
	
	AddOObject(myOObject, "leftBoutton");
	
	color_ts textColorFg;
	textColorFg.red = .0;
	textColorFg.green = .0;
	textColorFg.blue = .0;
	textColorFg.alpha = 1.0;
	ewol::OObject2DText * myOObjectText = new ewol::OObject2DText(62, 2, "Monospace", 17 , FONT_MODE_BOLD, textColorFg, "My Title ...");
	AddOObject(myOObjectText, "Title");
}



bool ewol::Windows::CalculateSize(double availlableX, double availlableY)
{
	m_size.x = availlableX;
	m_size.y = availlableY;
	return true;
}


bool ewol::Windows::OnEventInput(int32_t IdInput, eventInputType_te typeEvent, double x, double y)
{
	if(    x >= 60
	    && y <=20)
	{
		if(EVENT_INPUT_TYPE_MOVE == typeEvent && true == ewol::IsPressedInput(1) ) {
			ewol::StartMoveSystem();
		}
	}
	if(    x >= m_size.x - 20
	    && y >= m_size.y - 20)
	{
		if(EVENT_INPUT_TYPE_MOVE == typeEvent && true == ewol::IsPressedInput(1) ) {
			ewol::StartResizeSystem();
		}
	}
	return true;
}



#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <GL/glut.h>
#if defined(EWOL_X11_MODE__XF86V)
#	include <X11/extensions/xf86vmode.h>
#elif defined(EWOL_X11_MODE__XRENDER)
#	include <X11/extensions/Xrender.h>
#endif


#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library library; // handle to library
FT_Face face; // handle to face object

void ewol::Windows::SysDraw(void)
{

	//EWOL_DEBUG("Drow on (" << m_size.x << "," << m_size.y << ")");
	// set the size of the open GL system
	glViewport(0,0,m_size.x,m_size.y);
	
	// Clear the screen with transparency ...
	glClearColor(0.750, 0.750, 0.750, 0.5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0., m_size.x, 0., -m_size.y, 1., 20.);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, -m_size.y, -5);
	
	//http://www.khronos.org/opengles/documentation/opengles1_0/html/glBlendFunc.html
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
	
	GenDraw();
	
	static bool initTest = false;
	static uint32_t TMPtextureid;
	if (false == initTest) {
		initTest = true;
		int32_t error = FT_Init_FreeType( &library );
		if(0 != error) {
			EWOL_CRITICAL("... an error occurred during library initialization ...");
		} else {
			etk::File myFile = "Font/freefont/FreeMono.ttf";
			
			error = FT_New_Face( library, myFile.GetCompleateName().c_str(), 0, &face );
			if( FT_Err_Unknown_File_Format == error) {
				EWOL_ERROR("... the font file could be opened and read, but it appears ... that its font format is unsupported");
			} else if (0 != error) {
				EWOL_ERROR("... another error code means that the font file could not ... be opened or read, or simply that it is broken...");
			} else {
				// all OK
				EWOL_INFO("load font : \"" << myFile << "\" ");
				EWOL_INFO("    nuber of glyph       = " << face->num_glyphs);
				if ((FT_FACE_FLAG_SCALABLE & face->face_flags) != 0) {
					EWOL_INFO("    flags                = FT_FACE_FLAG_SCALABLE (enable)");
				} else {
					EWOL_DEBUG("    flags                = FT_FACE_FLAG_SCALABLE (disable)");
				}
				if ((FT_FACE_FLAG_FIXED_SIZES & face->face_flags) != 0) {
					EWOL_INFO("    flags                = FT_FACE_FLAG_FIXED_SIZES (enable)");
				} else {
					EWOL_DEBUG("    flags                = FT_FACE_FLAG_FIXED_SIZES (disable)");
				}
				if ((FT_FACE_FLAG_FIXED_WIDTH & face->face_flags) != 0) {
					EWOL_INFO("    flags                = FT_FACE_FLAG_FIXED_WIDTH (enable)");
				} else {
					EWOL_DEBUG("    flags                = FT_FACE_FLAG_FIXED_WIDTH (disable)");
				}
				if ((FT_FACE_FLAG_SFNT & face->face_flags) != 0) {
					EWOL_INFO("    flags                = FT_FACE_FLAG_SFNT (enable)");
				} else {
					EWOL_DEBUG("    flags                = FT_FACE_FLAG_SFNT (disable)");
				}
				if ((FT_FACE_FLAG_HORIZONTAL & face->face_flags) != 0) {
					EWOL_INFO("    flags                = FT_FACE_FLAG_HORIZONTAL (enable)");
				} else {
					EWOL_DEBUG("    flags                = FT_FACE_FLAG_HORIZONTAL (disable)");
				}
				if ((FT_FACE_FLAG_VERTICAL & face->face_flags) != 0) {
					EWOL_INFO("    flags                = FT_FACE_FLAG_VERTICAL (enable)");
				} else {
					EWOL_DEBUG("    flags                = FT_FACE_FLAG_VERTICAL (disable)");
				}
				if ((FT_FACE_FLAG_KERNING & face->face_flags) != 0) {
					EWOL_INFO("    flags                = FT_FACE_FLAG_KERNING (enable)");
				} else {
					EWOL_DEBUG("    flags                = FT_FACE_FLAG_KERNING (disable)");
				}
				/* Deprecated flag
				if ((FT_FACE_FLAG_FAST_GLYPHS & face->face_flags) != 0) {
					EWOL_INFO("    flags                = FT_FACE_FLAG_FAST_GLYPHS (enable)");
				} else {
					EWOL_DEBUG("    flags                = FT_FACE_FLAG_FAST_GLYPHS (disable)");
				}
				*/
				if ((FT_FACE_FLAG_MULTIPLE_MASTERS & face->face_flags) != 0) {
					EWOL_INFO("    flags                = FT_FACE_FLAG_MULTIPLE_MASTERS (enable)");
				} else {
					EWOL_DEBUG("    flags                = FT_FACE_FLAG_MULTIPLE_MASTERS (disable)");
				}
				if ((FT_FACE_FLAG_GLYPH_NAMES & face->face_flags) != 0) {
					EWOL_INFO("    flags                = FT_FACE_FLAG_GLYPH_NAMES (enable)");
				} else {
					EWOL_DEBUG("    flags                = FT_FACE_FLAG_GLYPH_NAMES (disable)");
				}
				if ((FT_FACE_FLAG_EXTERNAL_STREAM & face->face_flags) != 0) {
					EWOL_INFO("    flags                = FT_FACE_FLAG_EXTERNAL_STREAM (enable)");
				} else {
					EWOL_DEBUG("    flags                = FT_FACE_FLAG_EXTERNAL_STREAM (disable)");
				}
				if ((FT_FACE_FLAG_HINTER & face->face_flags) != 0) {
					EWOL_INFO("    flags                = FT_FACE_FLAG_HINTER (enable)");
				} else {
					EWOL_DEBUG("    flags                = FT_FACE_FLAG_HINTER (disable)");
				}
				if ((FT_FACE_FLAG_CID_KEYED & face->face_flags) != 0) {
					EWOL_INFO("    flags                = FT_FACE_FLAG_CID_KEYED (enable)");
				} else {
					EWOL_DEBUG("    flags                = FT_FACE_FLAG_CID_KEYED (disable)");
				}
				if ((FT_FACE_FLAG_TRICKY & face->face_flags) != 0) {
					EWOL_INFO("    flags                = FT_FACE_FLAG_TRICKY (enable)");
				} else {
					EWOL_DEBUG("    flags                = FT_FACE_FLAG_TRICKY (disable)");
				}
				EWOL_INFO("    unit per EM          = " << face->units_per_EM);
				EWOL_INFO("    num of fixed sizes   = " << face->num_fixed_sizes);
				EWOL_INFO("    Availlable sizes     = " << face->available_sizes);
				
				EWOL_INFO("    Current size         = " << face->size);
				
				// set size : 
				int32_t fontSize = 12;
				int32_t fontQuality = 96; // 300dpi (hight quality) 96 dpi (normal quality)
				error = FT_Set_Char_Size(face, fontSize<<6, fontSize<<6, fontQuality, fontQuality); // note tha <<6==*64 corespond with the 1/64th of points calculation of freetype
				float lineHeight = fontSize*1.43f; // the line height to have a correct display

				/* retrieve glyph index from character code */
				int32_t glyph_index = FT_Get_Char_Index(face, 'A' );
				/* load glyph image into the slot (erase previous one) */
				error = FT_Load_Glyph(face, /* handle to face object */
				                      glyph_index, /* glyph index */
				                      FT_LOAD_DEFAULT );
				if ( error ) {
					EWOL_ERROR("FT_Load_Glyph");
				}
				/* a small shortcut */
				FT_GlyphSlot slot = face->glyph;
				
				/* convert to an anti-aliased bitmap */
				error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL );
				if ( error ) {
					EWOL_ERROR("FT_Render_Glyph");
				}
				int32_t tmpWidth=slot->bitmap.width;
				int32_t tmpHeight=slot->bitmap.rows;
				
				EWOL_DEBUG("Width=" << tmpWidth);
				EWOL_DEBUG("Height=" << tmpHeight);
				/*
				my_draw_bitmap(&slot->bitmap,
				               pen_x + slot->bitmap_left,
				               pen_y - slot->bitmap_top);
				glEnable(GL_TEXTURE_2D);
				*/
				// Allocate Memory For The Texture Data.
				GLubyte* expanded_data = new GLubyte[ 2 * tmpWidth * tmpHeight];
				
				/*
					Here We Fill In The Data For The Expanded Bitmap.
					Notice That We Are Using A Two Channel Bitmap (One For
					Channel Luminosity And One For Alpha), But We Assign
					Both Luminosity And Alpha To The Value That We
					Find In The FreeType Bitmap.
					We Use The ?: Operator To Say That Value Which We Use
					Will Be 0 If We Are In The Padding Zone, And Whatever
					Is The FreeType Bitmap Otherwise.
				*/
				for(int j=0; j <tmpHeight;j++) {
					for(int i=0; i < tmpWidth; i++){
						expanded_data[2*(i+j*tmpWidth)]= expanded_data[2*(i+j*tmpWidth)+1] = (i>=tmpWidth || j>=tmpHeight) ? 0 : slot->bitmap.buffer[i + tmpWidth*j];
					}
				}
				
				// Now We Just Setup Some Texture Parameters.
				glGenTextures(1, &TMPtextureid);
				glBindTexture( GL_TEXTURE_2D, TMPtextureid);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				
				// Here We Actually Create The Texture Itself, Notice That We Are Using GL_LUMINANCE_ALPHA To Indicate That we Are Using 2 Channel Data.
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tmpWidth, tmpHeight, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );
				
				// With The Texture Created, We Don't Need The Expanded Data Anymore.
				delete [] expanded_data;
			}
		}
	}

	/*
	ewol::OObject2DColored tmpOObjects;
	tmpOObjects.Rectangle( 290, 90, 60, 60,  1.0, 0.0, 0.0, 1.0);
	tmpOObjects.Draw();
	*/


	glColor4f(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TMPtextureid);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(300.0, 100.0, 0.0);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(310.0, 100.0, 0.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(310.0, 112.0, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(300.0, 112.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);


	glDisable(GL_BLEND);
	return;
}


bool ewol::Windows::OnEventArea(const char * generateEventId, double x, double y)
{
	bool eventIsOK = false;
	//EWOL_DEBUG("Receive event : \"" << generateEventId << "\"");
	if(ewolEventWindowsClose == generateEventId) {
		EWOL_INFO("Request close of the windows");
		ewol::Stop();
		eventIsOK = true;
	} else if(ewolEventWindowsMinimize == generateEventId) {
		EWOL_INFO("Request Minimize of the windows");
		eventIsOK = true;
	} else if(ewolEventWindowsExpend == generateEventId) {
		EWOL_INFO("Request Expend of the windows");
		eventIsOK = true;
	}
	return eventIsOK;
}


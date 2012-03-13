/**
 *******************************************************************************
 * @file ewol/OObject/2DColored.cpp
 * @brief ewol OpenGl Object system (Sources)
 * @author Edouard DUPIN
 * @date 09/11/2011
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

#include <ewol/OObject/e2d.h>
#include <ewol/importgl.h>
#include "tinyXML/tinyxml.h"


#undef __class__
#define __class__	"OObject::e2d"


ewol::oobject::e2d::e2d(void)
{
	m_size.x = 30;
	m_size.y = 30;
}

ewol::oobject::e2d::e2d(etk::File file)
{
	LoadFile(file);
}


ewol::oobject::e2d::~e2d(void)
{
	
}


void ewol::oobject::e2d::Draw(void)
{
	if (m_coord.Size()<=0) {
		return;
	}
	glPushMatrix();
	// Enable Pointers
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	
	etkFloat_t minimum = etk_min(m_size.x, m_size.y);
	glScalef( minimum, minimum, 1.0 );
	//glScalef( 30.0, 30.0, 1.0 );
	
	// Set the vertex pointer to our vertex data
	glVertexPointer(2, oglTypeFloat_t, 0, &m_coord[0] );
	glColorPointer(4, oglTypeFloat_t, 0, &m_coordColor[0] );
	// Render : draw all of the triangles at once
	glDrawArrays( GL_TRIANGLES, 0, m_coord.Size());
	
	
	// Disable Pointers
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
	glPopMatrix();
}


void ewol::oobject::e2d::LoadFile(etk::File file)
{
	m_fileName = file;
	// Remove all local elements :
	m_dotList.Clear();
	m_linkList.Clear();
	
	EWOL_DEBUG("open file (e2d) \"" << m_fileName << "\"");

	// allocate the document in the stack
	TiXmlDocument XmlDocument;
	if (false == m_fileName.Exist()) {
		EWOL_ERROR("File Does not exist : " << m_fileName);
		return;
	}
	int32_t fileSize = m_fileName.Size();
	if (0==fileSize) {
		EWOL_ERROR("This file is empty : " << m_fileName);
		return;
	}
	if (false == m_fileName.fOpenRead()) {
		EWOL_ERROR("Can not open the file : " << m_fileName);
		return;
	}
	// allocate data
	char * fileBuffer = new char[fileSize+5];
	if (NULL == fileBuffer) {
		EWOL_ERROR("Error Memory allocation size=" << m_fileName);
		return;
	}
	memset(fileBuffer, 0, (fileSize+5)*sizeof(char));
	// load data from the file :
	m_fileName.fRead(fileBuffer, 1, fileSize);
	// close the file:
	m_fileName.fClose();
	// load the XML from the memory
	XmlDocument.Parse((const char*)fileBuffer, 0, TIXML_ENCODING_UTF8);
	
	TiXmlElement* root = XmlDocument.FirstChildElement( "e2d" );
	if (NULL == root ) {
		EWOL_ERROR("(l ?) main node not find: \"e2d\" in \"" << m_fileName << "\"");
		return;
	} else {
		
		for(TiXmlNode * pNode = root->FirstChild();
		    NULL != pNode;
		    pNode = pNode->NextSibling() ) {
			if (pNode->Type()==TiXmlNode::TINYXML_COMMENT) {
				// nothing to do, just proceed to next step
			} else if (!strcmp(pNode->Value(), "element")) {
				for(TiXmlNode * pGuiNode = pNode->FirstChild();
				    NULL != pGuiNode;
				    pGuiNode = pGuiNode->NextSibling()) {
					if (pGuiNode->Type()==TiXmlNode::TINYXML_COMMENT) {
						// nothing to do, just proceed to next step
					} else if (!strcmp(pGuiNode->Value(), "dot")) {
						const char *xxx = pGuiNode->ToElement()->Attribute("x");
						const char *yyy = pGuiNode->ToElement()->Attribute("y");
						
						if(    NULL != xxx
						    && NULL != yyy) {
							coord2D_ts pos;
							double posX, posY;
							sscanf(xxx, "%lf", &posX);
							sscanf(yyy, "%lf", &posY);
							pos.x = posX;
							pos.y = posY;
							EWOL_DEBUG("load dot : " << xxx << "," << yyy << " ==>" << pos);
							m_dotList.PushBack(pos);
						}
					} else if (!strcmp(pGuiNode->Value(), "link")) {
						const char *id[3];
						const char *color[3];
						id[0] = pGuiNode->ToElement()->Attribute("id1");
						id[1] = pGuiNode->ToElement()->Attribute("id2");
						id[2] = pGuiNode->ToElement()->Attribute("id3");
						color[0] = pGuiNode->ToElement()->Attribute("color1");
						color[1] = pGuiNode->ToElement()->Attribute("color2");
						color[2] = pGuiNode->ToElement()->Attribute("color3");
						
						if(    NULL != id[0]
						    && NULL != id[1]
						    && NULL != id[2]
						    && NULL != color[0]
						    && NULL != color[1]
						    && NULL != color[2]) {
							link_ts localLink;
							int r=0;
							int v=0;
							int b=0;
							int a=-1;
							for(int32_t kkk=0; kkk<3; kkk++) {
								sscanf(id[kkk], "%d", &localLink.dot[kkk]);
								sscanf(color[kkk], "#%02x%02x%02x%02x", &r, &v, &b, &a);
								localLink.color[kkk].red = (float)r/255.0;
								localLink.color[kkk].green = (float)v/255.0;
								localLink.color[kkk].blue = (float)b/255.0;
								if (-1 == a) {
									localLink.color[kkk].alpha = 1;
								} else {
									localLink.color[kkk].alpha = (float)a/255.0;
								}
							}
							EWOL_DEBUG("load link : [" << localLink.dot[0] << "," << localLink.dot[1] << "," << localLink.dot[2] << "] ");
							EWOL_DEBUG("       col: [" << localLink.color[0] << "," << localLink.color[1] << "," << localLink.color[2] << "] ");
							m_linkList.PushBack(localLink);
						}
					} else {
						EWOL_ERROR("(l "<<pGuiNode->Row()<<") node not suported : \""<<pGuiNode->Value()<<"\" must be [dot,link]");
					}
				}
			} else {
				EWOL_ERROR("(l "<<pNode->Row()<<") node not suported : \""<<pNode->Value()<<"\" must be [element]");
			}
		}
	}
	if (NULL != fileBuffer) {
		delete[] fileBuffer;
	}
	// Generate the drawing : 
	
	for (int32_t iii=0; iii<m_linkList.Size(); iii++) {
		for (int32_t jjj=0; jjj<3; jjj++) {
			// set color
			ewol::OObject2DColored::SetColor(m_linkList[iii].color[jjj]);
			// Set the specific dot
			ewol::OObject2DColored::SetPoint(m_dotList[m_linkList[iii].dot[jjj]]);
		}
	}
}



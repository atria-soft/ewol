/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/renderer/resources/Colored3DObject.h>
#include <ewol/renderer/ResourceManager.h>


ewol::Colored3DObject::Colored3DObject(etk::UString genName) :
	ewol::Resource(genName),
	m_GLprogram(NULL)
{
	etk::UString tmpString("DATA:color3.prog");
	// get the shader resource :
	m_GLPosition = 0;
	if (true == ewol::resource::Keep(tmpString, m_GLprogram) ) {
		m_GLPosition = m_GLprogram->GetAttribute("EW_coord3d");
		m_GLColor    = m_GLprogram->GetAttribute("EW_color");
		m_GLMatrix   = m_GLprogram->GetUniform("EW_MatrixTransformation");
	}
}

ewol::Colored3DObject::~Colored3DObject(void)
{
	// remove dynamics dependencies :
	ewol::resource::Release(m_GLprogram);
}


void ewol::Colored3DObject::Draw(etk::Vector<vec3>& vertices,
                                 etk::Vector<draw::Colorf>& color,
                                 bool updateDepthBuffer)
{
	if (vertices.Size()<=0) {
		return;
	}
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	glEnable(GL_DEPTH_TEST);
	if (false==updateDepthBuffer) {
		glDepthMask(GL_FALSE);
	}
	//EWOL_DEBUG("    Display " << m_coord.Size() << " elements" );
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::GetMatrix();
	mat4 camMatrix = ewol::openGL::GetCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix;
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// position :
	m_GLprogram->SendAttribute(m_GLPosition, 3/*x,y,z*/, &vertices[0]);
	// color :
	m_GLprogram->SendAttribute(m_GLColor, 4/*r,g,b,a*/, &color[0]);
	// Request the draw od the elements : 
	glDrawArrays(GL_TRIANGLES, 0, vertices.Size());
	m_GLprogram->UnUse();
	if (false==updateDepthBuffer) {
		glDepthMask(GL_TRUE);
	}
	glDisable(GL_DEPTH_TEST);
}

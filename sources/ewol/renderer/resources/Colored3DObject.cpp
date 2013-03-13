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
	etk::UString tmpString("DATA:simple3D.prog");
	// get the shader resource :
	m_GLPosition = 0;
	if (true == ewol::resource::Keep(tmpString, m_GLprogram) ) {
		m_GLPosition = m_GLprogram->GetAttribute("EW_coord3d");
		m_GLColor    = m_GLprogram->GetUniform("EW_color");
		m_GLMatrix   = m_GLprogram->GetUniform("EW_MatrixTransformation");
	}
}

ewol::Colored3DObject::~Colored3DObject(void)
{
	// remove dynamics dependencies :
	ewol::resource::Release(m_GLprogram);
}


void ewol::Colored3DObject::Draw(etk::Vector<vec3>& vertices,
                                 const draw::Colorf& color,
                                 bool updateDepthBuffer)
{
	if (vertices.Size()<=0) {
		return;
	}
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	ewol::openGL::Enable(ewol::openGL::FLAG_DEPTH_TEST);
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
	m_GLprogram->SendAttribute(m_GLPosition, 3/*x,y,z,unused*/, &vertices[0], 4*sizeof(float));
	// color :
	m_GLprogram->Uniform4fv(m_GLColor, 1/*r,g,b,a*/, (float*)&color);
	// Request the draw od the elements : 
	ewol::openGL::DrawArrays(GL_TRIANGLES, 0, vertices.Size());
	m_GLprogram->UnUse();
	// Request the draw od the elements : 
	//glDrawArrays(GL_LINES, 0, vertices.Size());
	//m_GLprogram->UnUse();
	if (false==updateDepthBuffer) {
		glDepthMask(GL_TRUE);
	}
	ewol::openGL::Disable(ewol::openGL::FLAG_DEPTH_TEST);
}

void ewol::Colored3DObject::Draw(etk::Vector<vec3>& vertices,
                                 const draw::Colorf& color,
                                 mat4& transformationMatrix)
{
	if (vertices.Size()<=0) {
		return;
	}
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	ewol::openGL::Enable(ewol::openGL::FLAG_DEPTH_TEST);
	//EWOL_DEBUG("    Display " << m_coord.Size() << " elements" );
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::GetMatrix();
	mat4 camMatrix = ewol::openGL::GetCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix * transformationMatrix;
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// position :
	m_GLprogram->SendAttribute(m_GLPosition, 3/*x,y,z*/, &vertices[0], 4*sizeof(float));
	// color :
	m_GLprogram->Uniform4fv(m_GLColor, 1/*r,g,b,a*/, (float*)&color);
	// Request the draw od the elements : 
	ewol::openGL::DrawArrays(GL_TRIANGLES, 0, vertices.Size());
	m_GLprogram->UnUse();
	ewol::openGL::Disable(ewol::openGL::FLAG_DEPTH_TEST);
}

void ewol::Colored3DObject::DrawLine(etk::Vector<vec3>& vertices,
                                     const draw::Colorf& color,
                                     mat4& transformationMatrix)
{
	if (vertices.Size()<=0) {
		return;
	}
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	ewol::openGL::Enable(ewol::openGL::FLAG_DEPTH_TEST);
	//EWOL_DEBUG("    Display " << m_coord.Size() << " elements" );
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::GetMatrix();
	mat4 camMatrix = ewol::openGL::GetCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix * transformationMatrix;
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// position :
	m_GLprogram->SendAttribute(m_GLPosition, 3/*x,y,z*/, &vertices[0], 4*sizeof(float));
	// color :
	m_GLprogram->Uniform4fv(m_GLColor, 1/*r,g,b,a*/, (float*)&color);
	// Request the draw od the elements : 
	ewol::openGL::DrawArrays(GL_LINES, 0, vertices.Size());
	m_GLprogram->UnUse();
	ewol::openGL::Disable(ewol::openGL::FLAG_DEPTH_TEST);
}

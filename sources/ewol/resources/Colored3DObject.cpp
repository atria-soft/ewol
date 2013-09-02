/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/resources/Colored3DObject.h>
#include <ewol/resources/ResourceManager.h>

#undef __class__
#define __class__	"Colored3DObject"


ewol::Colored3DObject::Colored3DObject(etk::UString _genName) :
	ewol::Resource(_genName),
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


void ewol::Colored3DObject::Draw(etk::Vector<vec3>& _vertices,
                                 const etk::Color<float>& _color,
                                 bool _updateDepthBuffer,
                                 bool _depthtest)
{
	if (_vertices.Size()<=0) {
		return;
	}
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	if (true==_depthtest) {
		ewol::openGL::Enable(ewol::openGL::FLAG_DEPTH_TEST);
		if (false==_updateDepthBuffer) {
			glDepthMask(GL_FALSE);
		}
	}
	//EWOL_DEBUG("    Display " << m_coord.Size() << " elements" );
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::GetMatrix();
	mat4 camMatrix = ewol::openGL::GetCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix;
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// position :
	m_GLprogram->SendAttribute(m_GLPosition, 3/*x,y,z,unused*/, &_vertices[0], 4*sizeof(float));
	// color :
	m_GLprogram->Uniform4fv(m_GLColor, 1/*r,g,b,a*/, (float*)&_color);
	// Request the draw od the elements : 
	ewol::openGL::DrawArrays(GL_TRIANGLES, 0, _vertices.Size());
	m_GLprogram->UnUse();
	// Request the draw od the elements : 
	//glDrawArrays(GL_LINES, 0, vertices.Size());
	//m_GLprogram->UnUse();
	if (true==_depthtest) {
		if (false==_updateDepthBuffer) {
			glDepthMask(GL_TRUE);
		}
		ewol::openGL::Disable(ewol::openGL::FLAG_DEPTH_TEST);
	}
}

void ewol::Colored3DObject::Draw(etk::Vector<vec3>& _vertices,
                                 const etk::Color<float>& _color,
                                 mat4& _transformationMatrix,
                                 bool _updateDepthBuffer,
                                 bool _depthtest)
{
	if (_vertices.Size()<=0) {
		return;
	}
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	if (true==_depthtest) {
		ewol::openGL::Enable(ewol::openGL::FLAG_DEPTH_TEST);
		if (false==_updateDepthBuffer) {
			glDepthMask(GL_FALSE);
		}
	}
	//EWOL_DEBUG("    Display " << m_coord.Size() << " elements" );
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::GetMatrix();
	mat4 camMatrix = ewol::openGL::GetCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix * _transformationMatrix;
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// position :
	m_GLprogram->SendAttribute(m_GLPosition, 3/*x,y,z*/, &_vertices[0], 4*sizeof(float));
	// color :
	m_GLprogram->Uniform4fv(m_GLColor, 1/*r,g,b,a*/, (float*)&_color);
	// Request the draw od the elements : 
	ewol::openGL::DrawArrays(GL_TRIANGLES, 0, _vertices.Size());
	m_GLprogram->UnUse();
	if (true==_depthtest) {
		if (false==_updateDepthBuffer) {
			glDepthMask(GL_TRUE);
		}
		ewol::openGL::Disable(ewol::openGL::FLAG_DEPTH_TEST);
	}
}

void ewol::Colored3DObject::DrawLine(etk::Vector<vec3>& _vertices,
                                     const etk::Color<float>& _color,
                                     mat4& _transformationMatrix,
                                     bool _updateDepthBuffer,
                                     bool _depthtest)
{
	if (_vertices.Size()<=0) {
		return;
	}
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	if (true==_depthtest) {
		ewol::openGL::Enable(ewol::openGL::FLAG_DEPTH_TEST);
		if (false==_updateDepthBuffer) {
			glDepthMask(GL_FALSE);
		}
	}
	//EWOL_DEBUG("    Display " << m_coord.Size() << " elements" );
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::GetMatrix();
	mat4 camMatrix = ewol::openGL::GetCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix * _transformationMatrix;
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// position :
	m_GLprogram->SendAttribute(m_GLPosition, 3/*x,y,z*/, &_vertices[0], 4*sizeof(float));
	// color :
	m_GLprogram->Uniform4fv(m_GLColor, 1/*r,g,b,a*/, (float*)&_color);
	// Request the draw od the elements : 
	ewol::openGL::DrawArrays(GL_LINES, 0, _vertices.Size());
	m_GLprogram->UnUse();
	if (true==_depthtest) {
		if (false==_updateDepthBuffer) {
			glDepthMask(GL_TRUE);
		}
		ewol::openGL::Disable(ewol::openGL::FLAG_DEPTH_TEST);
	}
}

/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/os/FSNode.h>
#include <ewol/debug.h>
#include <ewol/compositing/Shaper.h>

#undef __class__
#define __class__	"ewol::Shaper"

ewol::Shaper::Shaper(const etk::UString& _shaperName) :
	m_name(_shaperName),
	m_config(NULL),
	m_confIdPaddingX(-1),
	m_confIdPaddingY(-1),
	m_confIdChangeTime(-1),
	m_confProgramFile(-1),
	m_GLprogram(NULL),
	m_GLPosition(-1),
	m_GLMatrix(-1),
	m_GLPropertySize(-1),
	m_GLPropertyInsidePos(-1),
	m_GLPropertyInsideSize(-1),
	m_GLStateOld(-1),
	m_GLStateNew(-1),
	m_GLStateTransition(-1),
	m_resourceTexture(NULL),
	m_nextStatusRequested(-1),
	m_propertyOrigin(0,0),
	m_propertySize(0,0),
	m_propertyInsidePosition(0,0),
	m_propertyInsideSize(0,0),
	m_stateOld(0),
	m_stateNew(0),
	m_stateTransition(1.0)
{
	LoadProgram();
	UpdateVertex();
}

ewol::Shaper::~Shaper(void)
{
	UnLoadProgram();
}

void ewol::Shaper::UnLoadProgram(void)
{
	if (NULL != m_GLprogram) {
		ewol::ResourceManager::Release(m_GLprogram);
		m_GLprogram = NULL;
	}
	if (NULL != m_resourceTexture) {
		ewol::ResourceManager::Release(m_resourceTexture);
		m_resourceTexture = NULL;
	}
	if (NULL != m_config) {
		ewol::ResourceManager::Release(m_config);
		m_config = NULL;
	}
}

void ewol::Shaper::LoadProgram(void)
{
	if (m_name=="") {
		EWOL_DEBUG("no Shaper set for loading resources ...");
		return;
	}
	if (true == ewol::ResourceManager::Keep(m_name, m_config) ) {
		m_confIdPaddingX   = m_config->Request("PaddingX");
		m_confIdPaddingY   = m_config->Request("PaddingY");
		m_confIdChangeTime = m_config->Request("ChangeTime");
		m_confProgramFile  = m_config->Request("program");
		m_confImageFile    = m_config->Request("image");
	}
	etk::UString basicShaderFile = m_config->GetString(m_confProgramFile);
	if (basicShaderFile!="") {
		// Get the relative position of the current file ...
		etk::FSNode file(m_name);
		etk::UString tmpFilename = file.GetRelativeFolder() + basicShaderFile;
		EWOL_DEBUG("Shaper try load shader : " << tmpFilename << " with base : " << basicShaderFile);
		// get the shader resource :
		m_GLPosition = 0;
		if (true == ewol::ResourceManager::Keep(tmpFilename, m_GLprogram) ) {
			m_GLPosition        = m_GLprogram->GetAttribute("EW_coord2d");
			m_GLMatrix          = m_GLprogram->GetUniform("EW_MatrixTransformation");
			// Widget property ==> for the Vertex shader
			m_GLPropertySize       = m_GLprogram->GetUniform("EW_widgetProperty.size");
			m_GLPropertyOrigin     = m_GLprogram->GetUniform("EW_widgetProperty.origin");
			m_GLPropertyInsidePos  = m_GLprogram->GetUniform("EW_widgetProperty.insidePos");
			m_GLPropertyInsideSize = m_GLprogram->GetUniform("EW_widgetProperty.insideSize");
			// status property ==> for the fragment shader
			m_GLStateOld           = m_GLprogram->GetUniform("EW_status.stateOld");
			m_GLStateNew           = m_GLprogram->GetUniform("EW_status.stateNew");
			m_GLStateTransition    = m_GLprogram->GetUniform("EW_status.transition");
			// for the texture ID : 
			m_GLtexID              = m_GLprogram->GetUniform("EW_texID");
		}
		etk::UString basicImageFile = m_config->GetString(m_confImageFile);
		if (basicImageFile != "") {
			tmpFilename = file.GetRelativeFolder() + basicImageFile;
			ivec2 size(64,64);
			if (true == ewol::ResourceManager::Keep(tmpFilename, m_resourceTexture, size) ) {
				// nothing else to do ...
			}
		}
	}
}


void ewol::Shaper::Draw(void)
{
	if (m_config == NULL) {
		// this is a normale case ... the user can choice to have no config basic file ...
		return;
	}
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	//glScalef(m_scaling.x, m_scaling.y, 1.0);
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = ewol::openGL::GetMatrix();
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// position :
	m_GLprogram->SendAttribute(m_GLPosition, 2/*x,y*/, m_coord);
	// all entry parameters :
	m_GLprogram->Uniform2(m_GLPropertySize,       m_propertySize);
	m_GLprogram->Uniform2(m_GLPropertyOrigin,     m_propertyOrigin);
	m_GLprogram->Uniform2(m_GLPropertyInsidePos,  m_propertyInsidePosition);
	m_GLprogram->Uniform2(m_GLPropertyInsideSize, m_propertyInsideSize);
	m_GLprogram->Uniform1i(m_GLStateOld,        m_stateOld);
	m_GLprogram->Uniform1i(m_GLStateNew,        m_stateNew);
	m_GLprogram->Uniform1f(m_GLStateTransition, m_stateTransition);
	
	if (NULL!=m_resourceTexture) {
		// TextureID
		m_GLprogram->SetTexture0(m_GLtexID, m_resourceTexture->GetId());
	}
	// Request the draw of the elements : 
	ewol::openGL::DrawArrays(GL_TRIANGLES, 0, 6);
	m_GLprogram->UnUse();
}

void ewol::Shaper::Clear(void)
{
	// nothing to do ...
}

bool ewol::Shaper::ChangeStatusIn(int32_t _newStatusId)
{
	if (_newStatusId != m_stateNew) {
		m_nextStatusRequested = _newStatusId;
		return true;
	}
	if(    m_nextStatusRequested != -1
	    || m_stateNew != m_stateOld) {
		return true;
	}
	return false;
}

bool ewol::Shaper::PeriodicCall(const ewol::EventTime& _event)
{
	//EWOL_DEBUG("call=" << _event);
	// start :
	if (m_stateTransition>=1.0) {
		m_stateOld = m_stateNew;
		if(    m_nextStatusRequested != -1
		    && m_nextStatusRequested != m_stateOld) {
			m_stateNew = m_nextStatusRequested;
			m_nextStatusRequested = -1;
			m_stateTransition = 0.0;
			//EWOL_DEBUG("     ##### START #####  ");
		} else {
			m_nextStatusRequested = -1;
			// disable periodic call ...
			return false;
		}
	}
	if (m_stateTransition<1.0) {
		// check if no new state requested:
		if (m_nextStatusRequested != -1 && m_stateTransition<0.5) {
			// invert sources with destination
			int32_t tmppp = m_stateOld;
			m_stateOld = m_stateNew;
			m_stateNew = tmppp;
			m_stateTransition = 1.0 - m_stateTransition;
			if (m_nextStatusRequested == m_stateNew) {
				m_nextStatusRequested = -1;
			}
		}
		float timeRelativity = m_config->GetFloat(m_confIdChangeTime)/1000.0;
		m_stateTransition += _event.GetDeltaCall()/timeRelativity;
		//m_stateTransition += _event.GetDeltaCall();
		m_stateTransition = etk_avg(0.0f, m_stateTransition, 1.0f);
		//EWOL_DEBUG("relative=" << timeRelativity << " Transition : " << m_stateTransition);
	}
	return true;
}


void ewol::Shaper::UpdateVertex(void)
{
	// set coord ==> must be a static VBO ...
	m_coord[0].setValue( m_propertyOrigin.x(),
	                     m_propertyOrigin.y()+m_propertySize.y());
	m_coord[1].setValue( m_propertyOrigin.x(),
	                     m_propertyOrigin.y());
	m_coord[2].setValue( m_propertyOrigin.x()+m_propertySize.x(),
	                     m_propertyOrigin.y());
	
	m_coord[3].setValue( m_propertyOrigin.x()+m_propertySize.x(),
	                     m_propertyOrigin.y());
	m_coord[4].setValue( m_propertyOrigin.x()+m_propertySize.x(),
	                     m_propertyOrigin.y()+m_propertySize.y());
	m_coord[5].setValue( m_propertyOrigin.x(),
	                     m_propertyOrigin.y()+m_propertySize.y());
}

void ewol::Shaper::SetOrigin(const vec2& _newOri)
{
	if (m_propertyOrigin != _newOri) {
		m_propertyOrigin = _newOri;
		UpdateVertex();
	}

}

void ewol::Shaper::SetSize(const vec2& _newSize)
{
	if (m_propertySize != _newSize) {
		m_propertySize = _newSize;
		UpdateVertex();
	}
}

void ewol::Shaper::SetInsideSize(const vec2& _newInsideSize)
{
	m_propertyInsideSize = _newInsideSize;
}

void ewol::Shaper::SetInsidePos(const vec2& _newInsidePos)
{
	m_propertyInsidePosition = _newInsidePos;
}


vec2 ewol::Shaper::GetPadding(void)
{
	vec2 padding(0,0);
	if (m_config!=NULL) {
		padding.setValue(m_config->GetFloat(m_confIdPaddingX),
		                 m_config->GetFloat(m_confIdPaddingY));
	}
	return padding;
}


void ewol::Shaper::SetSource(const etk::UString& _newFile)
{
	Clear();
	UnLoadProgram();
	m_name = _newFile;
	LoadProgram();
}

bool ewol::Shaper::HasSources(void)
{
	return m_GLprogram!=NULL;
}


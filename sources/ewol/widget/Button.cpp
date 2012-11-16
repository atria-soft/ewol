/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/widget/Button.h>

#include <ewol/oObject/OObject.h>
#include <ewol/widget/WidgetManager.h>


extern const char * const ewolEventButtonPressed    = "ewol-button-Pressed";
extern const char * const ewolEventButtonDown       = "ewol-button-down";
extern const char * const ewolEventButtonUp         = "ewol-button-up";
extern const char * const ewolEventButtonEnter      = "ewol-button-enter";
extern const char * const ewolEventButtonLeave      = "ewol-button-leave";


void ewol::WIDGET_ButtonInit(void)
{
	
}

#undef __class__
#define __class__	"Button"


void ewol::Button::Init(void)
{
	m_oObjectImage=NULL;
	AddEventId(ewolEventButtonPressed);
	AddEventId(ewolEventButtonDown);
	AddEventId(ewolEventButtonUp);
	AddEventId(ewolEventButtonEnter);
	AddEventId(ewolEventButtonLeave);
	m_hasAnImage = false;
	m_alignement = ewol::TEXT_ALIGN_CENTER;
	
	m_status.m_stateOld = 0;
	m_status.m_stateNew = 0;
	m_status.m_transition = 1.0;
	m_time = -1;
	m_nextStatusRequested = -1;
	/*
	#ifdef __TARGET_OS__Android
		m_padding.y = 12;
		m_padding.x = 12;
	#else
		m_padding.y = 4;
		m_padding.x = 4;
	#endif
	*/
	m_textColorFg = draw::color::black;
	
	SetCanHaveFocus(true);
	etk::UString tmpString("THEME:GUI:widgetButton.conf");
	if (true == ewol::resource::Keep(tmpString, m_config) ) {
		m_confIdPaddingX   = m_config->Request("PaddingX");
		m_confIdPaddingY   = m_config->Request("PaddingY");
		m_confIdChangeTime = m_config->Request("ChangeTime");
	}
	tmpString ="THEME:GUI:widgetButton.prog";
	// get the shader resource :
	m_GLPosition = 0;
	if (true == ewol::resource::Keep(tmpString, m_GLprogram) ) {
		m_GLPosition        = m_GLprogram->GetAttribute("EW_coord2d");
		m_GLMatrix          = m_GLprogram->GetUniform("EW_MatrixTransformation");
		// Widget property ==> for the Vertex shader
		m_GLwidgetProperty.m_size       = m_GLprogram->GetUniform("EW_widgetProperty.size");
		m_GLwidgetProperty.m_insidePos  = m_GLprogram->GetUniform("EW_widgetProperty.insidePos");
		m_GLwidgetProperty.m_insideSize = m_GLprogram->GetUniform("EW_widgetProperty.insideSize");
		// status property ==> for the fragment shader
		m_GLstatus.m_stateOld   = m_GLprogram->GetUniform("EW_status.stateOld");
		m_GLstatus.m_stateNew   = m_GLprogram->GetUniform("EW_status.stateNew");
		m_GLstatus.m_transition = m_GLprogram->GetUniform("EW_status.transition");
	}
}

ewol::Button::Button(void)
{
	m_label = "No Label";
	Init();
}

ewol::Button::Button(etk::UString newLabel)
{
	m_label = newLabel;
	Init();
}


ewol::Button::~Button(void)
{
	
}

void ewol::Button::SetImage(etk::UString imageName)
{
	if (imageName == "") {
		m_hasAnImage = false;
	} else {
		m_imageSelected = imageName;
		m_hasAnImage = true;
	}
	MarkToRedraw();
}

bool ewol::Button::CalculateMinSize(void)
{
	etk::Vector2D<int32_t> padding;
	padding.x = m_config->GetInteger(m_confIdPaddingX);
	padding.y = m_config->GetInteger(m_confIdPaddingY);
	
	etk::Vector3D<int32_t> minSize = m_displayText.CalculateSize(m_label);
	m_minSize.x = padding.x*2 + minSize.x;
	m_minSize.y = padding.y*2 + minSize.y;
	// Add the image element ...
	if (true == m_hasAnImage) {
		//m_minSize.x += -m_padding.x + m_padding.y*2 + minHeight;
		//m_minSize.y += m_padding.y*2;
		m_minSize.x += padding.x + minSize.y;
	}
	
	MarkToRedraw();
	return true;
}


void ewol::Button::SetLabel(etk::UString newLabel)
{
	m_label = newLabel;
	MarkToRedraw();
}

void ewol::Button::SetValue(bool val)
{
	
}

void ewol::Button::SetAlignement(textAlignement_te typeAlign)
{
	m_alignement = typeAlign;
	MarkToRedraw();
}


bool ewol::Button::GetValue(void)
{
	return false;
}


void ewol::Button::SetPoint(float x, float y)
{
	etk::Vector2D<float> triangle(x, y);
	m_coord.PushBack(triangle);
}

void ewol::Button::Rectangle(float x, float y, float w, float h)
{
	m_coord.Clear();
	/* Bitmap position
	 *      xA     xB
	 *   yC *------*
	 *      |      |
	 *      |      |
	 *   yD *------*
	 */
	float dxA = x;
	float dxB = x + w;
	float dyC = y;
	float dyD = y + h;
	SetPoint(dxA, dyD);
	SetPoint(dxA, dyC);
	SetPoint(dxB, dyC);

	SetPoint(dxB, dyC);
	SetPoint(dxB, dyD);
	SetPoint(dxA, dyD);
}


void ewol::Button::OnDraw(DrawProperty& displayProp)
{
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	//glScalef(m_scaling.x, m_scaling.y, 1.0);
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	etk::Matrix4 tmpMatrix = ewol::openGL::GetMatrix();
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// position :
	// Note : Must be all the time a [-1..1] square ...  
	// TODO : plop ...
	m_GLprogram->SendAttribute(m_GLPosition, 2/*x,y*/, &m_coord[0]);
	// all entry parameters :
	m_GLprogram->Uniform2fv(m_GLwidgetProperty.m_size,       1, &m_size.x);
	m_GLprogram->Uniform2fv(m_GLwidgetProperty.m_insidePos,  1, &m_widgetProperty.m_insidePos.x);
	m_GLprogram->Uniform2fv(m_GLwidgetProperty.m_insideSize, 1, &m_widgetProperty.m_insideSize.x);
	m_GLprogram->Uniform1i(m_GLstatus.m_stateOld,   m_status.m_stateOld);
	m_GLprogram->Uniform1i(m_GLstatus.m_stateNew,   m_status.m_stateNew);
	m_GLprogram->Uniform1f(m_GLstatus.m_transition, m_status.m_transition);
	
	// Request the draw of the elements : 
	glDrawArrays(GL_TRIANGLES, 0, m_coord.Size());
	m_GLprogram->UnUse();
	
	
	if (NULL != m_oObjectImage) {
		m_oObjectImage->Draw();
	}
	//m_oObjectText.Draw();
	m_displayText.Draw();
}

void ewol::Button::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		
		etk::Vector2D<int32_t> padding;
		padding.x = m_config->GetInteger(m_confIdPaddingX);
		padding.y = m_config->GetInteger(m_confIdPaddingY);
		
		if (NULL != m_oObjectImage) {
			m_oObjectImage->Clear();
		}
		int32_t tmpSizeX = m_minSize.x;
		int32_t tmpSizeY = m_minSize.y;
		etk::Vector3D<float> tmpOrigin((float)((m_size.x - m_minSize.x) / 2.0),
		                               (float)((m_size.y - m_minSize.y) / 2.0),
		                               (float)(0.0));
		// no change for the text orogin : 
		etk::Vector3D<float> tmpTextOrigin((float)((m_size.x - m_minSize.x) / 2.0 + padding.x),
		                                   (float)((m_size.y - m_minSize.y) / 2.0 + padding.y),
		                                   (float)(0.0));
		
		if (true==m_userFill.x) {
			tmpSizeX = m_size.x;
			tmpOrigin.x = 0.0;
			if (m_alignement == ewol::TEXT_ALIGN_LEFT) {
				tmpTextOrigin.x = padding.x;
			}
		}
		if (true==m_userFill.y) {
			tmpSizeY = m_size.y;
			tmpOrigin.y = 0.0;
		}
		tmpOrigin.x += padding.x;
		tmpOrigin.x += padding.y;
		tmpSizeX -= 2*padding.x;
		tmpSizeY -= 2*padding.y;
		
		etk::Vector2D<float> textPos(tmpTextOrigin.x, tmpTextOrigin.x);
		
		/*ewol::OObject2DTextured * tmpImage = NULL;
		if (true == m_hasAnImage) {
			int32_t fontId = GetDefaultFontId();
			int32_t fontHeight = ewol::GetHeight(fontId);
			tmpImage = new ewol::OObject2DTextured(m_imageSelected, fontHeight, fontHeight);
			tmpImage->Rectangle(textPos.x, textPos.y, fontHeight, fontHeight);
			// update the text position ...
			textPos.x += m_padding.x + fontHeight;
		}
		*/
		etk::Vector3D<float> drawClippingPos((float)padding.x, (float)padding.y, (float)-0.5);
		etk::Vector3D<float> drawClippingSize((float)(m_size.x - 2*padding.x),
		                                      (float)(m_size.y - 2*padding.y),
		                                      (float)1.0);
		
		// clean the element
		m_displayText.Clear();
		m_displayText.SetClipping(drawClippingPos, drawClippingSize);
		m_displayText.Print(m_label);
		m_displayText.Tranlate(tmpTextOrigin);
		
		
		m_widgetProperty.m_insidePos = textPos;
		etk::Vector3D<float> tmpp = m_displayText.CalculateSize(m_label);
		etk::Vector2D<float> tmpp2(tmpp.x, tmpp.y);
		m_widgetProperty.m_insideSize = tmpp2;
		
		Rectangle(0, 0, m_size.x, m_size.y);
	}
}


bool ewol::Button::OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, etk::Vector2D<float> pos)
{
	//EWOL_DEBUG("Event on BT ...");
	if(ewol::EVENT_INPUT_TYPE_ENTER == typeEvent) {
		ChangeStatusIn(2);
	}else if(ewol::EVENT_INPUT_TYPE_LEAVE == typeEvent) {
		ChangeStatusIn(0);
	}
	if (1 == IdInput) {
		if(ewol::EVENT_INPUT_TYPE_DOWN == typeEvent) {
			GenerateEventId(ewolEventButtonDown);
			ChangeStatusIn(1);
			MarkToRedraw();
		}
		if(ewol::EVENT_INPUT_TYPE_UP == typeEvent) {
			GenerateEventId(ewolEventButtonUp);
			ChangeStatusIn(0);
			MarkToRedraw();
		}
		if(    ewol::EVENT_INPUT_TYPE_SINGLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_DOUBLE == typeEvent
		    || ewol::EVENT_INPUT_TYPE_TRIPLE == typeEvent) {
			GenerateEventId(ewolEventButtonPressed);
			MarkToRedraw();
			return true;
		}
	}
	return false;
}


bool ewol::Button::OnEventKb(ewol::eventKbType_te typeEvent, uniChar_t unicodeData)
{
	//EWOL_DEBUG("BT PRESSED : \"" << UTF8_data << "\" size=" << strlen(UTF8_data));
	if(    typeEvent == ewol::EVENT_KB_TYPE_DOWN
	    && unicodeData == '\r') {
		GenerateEventId(ewolEventButtonEnter);
	}
	return false;
}



void ewol::Button::ChangeStatusIn(int32_t newStatusId)
{
	m_nextStatusRequested = newStatusId;
	PeriodicCallSet(true);
	MarkToRedraw();
}


void ewol::Button::PeriodicCall(int64_t localTime)
{
	// start :
	if (m_time == -1) {
		m_time = localTime;
		m_status.m_stateOld = m_status.m_stateNew;
		m_status.m_stateNew = m_nextStatusRequested;
		m_nextStatusRequested = -1;
		m_status.m_transition = 0.0;
		//EWOL_ERROR("     ##### START #####  ");
	}
	int64_t offset = localTime - m_time;
	float timeRelativity = m_config->GetFloat(m_confIdChangeTime)*1000.0;
	if (offset > timeRelativity) {
		// check if no new state requested:
		if (m_nextStatusRequested != -1) {
			m_time = localTime;
			m_status.m_stateOld = m_status.m_stateNew;
			m_status.m_stateNew = m_nextStatusRequested;
			m_nextStatusRequested = -1;
			m_status.m_transition = 0.0;
		} else {
			m_status.m_transition = 1.0;
			//EWOL_ERROR("     ##### STOP #####  ");
			PeriodicCallSet(false);
			m_time = -1;
		}
	} else {
		m_status.m_transition = (float)offset / timeRelativity;
		//EWOL_DEBUG("time=" << offset << " in " << timeRelativity << " Transition : " << m_status.m_transition);
	}
	MarkToRedraw();
}

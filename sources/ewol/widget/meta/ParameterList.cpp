/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/meta/ParameterList.h>
#include <etk/tool.h>

#include <ewol/widget/List.h>

#include <ewol/compositing/Drawing.h>
#include <ewol/compositing/Text.h>
#include <ewol/widget/WidgetManager.h>

#undef __class__
#define __class__	"ParameterList"

extern const char * const ewolEventParameterListSelect     = "ewol-event-parameter-list-select";


#undef __class__
#define __class__	"List"


widget::ParameterList::ParameterList(void)
{
	AddEventId(ewolEventParameterListSelect);
	
	m_idSelected = -1;
	m_paddingSizeX = 2;
	#ifdef __TARGET_OS__Android
		m_paddingSizeY = 10;
	#else
		m_paddingSizeY = 2;
	#endif
	SetCanHaveFocus(true);
}

widget::ParameterList::~ParameterList(void)
{
	//clean all the object
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		delete(m_listOObject[iii]);
		m_listOObject[iii] = NULL;
	}
	m_listOObject.Clear();
	MenuClear();
}


bool widget::ParameterList::CalculateMinSize(void)
{
	/*int32_t fontId = GetDefaultFontId();
	int32_t minWidth = ewol::GetWidth(fontId, m_label);
	int32_t minHeight = ewol::GetHeight(fontId);
	m_minSize.x = 3+minWidth;
	m_minSize.y = 3+minHeight;
	*/
	m_minSize.x = 150;
	m_minSize.y = 150;
	return true;
}


void widget::ParameterList::AddOObject(ewol::Compositing* newObject, int32_t pos)
{
	if (NULL == newObject) {
		EWOL_ERROR("Try to add an empty object in the Widget generic display system");
		return;
	}
	if (pos < 0 || pos >= m_listOObject.Size() ) {
		m_listOObject.PushBack(newObject);
	} else {
		m_listOObject.Insert(pos, newObject);
	}
}


void widget::ParameterList::ClearOObjectList(void)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		delete(m_listOObject[iii]);
		m_listOObject[iii] = NULL;
	}
	m_listOObject.Clear();
}

void widget::ParameterList::OnDraw(ewol::DrawProperty& displayProp)
{
	for (int32_t iii=0; iii<m_listOObject.Size(); iii++) {
		if (NULL != m_listOObject[iii]) {
			m_listOObject[iii]->Draw();
		}
	}
	WidgetScrooled::OnDraw(displayProp);
}




void widget::ParameterList::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		
		// clean the object list ...
		ClearOObjectList();
		//EWOL_DEBUG("OnRegenerateDisplay(" << m_size.x << "," << m_size.y << ")");
		
		int32_t tmpOriginX = 0;
		int32_t tmpOriginY = 0;
		/*
		if (true==m_userFill.x) {
			tmpOriginX = 0;
		}
		if (true==m_userFill.y) {
			tmpOriginY = 0;
		}*/
		tmpOriginX += m_paddingSizeX;
		tmpOriginY += m_paddingSizeY;
	
		/*
		int32_t fontId = GetDefaultFontId();
		//int32_t minWidth = ewol::GetWidth(fontId, m_label);
		int32_t minHeight = ewol::GetHeight(fontId);
		*/
		// TODO : Rework this ...
		int32_t minHeight=20;
	
		//uint32_t nbColomn = GetNuberOfColomn();
		int32_t nbRaw    = m_list.Size();
		// For the scrooling windows
		m_maxSize.x = m_size.x;
		m_maxSize.y = (minHeight + 2*m_paddingSizeY) * nbRaw;
		
		
		etk::Vector<int32_t> listSizeColomn;
		
		// set background color :
		ewol::Drawing * tmpDraw = new ewol::Drawing();
		tmpDraw->SetColor(0xFFFFFFFF);
		tmpDraw->SetPos(etk::Vector3D<float>(0,0,0) );
		tmpDraw->RectangleWidth(etk::Vector3D<float>(m_size.x, m_size.y) );
		
		uint32_t displayableRaw = m_size.y / (minHeight + 2*m_paddingSizeY) +2;
		
		int32_t startRaw = m_originScrooled.y / (minHeight + 2*m_paddingSizeY);
		
		if (startRaw >= nbRaw-1 ) {
			startRaw = nbRaw - 1;
		}
		if (startRaw<0) {
			startRaw = 0;
		}
		// Calculate the real position ...
		tmpOriginY = m_size.y - (-m_originScrooled.y + (startRaw+1)*(minHeight + 2*m_paddingSizeY));
		
		for(int32_t iii=startRaw; iii<nbRaw && iii<(startRaw+displayableRaw); iii++) {
			etk::UString myTextToWrite = "???";
			draw::Color fg(0x000000FF);
			if (m_list[iii] != NULL) {
				myTextToWrite = m_list[iii]->m_label;
			}
			
			ewol::Text * tmpText = new ewol::Text();
			
			etk::Vector3D<float> textPos;
			textPos.x = (int32_t)tmpOriginX;
			if (m_list[iii]->m_group == false) {
				textPos.x += minHeight;
			}
			textPos.y = (int32_t)(tmpOriginY + m_paddingSizeY);
			tmpText->SetPos(textPos);
			tmpText->Print(myTextToWrite);
			
			AddOObject(tmpText);
			tmpOriginY -= minHeight + 2* m_paddingSizeY;
		}
		AddOObject(tmpDraw, 0);
		
		// call the herited class...
		widget::WidgetScrooled::OnRegenerateDisplay();
	}
}


bool widget::ParameterList::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, etk::Vector2D<float> pos)
{
	if (true == WidgetScrooled::OnEventInput(type, IdInput, typeEvent, pos)) {
		ewol::widgetManager::FocusKeep(this);
		// nothing to do ... done on upper widet ...
		return true;
	}
	if (IdInput == 1 && typeEvent == ewol::keyEvent::statusSingle) {
		etk::Vector2D<float> relativePos = RelativePosition(pos);
		// corection for the openGl abstraction
		relativePos.y = m_size.y - relativePos.y;
		// TODO : Rework this ...
		/*
		int32_t fontId = GetDefaultFontId();
		//int32_t minWidth = ewol::GetWidth(fontId, m_label.c_str());
		int32_t minHeight = ewol::GetHeight(fontId);
		*/
		int32_t minHeight = 20;
		int32_t rawID = (relativePos.y+m_originScrooled.y) / (minHeight + 2*m_paddingSizeY);
		// generate an event on a rawId if the element request change and Select it ...
		if (rawID >=0 && rawID<m_list.Size()) {
			if (m_list[rawID]!=NULL) {
				if (m_list[rawID]->m_refId>=0) {
					GenerateEventId(ewolEventParameterListSelect, m_list[rawID]->m_refId);
					m_idSelected = rawID;
					MarkToRedraw();
					return true;
				}
			}
		}
	}
	return false;
}



void widget::ParameterList::OnGetFocus(void)
{
	EWOL_DEBUG("Ewol::List Get Focus");
}

void widget::ParameterList::OnLostFocus(void)
{
	EWOL_DEBUG("Ewol::List Lost Focus");
}

void widget::ParameterList::MenuAdd(etk::UString& label, int32_t refId, etk::UString& image)
{
	widget::elementPL* tmpEmement = new widget::elementPL(label, refId, image, false);
	if (NULL != tmpEmement) {
		m_list.PushBack(tmpEmement);
		if (m_idSelected == -1 && label != "---" && refId>0) {
			m_idSelected = m_list.Size()-1;
		}
		MarkToRedraw();
	}
}


void widget::ParameterList::MenuAddGroup(etk::UString& label)
{
	etk::UString image = "";
	widget::elementPL* tmpEmement = new widget::elementPL(label, -1, image, true);
	if (NULL != tmpEmement) {
		m_list.PushBack(tmpEmement);
		MarkToRedraw();
	}
}


void widget::ParameterList::MenuClear(void)
{
	m_idSelected = -1;
	for (int32_t iii=0; iii<m_list.Size(); iii++) {
		if (NULL != m_list[iii]) {
			delete(m_list[iii]);
			m_list[iii] = NULL;
		}
	}
	m_list.Clear();
}


void widget::ParameterList::MenuSeparator(void)
{
	if (m_list.Size()>0) {
		etk::UString label = "";
		etk::UString image = "";
		MenuAdd(label, -1, image);
	}
}


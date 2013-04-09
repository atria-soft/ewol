/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/ewol.h>
#include <ewol/widget/Composer.h>
#include <etk/os/FSNode.h>
#include <ewol/widget/WidgetManager.h>

widget::Composer::Composer(void) :
	m_subWidget(NULL)
{
	// nothing to do ...
	
}

widget::Composer::~Composer(void)
{
	Clean();
}

void widget::Composer::Clean(void)
{
	if (NULL != m_subWidget) {
		delete(m_subWidget);
		// might have been destroy first here : 
		if (m_subWidget!=NULL) {
			EWOL_ERROR("Composer : An error Occured when removing main node");
		}
	}
	// nothing else to do .. all node in the list might have been removed now ...
	if (0!=m_list.Size()) {
		EWOL_ERROR("the subName element in the list are incorect...");
	}
}


bool widget::Composer::LoadFromFile(const etk::UString& _fileName)
{
	// open the curent File
	etk::FSNode fileName(_fileName);
	if (false == fileName.Exist()) {
		EWOL_ERROR("File Does not exist : " << fileName);
		return false;
	}
	int32_t fileSize = fileName.FileSize();
	if (0==fileSize) {
		EWOL_ERROR("This file is empty : " << fileName);
		return false;
	}
	if (false == fileName.FileOpenRead()) {
		EWOL_ERROR("Can not open the file : " << fileName);
		return false;
	}
	// allocate data
	char * fileBuffer = new char[fileSize+5];
	if (NULL == fileBuffer) {
		EWOL_ERROR("Error Memory allocation size=" << fileSize);
		return false;
	}
	memset(fileBuffer, 0, (fileSize+5)*sizeof(char));
	// load data from the file :
	fileName.FileRead(fileBuffer, 1, fileSize);
	// close the file:
	fileName.FileClose();
	
	bool ret = CommonLoadXML((const char*)fileBuffer);
	
	if (NULL != fileBuffer) {
		delete[] fileBuffer;
	}
	return ret;
}

bool widget::Composer::LoadFromString(const etk::UString& composerXmlString)
{
	etk::Char tmpData = composerXmlString.c_str();
	return CommonLoadXML(tmpData);
}

bool widget::Composer::CommonLoadXML(const char* data)
{
	if (NULL==data) {
		return false;
	}
	TiXmlDocument XmlDocument;
	// load the XML from the memory
	XmlDocument.Parse(data, 0, TIXML_ENCODING_UTF8);
	
	TiXmlElement* root = XmlDocument.FirstChildElement("composer");
	if (NULL == root ) {
		EWOL_ERROR("(l ?) main node not find: \"composer\" ...");
		return false;
	}
	// remove previous elements ...
	Clean();
	ewol::Widget::LoadXML(root);
	
	for(TiXmlNode * pNode = root->FirstChild() ;
	    NULL != pNode ;
	    pNode = pNode->NextSibling() ) {
		if (pNode->Type()==TiXmlNode::TINYXML_COMMENT) {
			// nothing to do, just proceed to next step
			continue;
		}
		etk::UString widgetName = pNode->Value();
		if (ewol::widgetManager::Exist(widgetName) == false) {
			EWOL_ERROR("(l "<<pNode->Row()<<") Unknown basic node=\"" << widgetName << "\" not in : [" << ewol::widgetManager::List() << "]" );
			continue;
		}
		if (NULL != m_subWidget) {
			EWOL_ERROR("(l "<<pNode->Row()<<") " << __class__ << " Can only have one subWidget ??? node=\"" << widgetName << "\"" );
			continue;
		}
		ewol::Widget* tmpWidget = ewol::widgetManager::Create(widgetName);
		if (tmpWidget == NULL) {
			EWOL_ERROR ("(l "<<pNode->Row()<<") Can not create the widget : \"" << widgetName << "\"");
			continue;
		}
		if (false == tmpWidget->LoadXML(pNode)) {
			EWOL_ERROR ("(l "<<pNode->Row()<<") can not load widget properties : \"" << widgetName << "\"");
			return false;
		}
	}
	return true;
}


ewol::Widget* widget::Composer::GetWidgetNamed(const etk::UString& widgetName)
{
	for (int32_t iii=0; iii<m_list.Size(); iii++) {
		if (m_list[iii].widgetName == widgetName) {
			return m_list[iii].widget;
		}
	}
	return NULL;
}

void widget::Composer::OnObjectRemove(ewol::EObject* removeObject)
{
	for (int32_t iii=0; iii<m_list.Size(); iii++) {
		if (m_list[iii].widget == removeObject) {
			m_list.Erase(iii);
		}
	}
	if (m_subWidget==removeObject) {
		m_subWidget=NULL;
	}
}

void widget::Composer::OnDraw(ewol::DrawProperty& displayProp)
{
	if (NULL!=m_subWidget) {
		m_subWidget->GenDraw(displayProp);
	}
}

void widget::Composer::CalculateSize(const vec2& availlable)
{
	if (NULL!=m_subWidget) {
		m_subWidget->CalculateSize(availlable);
		// copy all sub parameters:
		m_hide = m_subWidget->IsHide();
	}
}
void widget::Composer::CalculateMinSize(void)
{
	if (NULL!=m_subWidget) {
		m_subWidget->CalculateMinSize();
		// copy all sub parameters :
		m_hide = m_subWidget->IsHide();
		m_userFill = m_subWidget->CanFill();
		
	}
}

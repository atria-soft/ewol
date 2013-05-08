/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/ewol.h>
#include <ewol/widget/Container.h>
#include <ewol/widget/WidgetManager.h>


#undef __class__
#define __class__	"Container"


widget::Container::Container(ewol::Widget* _subElement) :
	m_subWidget(_subElement)
{
	// nothing to do ...
}

widget::Container::~Container(void)
{
	SubWidgetRemove();
}

ewol::Widget* widget::Container::GetSubWidget(void)
{
	return m_subWidget;
}

void widget::Container::SetSubWidget(ewol::Widget* _newWidget)
{
	if (NULL==_newWidget) {
		return;
	}
	SubWidgetRemove();
	m_subWidget = _newWidget;
	MarkToRedraw();
	ewol::RequestUpdateSize();
}


void widget::Container::SubWidgetRemove(void)
{
	if (NULL != m_subWidget) {
		delete(m_subWidget);
		// might have been destroy first here : 
		if (m_subWidget!=NULL) {
			EWOL_ERROR("Composer : An error Occured when removing main node");
		}
		MarkToRedraw();
		ewol::RequestUpdateSize();
	}
}

ewol::Widget* widget::Container::GetWidgetNamed(const etk::UString& _widgetName)
{
	if (GetName()==_widgetName) {
		return this;
	}
	if (NULL != m_subWidget) {
		return m_subWidget->GetWidgetNamed(_widgetName);
	}
	return NULL;
}

void widget::Container::OnObjectRemove(ewol::EObject* _removeObject)
{
	if (m_subWidget==_removeObject) {
		m_subWidget=NULL;
		MarkToRedraw();
		ewol::RequestUpdateSize();
	}
}

void widget::Container::OnDraw(ewol::DrawProperty& _displayProp)
{
	if (NULL!=m_subWidget) {
		m_subWidget->GenDraw(_displayProp);
	}
}

void widget::Container::CalculateSize(const vec2& _availlable)
{
	if (NULL!=m_subWidget) {
		m_subWidget->SetOrigin(m_origin);
		m_subWidget->CalculateSize(_availlable);
	}
	ewol::Widget::CalculateSize(_availlable);
}

void widget::Container::CalculateMinMaxSize(void)
{
	// call main class
	ewol::Widget::CalculateMinMaxSize();
	// call sub classes
	if (NULL!=m_subWidget) {
		m_subWidget->CalculateMinMaxSize();
		vec2 min = m_subWidget->GetCalculateMinSize();
		m_minSize.setMax(min);
	}
}

void widget::Container::OnRegenerateDisplay(void)
{
	if (NULL!=m_subWidget) {
		m_subWidget->OnRegenerateDisplay();
	}
}

ewol::Widget* widget::Container::GetWidgetAtPos(const vec2& _pos)
{
	if (false==IsHide()) {
		if (NULL!=m_subWidget) {
			return m_subWidget->GetWidgetAtPos(_pos);
		}
	}
	return NULL;
};


bool widget::Container::LoadXML(TiXmlNode* _node)
{
	if (NULL==_node) {
		return false;
	}
	// parse generic properties :
	ewol::Widget::LoadXML(_node);
	// remove previous element :
	SubWidgetRemove();
	
	// parse all the elements :
	for(TiXmlNode * pNode = _node->FirstChild() ;
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
		if (NULL != GetSubWidget()) {
			EWOL_ERROR("(l "<<pNode->Row()<<") " << __class__ << " Can only have one subWidget ??? node=\"" << widgetName << "\"" );
			continue;
		}
		EWOL_DEBUG("try to create subwidget : '" << widgetName << "'");
		ewol::Widget* tmpWidget = ewol::widgetManager::Create(widgetName);
		if (tmpWidget == NULL) {
			EWOL_ERROR ("(l "<<pNode->Row()<<") Can not create the widget : \"" << widgetName << "\"");
			continue;
		}
		// add widget :
		SetSubWidget(tmpWidget);
		if (false == tmpWidget->LoadXML(pNode)) {
			EWOL_ERROR ("(l "<<pNode->Row()<<") can not load widget properties : \"" << widgetName << "\"");
			return false;
		}
	}
	return true;
}

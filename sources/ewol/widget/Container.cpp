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
	if (m_subWidget!=NULL) {
		m_subWidget->SetUpperWidget(this);
	}
	MarkToRedraw();
	ewol::RequestUpdateSize();
}


void widget::Container::SubWidgetRemove(void)
{
	if (NULL != m_subWidget) {
		m_subWidget->RemoveUpperWidget();
		delete(m_subWidget);
		// might have been destroy first here : 
		if (m_subWidget!=NULL) {
			EWOL_ERROR("Composer : An error Occured when removing main node");
		}
		MarkToRedraw();
		ewol::RequestUpdateSize();
	}
}

void widget::Container::SubWidgetRemoveDelayed(void)
{
	if (NULL != m_subWidget) {
		m_subWidget->RemoveUpperWidget();
		m_subWidget->RemoveObject();
		m_subWidget=NULL;
		MarkToRedraw();
		ewol::RequestUpdateSize();
	}
}

ewol::Widget* widget::Container::GetWidgetNamed(const etk::UString& _widgetName)
{
	ewol::Widget* tmpUpperWidget = ewol::Widget::GetWidgetNamed(_widgetName);
	if (NULL!=tmpUpperWidget) {
		return tmpUpperWidget;
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

void widget::Container::SystemDraw(const ewol::DrawProperty& _displayProp)
{
	if (true==m_hide){
		// widget is hidden ...
		return;
	}
	ewol::Widget::SystemDraw(_displayProp);
	if (NULL!=m_subWidget) {
		ewol::DrawProperty prop = _displayProp;
		prop.Limit(m_origin, m_size);
		m_subWidget->SystemDraw(prop);
	}
}

void widget::Container::CalculateSize(const vec2& _availlable)
{
	if (NULL!=m_subWidget) {
		vec2 origin = m_origin+m_offset;
		vec2 minSize = m_subWidget->GetCalculateMinSize();
		bvec2 expand = m_subWidget->GetExpand();
		if (    expand.x() == false
		    || minSize.x()>_availlable.x()) {
			if (m_gravity == ewol::gravityCenter) {
				origin -= vec2((minSize.x() - _availlable.x())/2.0f, 0);
			} else if (((int32_t)m_gravity & (int32_t)ewol::gravityRight) != 0) {
				origin -= vec2((minSize.x() - _availlable.x()), 0);
			}
		}
		if(    expand.y() == false
		    || minSize.y()>_availlable.y()) {
			if (m_gravity == ewol::gravityCenter) {
				origin -= vec2(0, (minSize.y() - _availlable.y())/2.0f);
			} else if (((int32_t)m_gravity & (int32_t)ewol::gravityTop) != 0) {
				origin -= vec2(0, (minSize.y() - _availlable.y()));
			}
		}
		m_subWidget->SetOrigin(origin);
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
	//EWOL_ERROR("[" << GetId() << "] Result min size : " <<  m_minSize);
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


bool widget::Container::LoadXML(exml::Element* _node)
{
	if (NULL==_node) {
		return false;
	}
	// parse generic properties :
	ewol::Widget::LoadXML(_node);
	// remove previous element :
	SubWidgetRemoveDelayed();
	
	// parse all the elements :
	for(int32_t iii=0; iii< _node->Size(); iii++) {
		exml::Node* pNode = _node->Get(iii);
		if (pNode==NULL) {
			continue;
		}
		if (!pNode->IsElement()) {
			// nothing to do, just proceed to next step
			continue;
		}
		etk::UString widgetName = pNode->GetValue();
		if (ewol::widgetManager::Exist(widgetName) == false) {
			EWOL_ERROR("(l "<<pNode->Pos()<<") Unknown basic node=\"" << widgetName << "\" not in : [" << ewol::widgetManager::List() << "]" );
			continue;
		}
		if (NULL != GetSubWidget()) {
			EWOL_ERROR("(l "<<pNode->Pos()<<") " << __class__ << " Can only have one subWidget ??? node=\"" << widgetName << "\"" );
			continue;
		}
		EWOL_DEBUG("try to create subwidget : '" << widgetName << "'");
		ewol::Widget* tmpWidget = ewol::widgetManager::Create(widgetName);
		if (tmpWidget == NULL) {
			EWOL_ERROR ("(l "<<pNode->Pos()<<") Can not create the widget : \"" << widgetName << "\"");
			continue;
		}
		// add widget :
		SetSubWidget(tmpWidget);
		if (false == tmpWidget->LoadXML((exml::Element*)pNode)) {
			EWOL_ERROR ("(l "<<pNode->Pos()<<") can not load widget properties : \"" << widgetName << "\"");
			return false;
		}
	}
	return true;
}

void widget::Container::SetOffset(const vec2& _newVal)
{
	if (m_offset != _newVal) {
		ewol::Widget::SetOffset(_newVal);
		// recalculate the new sise and position of sub widget ...
		CalculateSize(m_size);
	}
}



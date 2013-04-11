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


widget::Container::Container(ewol::Widget* subElement) :
	m_subWidget(subElement)
{
	// nothing to do ...
}

widget::Container::~Container(void)
{
	RemoveSubWidget();
}

ewol::Widget* widget::Container::GetSubWidget(void)
{
	return m_subWidget;
}

void widget::Container::SetSubWidget(ewol::Widget* newWidget)
{
	if (NULL==newWidget) {
		return;
	}
	RemoveSubWidget();
	m_subWidget = newWidget;
	MarkToRedraw();
	ewol::RequestUpdateSize();
}


void widget::Container::RemoveSubWidget(void)
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

ewol::Widget* widget::Container::GetWidgetNamed(const etk::UString& widgetName)
{
	if (GetName()==widgetName) {
		return this;
	}
	if (NULL != m_subWidget) {
		return m_subWidget->GetWidgetNamed(widgetName);
	}
	return NULL;
}

void widget::Container::OnObjectRemove(ewol::EObject* removeObject)
{
	if (m_subWidget==removeObject) {
		m_subWidget=NULL;
		MarkToRedraw();
		ewol::RequestUpdateSize();
	}
}

void widget::Container::OnDraw(ewol::DrawProperty& displayProp)
{
	if (NULL!=m_subWidget) {
		m_subWidget->GenDraw(displayProp);
	}
}

void widget::Container::CalculateSize(const vec2& availlable)
{
	if (NULL!=m_subWidget) {
		m_subWidget->SetOrigin(m_origin);
		m_subWidget->CalculateSize(availlable);
	}
	ewol::Widget::CalculateSize(availlable);
}

void widget::Container::CalculateMinMaxSize(void)
{
	// callmain class
	ewol::Widget::CalculateMinMaxSize();
	// call sub classes
	if (NULL!=m_subWidget) {
		m_subWidget->CalculateMinMaxSize();
		vec2 min = m_subWidget->GetCalculateMinSize();
		if (m_minSize.x()<min.x()) {
			m_minSize.setX(min.x());
		}
		if (m_minSize.y()<min.y()) {
			m_minSize.setY(min.y());
		}
	}
}

void widget::Container::OnRegenerateDisplay(void)
{
	if (NULL!=m_subWidget) {
		m_subWidget->OnRegenerateDisplay();
	}
}

ewol::Widget* widget::Container::GetWidgetAtPos(const vec2& pos)
{
	if (false==IsHide()) {
		if (NULL!=m_subWidget) {
			return m_subWidget->GetWidgetAtPos(pos);
		}
	}
	return NULL;
};

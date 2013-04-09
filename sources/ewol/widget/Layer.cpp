/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Layer.h>
#include <ewol/widget/WidgetManager.h>


#undef __class__
#define __class__	"Layer"

static ewol::Widget* Create(void)
{
	return new widget::Layer();
}

void widget::Layer::Init(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,&Create);
}

void widget::Layer::UnInit(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,NULL);
}


widget::Layer::Layer(void)
{
	// set contamination enable
	LockExpendContamination();
}

widget::Layer::~Layer(void)
{
	EWOL_DEBUG("[" << GetId() << "] Layer : destroy");
	SubWidgetRemoveAll();
}


void widget::Layer::CalculateSize(const vec2& availlable)
{
	//EWOL_DEBUG("Update Size");
	m_size = availlable;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->SetOrigin(m_origin);
			m_subWidget[iii]->CalculateSize(m_size);
		}
	}
	MarkToRedraw();
}


void widget::Layer::CalculateMinSize(void)
{
	m_userExpend.setValue(false, false);
	m_minSize.setValue(0,0);
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->CalculateMinSize();
			if (true == m_subWidget[iii]->CanExpand().x()) {
				m_userExpend.setX(true);
			}
			if (true == m_subWidget[iii]->CanExpand().y()) {
				m_userExpend.setY(true);
			}
			vec2 tmpSize = m_subWidget[iii]->GetMinSize();
			m_minSize.setValue( etk_max(tmpSize.x(), m_minSize.x()),
			                    etk_max(tmpSize.y(), m_minSize.y()) );
		}
	}
}

void widget::Layer::SetMinSize(const vec2& size)
{
	EWOL_ERROR("Layer can not have a user Minimum size (herited from under elements)");
}


bvec2 widget::Layer::CanExpand(void)
{
	if (true == m_lockExpendContamination) {
		return bvec2(false,false);
	}
	return m_userExpend;
}

void widget::Layer::LockExpendContamination(bool lockExpend)
{
	m_lockExpendContamination = lockExpend;
}

//etk::Vector<ewol::Widget*> m_SubWidget;

void widget::Layer::SubWidgetRemoveAll(void)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		delete(m_subWidget[iii]);
		m_subWidget[iii] = NULL;
	}
	m_subWidget.Clear();
}


void widget::Layer::SubWidgetAdd(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget.PushBack(newWidget);
}

void widget::Layer::SubWidgetAddFront(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget.PushFront(newWidget);
}

void widget::Layer::SubWidgetRemove(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (newWidget == m_subWidget[iii]) {
			delete(m_subWidget[iii]);
			m_subWidget[iii] = NULL;
			m_subWidget.Erase(iii);
			return;
		}
	}
}

void widget::Layer::SubWidgetUnLink(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (newWidget == m_subWidget[iii]) {
			m_subWidget[iii] = NULL;
			m_subWidget.Erase(iii);
			return;
		}
	}
}


void widget::Layer::OnDraw(ewol::DrawProperty& displayProp)
{
	// draw is done in the invert sense of inserting ... the first element inserted is on the top and the last is on the buttom
	for (int32_t iii=m_subWidget.Size()-1; iii>=0; iii--) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->GenDraw(displayProp);
		}
	}
}



void widget::Layer::OnRegenerateDisplay(void)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->OnRegenerateDisplay();
		}
	}
}


ewol::Widget * widget::Layer::GetWidgetAtPos(vec2 pos)
{
	// for all element in the sizer ...
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->GetSize();
			vec2 tmpOrigin = m_subWidget[iii]->GetOrigin();
			if(    (tmpOrigin.x() <= pos.x() && tmpOrigin.x() + tmpSize.x() >= pos.x())
			    && (tmpOrigin.y() <= pos.y() && tmpOrigin.y() + tmpSize.y() >= pos.y()) )
			{
				ewol::Widget * tmpWidget = m_subWidget[iii]->GetWidgetAtPos(pos);
				if (NULL != tmpWidget) {
					return tmpWidget;
				}
			}
		}
	}
	// otherwise the event go to this widget ...
	return this;
}


void widget::Layer::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	ewol::Widget::OnObjectRemove(removeObject);
	// second step find if in all the elements ...
	for(int32_t iii=m_subWidget.Size()-1; iii>=0; iii--) {
		if(m_subWidget[iii] == removeObject) {
			EWOL_DEBUG("Remove layer sub Element [" << iii << "] ==> destroyed object");
			m_subWidget[iii] = NULL;
			m_subWidget.Erase(iii);
		}
	}
}



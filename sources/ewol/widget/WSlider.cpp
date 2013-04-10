/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/WSlider.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/ewol.h>


#undef __class__
#define __class__	"WSlider"


widget::WSlider::WSlider(void)
{
	// set contamination enable
	LockExpandContamination(bvec2(false,false));
	m_windowsDestination = 0;
	m_slidingProgress = 0;
	m_windowsSources = 0;
	m_underExpand.setValue(false,false);
}

widget::WSlider::~WSlider(void)
{
	SubWidgetRemoveAll();
}


void widget::WSlider::CalculateSize(const vec2& availlable)
{
	//EWOL_DEBUG("Update Size");
	m_size = availlable;
	
	if (m_windowsDestination == m_windowsSources) {
		int32_t iii = m_windowsDestination;
		if (iii < m_subWidget.Size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->SetOrigin(m_origin);
				m_subWidget[iii]->CalculateSize(m_size);
			}
		}
	} else {
		int32_t iii = m_windowsSources;
		if (iii < m_subWidget.Size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->SetOrigin(vec2(m_origin.x() - (m_size.x()*(float)m_slidingProgress/1000.0),  m_origin.y()));
				m_subWidget[iii]->CalculateSize(m_size);
			}
		}
		iii = m_windowsDestination;
		if (iii < m_subWidget.Size()) {
			if (NULL != m_subWidget[iii]) {
				m_subWidget[iii]->SetOrigin(vec2(m_origin.x() - (m_size.x()*((float)m_slidingProgress/1000.0) - m_size.x()),  m_origin.y()));
				m_subWidget[iii]->CalculateSize(m_size);
			}
		}
	}
	MarkToRedraw();
}


void widget::WSlider::CalculateMinMaxSize(void)
{
	EWOL_DEBUG("Calculate MinSize");
	m_underExpand.setValue(false,false);
	m_minSize.setValue(0,0);
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->CalculateMinMaxSize();
			if (true == m_subWidget[iii]->CanExpand().x()) {
				m_underExpand.setX(true);
			}
			if (true == m_subWidget[iii]->CanExpand().y()) {
				m_underExpand.setY(true);
			}
			vec2 tmpSize = m_subWidget[iii]->GetCalculateMinSize();
			m_minSize.setValue(etk_max(tmpSize.x(), m_minSize.x()),
			                   etk_max(tmpSize.y(), m_minSize.y()));
		}
	}
}

void widget::WSlider::SetMinSize(const vec2& size)
{
	EWOL_ERROR("Layer can not have a user Minimum size (herited from under elements)");
}

bvec2 widget::WSlider::CanExpand(void)
{
	bvec2 res = m_userExpand;
	if (true == m_underExpand.x()) {
		res.setX(true);
	}
	if (true == m_underExpand.y()) {
		res.setY(true);
	}
	if (true == m_lockExpandContamination.x()) {
		res.setX(false);
	}
	if (true == m_lockExpandContamination.y()) {
		res.setY(false);
	}
	return res;
}

void widget::WSlider::LockExpandContamination(const bvec2& lockExpand)
{
	m_lockExpandContamination = lockExpand;
}

//etk::Vector<ewol::Widget*> m_SubWidget;

void widget::WSlider::SubWidgetRemoveAll(void)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		delete(m_subWidget[iii]);
		m_subWidget[iii] = NULL;
	}
	m_subWidget.Clear();
}


void widget::WSlider::SubWidgetAdd(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget.PushBack(newWidget);
	MarkToRedraw();
	ewol::RequestUpdateSize();
}


void widget::WSlider::SubWidgetRemove(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (newWidget == m_subWidget[iii]) {
			delete(m_subWidget[iii]);
			m_subWidget[iii] = NULL;
			m_subWidget.Erase(iii);
			MarkToRedraw();
			ewol::RequestUpdateSize();
			return;
		}
	}
}

void widget::WSlider::SubWidgetUnLink(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (newWidget == m_subWidget[iii]) {
			m_subWidget[iii] = NULL;
			m_subWidget.Erase(iii);
			ewol::RequestUpdateSize();
			MarkToRedraw();
			return;
		}
	}
}

void widget::WSlider::SubWidgetSelectSet(int32_t id)
{
	if (id<0 || id > m_subWidget.Size()) {
		EWOL_ERROR("Can not change to a widget not present");
	}
	m_windowsDestination = id;
	m_slidingProgress = 0;
	PeriodicCallSet(true);
	MarkToRedraw();
}


void widget::WSlider::PeriodicCall(int64_t localTime)
{
	if (m_slidingProgress >= 1000) {
		// end of periodic :
		PeriodicCallSet(false);
		m_windowsSources = m_windowsDestination;
	} else {
		m_slidingProgress += 30;
		m_slidingProgress = etk_avg(0, m_slidingProgress, 1000);
	}
	CalculateSize(m_size);
	MarkToRedraw();
}


void widget::WSlider::OnDraw(ewol::DrawProperty& displayProp)
{
	if (m_windowsDestination == m_windowsSources) {
		//EWOL_DEBUG("Draw : " << m_windowsDestination);
		int32_t iii = m_windowsDestination;
		if (iii<0 || iii > m_subWidget.Size()) {
			return;
		}
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->GenDraw(displayProp);
		}
	} else {
		//EWOL_DEBUG("Draw : " << m_windowsSources << "=>" << m_windowsDestination << "progress=" << ((float)m_slidingProgress/1000.) );
		// draw Sources :
		int32_t iii = m_windowsSources;
		if (iii<0 || iii > m_subWidget.Size()) {
			return;
		}
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->GenDraw(displayProp);
		}
		// Draw Destination : 
		iii = m_windowsDestination;
		if (iii<0 || iii > m_subWidget.Size()) {
			return;
		}
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->GenDraw(displayProp);
		}
	}
}


void widget::WSlider::OnRegenerateDisplay(void)
{
	if (m_windowsDestination == m_windowsSources) {
		int32_t iii = m_windowsDestination;
		if (iii<0 || iii > m_subWidget.Size()) {
			return;
		}
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->OnRegenerateDisplay();
		}
	} else {
		int32_t iii = m_windowsSources;
		if (iii<0 || iii > m_subWidget.Size()) {
			return;
		}
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->OnRegenerateDisplay();
		}
		iii = m_windowsDestination;
		if (iii<0 || iii > m_subWidget.Size()) {
			return;
		}
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->OnRegenerateDisplay();
		}
	}
}


ewol::Widget * widget::WSlider::GetWidgetAtPos(const vec2& pos)
{
	// TODO : Review this ...
	if (m_windowsDestination<0 || m_windowsDestination > m_subWidget.Size()) {
		// error ...
		return NULL;
	}
	int32_t iii = m_windowsDestination;
	
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
	// otherwise the event go to this widget ...
	return this;
}


void widget::WSlider::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	ewol::Widget::OnObjectRemove(removeObject);
	// second step find if in all the elements ...
	for(int32_t iii=m_subWidget.Size()-1; iii>=0; iii--) {
		if(m_subWidget[iii] == removeObject) {
			EWOL_DEBUG("Remove sizer sub Element [" << iii << "] ==> destroyed object");
			m_subWidget[iii] = NULL;
			m_subWidget.Erase(iii);
		}
	}
}



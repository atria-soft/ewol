/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/SizerHori.h>
#include <ewol/widget/WidgetManager.h>

#undef __class__
#define __class__	"SizerHori"

widget::SizerHori::SizerHori(void)
{
	// set contamination enable
	LockExpendContamination();
}

widget::SizerHori::~SizerHori(void)
{
	SubWidgetRemoveAll();
}


bool widget::SizerHori::CalculateSize(float availlableX, float availlableY)
{
	//EWOL_DEBUG("Update Size");
	m_size.x = availlableX;
	m_size.y = availlableY;
	// calculate unExpendable Size :
	float unexpendableSize=0.0;
	int32_t nbWidgetFixedSize=0;
	int32_t nbWidgetNotFixedSize=0;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->GetMinSize();
			unexpendableSize += tmpSize.x;
			if (false == m_subWidget[iii]->CanExpentX()) {
				nbWidgetFixedSize++;
			} else {
				nbWidgetNotFixedSize++;
			}
		}
	}
	float sizeToAddAtEveryOne = 0;
	// 2 cases : 1 or more can Expend, or all is done ...
	if (0 != nbWidgetNotFixedSize) {
		sizeToAddAtEveryOne = (m_size.x - unexpendableSize) / nbWidgetNotFixedSize;
		if (sizeToAddAtEveryOne<0.0) {
			sizeToAddAtEveryOne=0;
		}
	}
	vec2 tmpOrigin;
	tmpOrigin.x = m_origin.x;
	tmpOrigin.y = m_origin.y;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->GetMinSize();
			// Set the origin :
			//EWOL_DEBUG("Set ORIGIN : " << tmpOrigin.x << "," << tmpOrigin.y << ")");
			m_subWidget[iii]->SetOrigin(tmpOrigin.x, tmpOrigin.y);
			// Now Update his Size  his size in X and the curent sizer size in Y:
			if (true == m_subWidget[iii]->CanExpentX()) {
				m_subWidget[iii]->CalculateSize(tmpSize.x+sizeToAddAtEveryOne, m_size.y);
				tmpOrigin.x += tmpSize.x+sizeToAddAtEveryOne;
			} else {
				m_subWidget[iii]->CalculateSize(tmpSize.x, m_size.y);
				tmpOrigin.x += tmpSize.x;
			}
		}
	}
	MarkToRedraw();
	return true;
}


bool widget::SizerHori::CalculateMinSize(void)
{
	//EWOL_DEBUG("Update minimum Size");
	m_userExpend.x=false;
	m_userExpend.y=false;
	m_minSize.x = 0.0;
	m_minSize.y = 0.0;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->CalculateMinSize();
			if (true == m_subWidget[iii]->CanExpentX()) {
				m_userExpend.x = true;
			}
			if (true == m_subWidget[iii]->CanExpentY()) {
				m_userExpend.y = true;
			}
			vec2 tmpSize = m_subWidget[iii]->GetMinSize();
			m_minSize.x += tmpSize.x;
			if (tmpSize.y>m_minSize.y) {
				m_minSize.y = tmpSize.y;
			}
		}
	}
	//EWOL_DEBUG("Result : expend  X="<< m_userExpend.x << " Y=" << m_userExpend.y);
	//EWOL_DEBUG("         minSize ("<< m_minSize.x << "," << m_minSize.y << ")");
	return true;
}

void widget::SizerHori::SetMinSize(float x, float y)
{
	EWOL_ERROR("Sizer can not have a user Minimum size (herited from under elements)");
}

void widget::SizerHori::SetExpendX(bool newExpend)
{
	EWOL_ERROR("Sizer can not have a user expend settings X (herited from under elements)");
}

bool widget::SizerHori::CanExpentX(void)
{
	if (true == m_lockExpendContamination) {
		return false;
	}
	return m_userExpend.x;
}

void widget::SizerHori::SetExpendY(bool newExpend)
{
	EWOL_ERROR("Sizer can not have a user expend settings Y (herited from under elements)");
}

bool widget::SizerHori::CanExpentY(void)
{
	if (true == m_lockExpendContamination) {
		return false;
	}
	return m_userExpend.y;
}

void widget::SizerHori::LockExpendContamination(bool lockExpend)
{
	m_lockExpendContamination = lockExpend;
}

//etk::Vector<ewol::Widget*> m_subWidget[m_currentCreateId];

void widget::SizerHori::SubWidgetRemoveAll(void)
{
	// the size automaticly decrement with the auto call of the OnObjectRemove function
	while (m_subWidget.Size() > 0 ) {
		if (NULL != m_subWidget[0]) {
			delete(m_subWidget[0]);
			// no remove, this element is removed with the function OnObjectRemove ==> it does not exist anymore ...
		}
	}
	m_subWidget.Clear();
}


void widget::SizerHori::SubWidgetAdd(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget.PushBack(newWidget);
}


void widget::SizerHori::SubWidgetRemove(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (newWidget == m_subWidget[iii]) {
			if (NULL != m_subWidget[iii]) {
				delete(m_subWidget[iii]);
				m_subWidget[iii] = NULL;
			}
			m_subWidget.Erase(iii);
			return;
		}
	}
}

void widget::SizerHori::SubWidgetUnLink(ewol::Widget* newWidget)
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


void widget::SizerHori::OnDraw(ewol::DrawProperty& displayProp)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->GenDraw(displayProp);
		}
	}
}


void widget::SizerHori::OnRegenerateDisplay(void)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->OnRegenerateDisplay();
		}
	}
}


ewol::Widget * widget::SizerHori::GetWidgetAtPos(vec2 pos)
{
	if (true == IsHide()) {
		return NULL;
	}
	// for all element in the sizer ...
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->GetSize();
			vec2 tmpOrigin = m_subWidget[iii]->GetOrigin();
			if(    (tmpOrigin.x <= pos.x && tmpOrigin.x + tmpSize.x >= pos.x)
			    && (tmpOrigin.y <= pos.y && tmpOrigin.y + tmpSize.y >= pos.y) )
			{
				ewol::Widget * tmpWidget = m_subWidget[iii]->GetWidgetAtPos(pos);
				if (NULL != tmpWidget) {
					return tmpWidget;
				}
				// stop searching
				break;
			}
		}
	}
	// TODO : Check if we have a mover, otherwire return NULL;
	return NULL;
	//return this;
}


void widget::SizerHori::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	ewol::Widget::OnObjectRemove(removeObject);
	// second step find if in all the elements ...
	for(int32_t iii=m_subWidget.Size()-1; iii>=0; iii--) {
		if(m_subWidget[iii] == removeObject) {
			EWOL_VERBOSE("Remove sizer sub Element [" << iii << "/" << m_subWidget.Size()-1 << "] ==> destroyed object");
			m_subWidget[iii] = NULL;
			m_subWidget.Erase(iii);
		}
	}
}






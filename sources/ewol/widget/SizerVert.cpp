/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/SizerVert.h>
#include <ewol/widget/WidgetManager.h>


#undef __class__
#define __class__	"SizerVert"


widget::SizerVert::SizerVert(void)
{
	// set contamination enable
	LockExpendContamination();
}

widget::SizerVert::~SizerVert(void)
{
	SubWidgetRemoveAll();
}


bool widget::SizerVert::CalculateSize(float availlableX, float availlableY)
{
	//EWOL_DEBUG("Update Size");
	m_size.setValue(availlableX, availlableY);
	// calculate unExpendable Size :
	float unexpendableSize=0.0;
	int32_t nbWidgetFixedSize=0;
	int32_t nbWidgetNotFixedSize=0;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->GetMinSize();
			unexpendableSize += tmpSize.y();
			if (false == m_subWidget[iii]->CanExpentY()) {
				nbWidgetFixedSize++;
			} else {
				nbWidgetNotFixedSize++;
			}
		}
	}
	// 2 cases : 1 or more can Expend, or all is done ...
	float sizeToAddAtEveryOne = 0;
	// 2 cases : 1 or more can Expend, or all is done ...
	if (0 != nbWidgetNotFixedSize) {
		sizeToAddAtEveryOne = (m_size.y() - unexpendableSize) / nbWidgetNotFixedSize;
		if (sizeToAddAtEveryOne<0.0) {
			sizeToAddAtEveryOne=0;
		}
	}
	vec2 tmpOrigin = m_origin;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->GetMinSize();
			// Set the origin :
			//EWOL_DEBUG("Set ORIGIN : " << tmpOrigin.x << "," << tmpOrigin.y << ")");
			m_subWidget[iii]->SetOrigin(tmpOrigin.x(), tmpOrigin.y());
			// Now Update his Size  his size in X and the curent sizer size in Y:
			if (true == m_subWidget[iii]->CanExpentY()) {
				m_subWidget[iii]->CalculateSize(m_size.x(), tmpSize.y()+sizeToAddAtEveryOne);
				tmpOrigin.setY(tmpOrigin.y() + tmpSize.y()+sizeToAddAtEveryOne);
			} else {
				m_subWidget[iii]->CalculateSize(m_size.x(), tmpSize.y());
				tmpOrigin.setY(tmpOrigin.y() + tmpSize.y());
			}
		}
	}
	MarkToRedraw();
	return true;
}


bool widget::SizerVert::CalculateMinSize(void)
{
	//EWOL_DEBUG("Update minimum Size");
	m_userExpend.setValue(false, false);
	m_minSize.setValue(0,0);
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->CalculateMinSize();
			if (true == m_subWidget[iii]->CanExpentX()) {
				m_userExpend.setX(true);
			}
			if (true == m_subWidget[iii]->CanExpentY()) {
				m_userExpend.setY(true);
			}
			vec2 tmpSize = m_subWidget[iii]->GetMinSize();
			//EWOL_DEBUG("VERT : NewMinSize=" << tmpSize);
			//EWOL_DEBUG("             Get minSize[" << iii << "] ("<< tmpSize.x << "," << tmpSize.y << ")");
			m_minSize.setY(m_minSize.y() + tmpSize.y());
			if (tmpSize.x()>m_minSize.x()) {
				m_minSize.setX(tmpSize.x());
			}
		}
	}
	//EWOL_DEBUG("Vert Result : expend="<< m_userExpend << "  minSize="<< m_minSize);
	return true;
}

void widget::SizerVert::SetMinSize(float x, float y)
{
	EWOL_ERROR("Sizer can not have a user Minimum size (herited from under elements)");
}

void widget::SizerVert::SetExpendX(bool newExpend)
{
	EWOL_ERROR("Sizer can not have a user expend settings X (herited from under elements)");
}

bool widget::SizerVert::CanExpentX(void)
{
	if (true == m_lockExpendContamination) {
		return false;
	}
	return m_userExpend.x();
}

void widget::SizerVert::SetExpendY(bool newExpend)
{
	EWOL_ERROR("Sizer can not have a user expend settings Y (herited from under elements)");
}

bool widget::SizerVert::CanExpentY(void)
{
	if (true == m_lockExpendContamination) {
		return false;
	}
	return m_userExpend.y();
}

void widget::SizerVert::LockExpendContamination(bool lockExpend)
{
	m_lockExpendContamination = lockExpend;
}

//etk::Vector<ewol::Widget*> m_SubWidget;

void widget::SizerVert::SubWidgetRemoveAll(void)
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


void widget::SizerVert::SubWidgetAdd(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget.PushBack(newWidget);
}


void widget::SizerVert::SubWidgetRemove(ewol::Widget* newWidget)
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

void widget::SizerVert::SubWidgetUnLink(ewol::Widget* newWidget)
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


void widget::SizerVert::OnDraw(ewol::DrawProperty& displayProp)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->GenDraw(displayProp);
		}
	}
}



void widget::SizerVert::OnRegenerateDisplay(void)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->OnRegenerateDisplay();
		}
	}
}


ewol::Widget * widget::SizerVert::GetWidgetAtPos(vec2 pos)
{
	if (true == IsHide()) {
		return NULL;
	}
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
				// stop searching
				break;
			}
		}
	}
	// TODO : Check if we have a mover, otherwire return NULL;
	return NULL;
	//return this;
}


void widget::SizerVert::OnObjectRemove(ewol::EObject * removeObject)
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



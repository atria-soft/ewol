/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/WidgetManager.h>


#undef __class__
#define __class__	"Sizer"


widget::Sizer::Sizer(widget::Sizer::displayMode_te mode):
	m_mode(mode),
	m_lockExpendContamination(false,false),
	m_borderSize(0,0)
{
	
}

widget::Sizer::~Sizer(void)
{
	EWOL_DEBUG("[" << GetId() << "]={" << GetObjectType() << "}  Sizer : destroy (mode=" << (m_mode==widget::Sizer::modeVert?"Vert":"Hori") << ")");
	SubWidgetRemoveAll();
}


void widget::Sizer::SetBorderSize(const ivec2& newBorderSize)
{
	m_borderSize = newBorderSize;
	if (m_borderSize.x() < 0) {
		EWOL_ERROR("Try to set a border size <0 on x : " << m_borderSize.x() << " ==> restore to 0");
		m_borderSize.setX(0);
	}
	if (m_borderSize.y() < 0) {
		EWOL_ERROR("Try to set a border size <0 on y : " << m_borderSize.y() << " ==> restore to 0");
		m_borderSize.setY(0);
	}
	MarkToRedraw();
	ewol::RequestUpdateSize();
}

void widget::Sizer::SetMode(widget::Sizer::displayMode_te mode)
{
	m_mode = mode;
	MarkToRedraw();
	ewol::RequestUpdateSize();
}

widget::Sizer::displayMode_te widget::Sizer::GetMode(void)
{
	return m_mode;
}

bool widget::Sizer::CalculateSize(float availlableX, float availlableY)
{
	//EWOL_DEBUG("Update Size");
	m_size.setValue(availlableX, availlableY);
	m_size -= m_borderSize*2;
	// calculate unExpendable Size :
	float unexpendableSize=0.0;
	int32_t nbWidgetFixedSize=0;
	int32_t nbWidgetNotFixedSize=0;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->GetMinSize();
			if (m_mode==widget::Sizer::modeVert) {
				unexpendableSize += tmpSize.y();
				if (false == m_subWidget[iii]->CanExpentY()) {
					nbWidgetFixedSize++;
				} else {
					nbWidgetNotFixedSize++;
				}
			} else {
				unexpendableSize += tmpSize.x();
				if (false == m_subWidget[iii]->CanExpentX()) {
					nbWidgetFixedSize++;
				} else {
					nbWidgetNotFixedSize++;
				}
			}
		}
	}
	// 2 cases : 1 or more can Expend, or all is done ...
	float sizeToAddAtEveryOne = 0;
	// 2 cases : 1 or more can Expend, or all is done ...
	if (0 != nbWidgetNotFixedSize) {
		if (m_mode==widget::Sizer::modeVert) {
			sizeToAddAtEveryOne = (m_size.y() - unexpendableSize) / nbWidgetNotFixedSize;
		} else {
			sizeToAddAtEveryOne = (m_size.x() - unexpendableSize) / nbWidgetNotFixedSize;
		}
		if (sizeToAddAtEveryOne<0.0) {
			sizeToAddAtEveryOne=0;
		}
	}
	vec2 tmpOrigin = m_origin + m_borderSize;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->GetMinSize();
			// Set the origin :
			//EWOL_DEBUG("Set ORIGIN : " << tmpOrigin.x << "," << tmpOrigin.y << ")");
			m_subWidget[iii]->SetOrigin(tmpOrigin.x(), tmpOrigin.y());
			// Now Update his Size  his size in X and the curent sizer size in Y:
			if (m_mode==widget::Sizer::modeVert) {
				if (true == m_subWidget[iii]->CanExpentY()) {
					m_subWidget[iii]->CalculateSize(m_size.x(), tmpSize.y()+sizeToAddAtEveryOne);
					tmpOrigin.setY(tmpOrigin.y() + tmpSize.y()+sizeToAddAtEveryOne);
				} else {
					m_subWidget[iii]->CalculateSize(m_size.x(), tmpSize.y());
					tmpOrigin.setY(tmpOrigin.y() + tmpSize.y());
				}
			} else {
				if (true == m_subWidget[iii]->CanExpentX()) {
					m_subWidget[iii]->CalculateSize(tmpSize.x()+sizeToAddAtEveryOne, m_size.y());
					tmpOrigin.setX(tmpOrigin.x() + tmpSize.x()+sizeToAddAtEveryOne);
				} else {
					m_subWidget[iii]->CalculateSize(tmpSize.x(), m_size.y());
					tmpOrigin.setX(tmpOrigin.x() + tmpSize.x());
				}
			}
		}
	}
	m_size += m_borderSize*2;
	MarkToRedraw();
	return true;
}


bool widget::Sizer::CalculateMinSize(void)
{
	//EWOL_DEBUG("Update minimum Size");
	m_userExpend.setValue(false, false);
	m_minSize.setValue(0,0);
	if (m_userMinSize.x()>0) {
		m_minSize.setX(m_userMinSize.x());
	}
	if (m_userMinSize.y()>0) {
		m_minSize.setY(m_userMinSize.y());
	}
	m_minSize += m_borderSize*2;
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
			//EWOL_DEBUG("             Get minSize[" << iii << "] "<< tmpSize);
			if (m_mode==widget::Sizer::modeVert) {
				m_minSize.setY(m_minSize.y() + tmpSize.y());
				if (tmpSize.x()>m_minSize.x()) {
					m_minSize.setX(tmpSize.x());
				}
			} else {
				m_minSize.setX(m_minSize.x() + tmpSize.x());
				if (tmpSize.y()>m_minSize.y()) {
					m_minSize.setY(tmpSize.y());
				}
			}
		}
	}
	//EWOL_DEBUG("Vert Result : expend="<< m_userExpend << "  minSize="<< m_minSize);
	return true;
}

void widget::Sizer::SetMinSize(float x, float y)
{
	EWOL_ERROR("[" << GetId() << "] Sizer can not have a user Minimum size (herited from under elements)");
}

void widget::Sizer::SetExpendX(bool newExpend)
{
	EWOL_ERROR("[" << GetId() << "] Sizer can not have a user expend settings X (herited from under elements)");
}

bool widget::Sizer::CanExpentX(void)
{
	if (true == m_lockExpendContamination.x()) {
		return false;
	}
	return m_userExpend.x();
}

void widget::Sizer::SetExpendY(bool newExpend)
{
	EWOL_ERROR("[" << GetId() << "] Sizer can not have a user expend settings Y (herited from under elements)");
}

bool widget::Sizer::CanExpentY(void)
{
	if (true == m_lockExpendContamination.y()) {
		return false;
	}
	return m_userExpend.y();
}

void widget::Sizer::LockExpendContamination(bool lockExpend)
{
	m_lockExpendContamination.setValue(lockExpend,lockExpend);
}
void widget::Sizer::LockExpendContaminationVert(bool lockExpend)
{
	m_lockExpendContamination.setY(lockExpend);
}
void widget::Sizer::LockExpendContaminationHori(bool lockExpend)
{
	m_lockExpendContamination.setX(lockExpend);
}

void widget::Sizer::SubWidgetRemoveAll(void)
{
	int32_t errorControl = m_subWidget.Size();
	// the size automaticly decrement with the auto call of the OnObjectRemove function
	while (m_subWidget.Size() > 0 ) {
		if (NULL != m_subWidget[0]) {
			delete(m_subWidget[0]);
			// no remove, this element is removed with the function OnObjectRemove ==> it does not exist anymore ...
			if (errorControl == m_subWidget.Size()) {
				EWOL_CRITICAL("[" << GetId() << "] The number of element might have been reduced ... ==> it is not the case ==> the herited class must call the \"OnObjectRemove\" function...");
				m_subWidget[0] = NULL;
			}
		} else {
			EWOL_WARNING("[" << GetId() << "] Must not have null pointer on the subWidget list ...");
			m_subWidget.Erase(0);
		}
		errorControl = m_subWidget.Size();
	}
	m_subWidget.Clear();
}


void widget::Sizer::SubWidgetAdd(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget.PushBack(newWidget);
}

void widget::Sizer::SubWidgetAddStart(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	m_subWidget.PushFront(newWidget);
}


void widget::Sizer::SubWidgetRemove(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	int32_t errorControl = m_subWidget.Size();
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (newWidget == m_subWidget[iii]) {
			delete(m_subWidget[iii]);
			// no remove, this element is removed with the function OnObjectRemove ==> it does not exist anymore ...
			if (errorControl == m_subWidget.Size()) {
				EWOL_CRITICAL("[" << GetId() << "] The number of element might have been reduced ... ==> it is not the case ==> the herited class must call the \"OnObjectRemove\" function...");
				m_subWidget[iii] = NULL;
				m_subWidget.Erase(iii);
			}
			return;
		}
	}
}

void widget::Sizer::SubWidgetUnLink(ewol::Widget* newWidget)
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


void widget::Sizer::OnDraw(ewol::DrawProperty& displayProp)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->GenDraw(displayProp);
		}
	}
}



void widget::Sizer::OnRegenerateDisplay(void)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->OnRegenerateDisplay();
		}
	}
}


ewol::Widget * widget::Sizer::GetWidgetAtPos(vec2 pos)
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


void widget::Sizer::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	ewol::Widget::OnObjectRemove(removeObject);
	// second step find if in all the elements ...
	for(int32_t iii=m_subWidget.Size()-1; iii>=0; iii--) {
		if(m_subWidget[iii] == removeObject) {
			EWOL_VERBOSE("[" << GetId() << "]={" << GetObjectType() << "} Remove sizer sub Element [" << iii << "/" << m_subWidget.Size()-1 << "] ==> destroyed object");
			m_subWidget[iii] = NULL;
			m_subWidget.Erase(iii);
		}
	}
}



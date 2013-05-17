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

static ewol::Widget* Create(void)
{
	return new widget::Sizer();
}

void widget::Sizer::Init(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,&Create);
}

void widget::Sizer::UnInit(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,NULL);
}


widget::Sizer::Sizer(widget::Sizer::displayMode_te _mode):
	m_mode(_mode),
	m_borderSize()
{
	
}

widget::Sizer::~Sizer(void)
{
	//EWOL_DEBUG("[" << GetId() << "]={" << GetObjectType() << "}  Sizer : destroy (mode=" << (m_mode==widget::Sizer::modeVert?"Vert":"Hori") << ")");
}


void widget::Sizer::SetBorderSize(const ewol::Dimension& _newBorderSize)
{
	m_borderSize = _newBorderSize;
	MarkToRedraw();
	ewol::RequestUpdateSize();
}

void widget::Sizer::SetMode(widget::Sizer::displayMode_te _mode)
{
	m_mode = _mode;
	MarkToRedraw();
	ewol::RequestUpdateSize();
}

widget::Sizer::displayMode_te widget::Sizer::GetMode(void)
{
	return m_mode;
}

void widget::Sizer::CalculateSize(const vec2& _availlable)
{
	ewol::Widget::CalculateSize(_availlable);
	vec2 tmpBorderSize = m_borderSize.GetPixel();
	//EWOL_DEBUG("[" << GetId() << "] Update Size : " << _availlable << " nbElement : " << m_subWidget.Size() << " borderSize=" << tmpBorderSize << " from border=" << m_borderSize);
	m_size -= tmpBorderSize*2;
	// calculate unExpandable Size :
	float unexpandableSize=0.0;
	int32_t nbWidgetFixedSize=0;
	int32_t nbWidgetNotFixedSize=0;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->GetCalculateMinSize();
			if (m_mode==widget::Sizer::modeVert) {
				unexpandableSize += tmpSize.y();
				if (false == m_subWidget[iii]->CanExpand().y()) {
					nbWidgetFixedSize++;
				} else {
					nbWidgetNotFixedSize++;
				}
			} else {
				unexpandableSize += tmpSize.x();
				if (false == m_subWidget[iii]->CanExpand().x()) {
					nbWidgetFixedSize++;
				} else {
					nbWidgetNotFixedSize++;
				}
			}
		}
	}
	// 2 cases : 1 or more can Expand, or all is done ...
	float sizeToAddAtEveryOne = 0;
	// 2 cases : 1 or more can Expand, or all is done ...
	if (0 != nbWidgetNotFixedSize) {
		if (m_mode==widget::Sizer::modeVert) {
			sizeToAddAtEveryOne = (m_size.y() - unexpandableSize) / nbWidgetNotFixedSize;
		} else {
			sizeToAddAtEveryOne = (m_size.x() - unexpandableSize) / nbWidgetNotFixedSize;
		}
		if (sizeToAddAtEveryOne<0.0) {
			sizeToAddAtEveryOne=0;
		}
	}
	vec2 tmpOrigin = m_origin + tmpBorderSize;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->GetCalculateMinSize();
			// Set the origin :
			//EWOL_DEBUG("[" << GetId() << "] Set iii=" << iii << " ORIGIN : " << tmpOrigin << " & offset=" << m_offset);
			m_subWidget[iii]->SetOrigin(vec2ClipInt32(tmpOrigin+m_offset));
			// Now Update his Size  his size in X and the curent sizer size in Y:
			if (m_mode==widget::Sizer::modeVert) {
				if (true == m_subWidget[iii]->CanExpand().y()) {
					m_subWidget[iii]->CalculateSize(vec2ClipInt32(vec2(m_size.x(), tmpSize.y()+sizeToAddAtEveryOne)));
					tmpOrigin.setY(tmpOrigin.y() + tmpSize.y()+sizeToAddAtEveryOne);
				} else {
					m_subWidget[iii]->CalculateSize(vec2ClipInt32(vec2(m_size.x(), tmpSize.y())));
					tmpOrigin.setY(tmpOrigin.y() + tmpSize.y());
				}
			} else {
				if (true == m_subWidget[iii]->CanExpand().x()) {
					m_subWidget[iii]->CalculateSize(vec2ClipInt32(vec2(tmpSize.x()+sizeToAddAtEveryOne, m_size.y())));
					tmpOrigin.setX(tmpOrigin.x() + tmpSize.x()+sizeToAddAtEveryOne);
				} else {
					m_subWidget[iii]->CalculateSize(vec2ClipInt32(vec2(tmpSize.x(), m_size.y())));
					tmpOrigin.setX(tmpOrigin.x() + tmpSize.x());
				}
			}
		}
	}
	m_size += tmpBorderSize*2;
	MarkToRedraw();
}


void widget::Sizer::CalculateMinMaxSize(void)
{
	//EWOL_DEBUG("[" << GetId() << "] Update minimum Size");
	m_subExpend.setValue(false, false);
	m_minSize = m_userMinSize.GetPixel();
	vec2 tmpBorderSize = m_borderSize.GetPixel();
	//EWOL_ERROR("[" << GetId() << "] {" << GetObjectType() << "} Set min size : " <<  m_minSize);
	m_minSize += tmpBorderSize*2;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->CalculateMinMaxSize();
			if (true == m_subWidget[iii]->CanExpand().x()) {
				m_subExpend.setX(true);
			}
			if (true == m_subWidget[iii]->CanExpand().y()) {
				m_subExpend.setY(true);
			}
			vec2 tmpSize = m_subWidget[iii]->GetCalculateMinSize();
			//EWOL_DEBUG("[" << GetId() << "] NewMinSize=" << tmpSize);
			//EWOL_DEBUG("[" << GetId() << "] {" << GetObjectType() << "}     Get minSize[" << iii << "] "<< tmpSize);
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
	//EWOL_ERROR("[" << GetId() << "] {" << GetObjectType() << "} Result min size : " <<  m_minSize);
}

bool widget::Sizer::LoadXML(TiXmlNode* _node)
{
	if (NULL==_node) {
		return false;
	}
	// parse generic properties :
	widget::ContainerN::LoadXML(_node);
	
	const char* tmpAttributeValue = _node->ToElement()->Attribute("border");
	if (NULL != tmpAttributeValue) {
		m_borderSize = tmpAttributeValue;
	}
	tmpAttributeValue = _node->ToElement()->Attribute("mode");
	if (NULL != tmpAttributeValue) {
		etk::UString val(tmpAttributeValue);
		if(    val.CompareNoCase("vert")
		    || val.CompareNoCase("vertical")) {
			m_mode = widget::Sizer::modeVert;
		} else {
			m_mode = widget::Sizer::modeHori;
		}
	}
	return true;
}



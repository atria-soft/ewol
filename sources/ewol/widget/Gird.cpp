/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/ewol.h>
#include <ewol/widget/Gird.h>
#include <ewol/widget/WidgetManager.h>


#undef __class__
#define __class__ "Gird"


static ewol::Widget* create(void) {
	return new widget::Gird();
}

void widget::Gird::init(ewol::WidgetManager& _widgetManager) {
	_widgetManager.addWidgetCreator(__class__,&create);
}


widget::Gird::Gird(int32_t _colNumber) :
  m_sizeRow(0),
  m_tmpWidget(NULL),
  m_gavityButtom(true),
  m_borderSize(0,0) {
	addObjectType("widget::Gird");
	setColNumber(_colNumber);
	requestUpdateSize();
}

widget::Gird::~Gird(void) {
	EWOL_DEBUG("[" << getId() << "]={" << getObjectType() << "} Gird : destroy");
	subWidgetRemoveAll();
}

void widget::Gird::setBorderSize(const ivec2& _newBorderSize) {
	m_borderSize = _newBorderSize;
	if (m_borderSize.x() < 0) {
		EWOL_ERROR("Try to set a border size <0 on x : " << m_borderSize.x() << "  == > restore to 0");
		m_borderSize.setX(0);
	}
	if (m_borderSize.y() < 0) {
		EWOL_ERROR("Try to set a border size <0 on y : " << m_borderSize.y() << "  == > restore to 0");
		m_borderSize.setY(0);
	}
	markToRedraw();
	requestUpdateSize();
}

void widget::Gird::calculateSize(const vec2& _availlable) {
	//EWOL_DEBUG("Update size");
	m_size = _availlable;
	m_size -= m_borderSize*2;
	
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (NULL != m_subWidget[iii].widget) {
			//calculate the origin :
			vec2 tmpOrigin = m_origin + m_borderSize;
			if (false == m_gavityButtom) {
				tmpOrigin += vec2(0, m_size.y()-m_borderSize.y());
			}
			
			int32_t tmpSizeWidth = 0;
			for (int32_t jjj=0; jjj<m_subWidget[iii].col; jjj++ ){
				tmpSizeWidth += abs(m_sizeCol[jjj]);
			}
			// adding Y origin : 
			int32_t addingPos = 0;
			if (true == m_gavityButtom) {
				addingPos = (m_subWidget[iii].row)*m_uniformSizeRow;
			} else {
				addingPos = -(m_subWidget[iii].row+1)*m_uniformSizeRow;
			}
			tmpOrigin += vec2(tmpSizeWidth, addingPos);
			
			EWOL_DEBUG("     [" << iii << "] set subwidget origin=" <<tmpOrigin << " size=" << ivec2(abs(m_sizeCol[m_subWidget[iii].col]), m_uniformSizeRow) );
			// set the origin :
			m_subWidget[iii].widget->setOrigin(vec2ClipInt32(tmpOrigin));
			// all time set oll the space .
			m_subWidget[iii].widget->calculateSize(vec2ClipInt32(vec2(abs(m_sizeCol[m_subWidget[iii].col]), m_uniformSizeRow)));
		}
	}
	m_size += m_borderSize*2;
	EWOL_DEBUG("Calculate size : " << m_size);
	markToRedraw();
}

void widget::Gird::calculateMinMaxSize(void) {
	for (size_t iii=0; iii<m_sizeCol.size(); iii++ ){
		if (m_sizeCol[iii] <= 0) {
			m_sizeCol[iii] = 0;
		}
	}
	//EWOL_DEBUG("Update minimum size");
	m_minSize = m_userMinSize.getPixel();
	m_maxSize = m_userMaxSize.getPixel();
	m_uniformSizeRow = 0;
	m_minSize += m_borderSize*2;
	int32_t lastLineID = 0;
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (m_subWidget[iii].row > lastLineID) {
			// change of line : 
			lastLineID = m_subWidget[iii].row;
		}
		if (NULL != m_subWidget[iii].widget) {
			m_subWidget[iii].widget->calculateMinMaxSize();
			vec2 tmpSize = m_subWidget[iii].widget->getCalculateMinSize();
			EWOL_DEBUG("     [" << iii << "] subWidgetMinSize=" << tmpSize);
			// for all we get the max size :
			m_uniformSizeRow = etk_max(tmpSize.y(), m_uniformSizeRow);
			// for the colomn size : We set the autamatic value in negative : 
			if (m_sizeCol[m_subWidget[iii].col] <= 0) {
				m_sizeCol[m_subWidget[iii].col] = etk_min(m_sizeCol[m_subWidget[iii].col], -tmpSize.x() );
			}
		}
	}
	
	if (m_sizeRow > 0) {
		m_uniformSizeRow = m_sizeRow;
	}
	int32_t tmpSizeWidth = 0;
	for (size_t iii=0; iii<m_sizeCol.size(); iii++ ){
		tmpSizeWidth += abs(m_sizeCol[iii]);
	}
	EWOL_DEBUG("     tmpSizeWidth=" << tmpSizeWidth);
	EWOL_DEBUG("     m_uniformSizeRow=" << m_uniformSizeRow);
	m_minSize += ivec2(tmpSizeWidth, (lastLineID+1)*m_uniformSizeRow);
	
	EWOL_DEBUG("Calculate min size : " << m_minSize);
	
	//EWOL_DEBUG("Vert Result : expand="<< m_userExpand << "  minSize="<< m_minSize);
}

void widget::Gird::setColNumber(int32_t _colNumber) {
	if ((int64_t)m_sizeCol.size() > _colNumber) {
		size_t errorControl = m_subWidget.size();
		// remove subWidget :
		for (int64_t iii=m_subWidget.size(); iii >= 0; iii--) {
			if (m_subWidget[iii].col>(_colNumber-1)) {
				// out of bounds : must remove it ...
				if (m_subWidget[iii].widget != NULL) {
					delete (m_subWidget[iii].widget);
					// no remove, this element is removed with the function onObjectRemove  == > it does not exist anymore ...
					if (errorControl == m_subWidget.size()) {
						EWOL_CRITICAL("[" << getId() << "] The number of element might have been reduced ...  == > it is not the case ==> the herited class must call the \"OnObjectRemove\" function...");
						m_subWidget[iii].widget = NULL;
					}
				} else {
					EWOL_WARNING("[" << getId() << "] Must not have null pointer on the subWidget list ...");
					m_subWidget.erase(m_subWidget.begin()+iii);
				}
				errorControl = m_subWidget.size();
			}
		}
		// just add the col size:
		m_sizeCol.erase(m_sizeCol.end());
	} else {
		// just add the col size:
		for (int32_t iii=m_sizeCol.size()-1; iii<_colNumber-1 ; iii++) {
			m_sizeCol.push_back(0);
		}
	}
}

void widget::Gird::setColSize(int32_t _colId, int32_t _size) {
	if ((int64_t)m_sizeCol.size() > _colId) {
		m_sizeCol[_colId] = _size;
	} else {
		EWOL_ERROR("Can not set the Colomn size : " << _colId+1
		           << " at " << _size << "px  we have "
		           << m_sizeCol.size() << " colomn");
	}
}

void widget::Gird::setRowSize(int32_t _size) {
	m_sizeRow = _size;
}

int32_t widget::Gird::getColSize(int32_t _colId) {
	if ((int64_t)m_sizeCol.size() > _colId) {
		if (m_sizeCol[_colId] <= 0) {
			return 0;
		}
		return m_sizeCol[_colId];
	}
	EWOL_ERROR("Can not get the Colomn size : " << _colId+1 << "  we have "<< m_sizeCol.size() << " colomn");
	return 0;
}

int32_t widget::Gird::getRowSize(void) {
	return m_sizeRow;
}

void widget::Gird::subWidgetRemoveAll(void) {
	size_t errorControl = m_subWidget.size();
	// the size automaticly decrement with the auto call of the onObjectRemove function
	while (m_subWidget.size() > 0 ) {
		if (NULL != m_subWidget[0].widget) {
			delete(m_subWidget[0].widget);
			// no remove, this element is removed with the function onObjectRemove  == > it does not exist anymore ...
			if (errorControl == m_subWidget.size()) {
				EWOL_CRITICAL("[" << getId() << "] The number of element might have been reduced ...  == > it is not the case ==> the herited class must call the \"OnObjectRemove\" function...");
				m_subWidget[0].widget = NULL;
			}
		} else {
			EWOL_WARNING("[" << getId() << "] Must not have null pointer on the subWidget list ...");
			m_subWidget.erase(m_subWidget.begin());
		}
		errorControl = m_subWidget.size();
	}
	m_subWidget.clear();
}


void widget::Gird::subWidgetAdd(int32_t _colId, int32_t _rowId, ewol::Widget* _newWidget) {
	if (NULL == _newWidget) {
		return;
	}
	GirdProperties prop;
	prop.row = _rowId;
	prop.col = _colId;
	prop.widget = _newWidget;
	
	// need to find the correct position : 
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (m_subWidget[iii].row < prop.row) {
			continue;
		} else if (m_subWidget[iii].row > prop.row) {
			// find a new position;
			m_subWidget.insert(m_subWidget.begin()+iii, prop);
			return;
		} else {
			if (m_subWidget[iii].col < prop.col) {
				continue;
			} else if (m_subWidget[iii].col > prop.col) {
				// find a new position;
				m_subWidget.insert(m_subWidget.begin()+iii, prop);
				return;
			} else {
				// The element already exist  == > replace it ...
				m_tmpWidget = m_subWidget[iii].widget;
				m_subWidget[iii].widget = _newWidget;
				if (NULL != m_tmpWidget) {
					delete(m_tmpWidget);
					if (m_tmpWidget != NULL) {
						EWOL_CRITICAL("[" << getId() << "] Error while replacing a widget ...  == > never call when free");
						m_tmpWidget = NULL;
					}
				}
			}
		}
	}
	// not find  == > just adding it ...
	m_subWidget.push_back(prop);
}

void widget::Gird::subWidgetRemove(ewol::Widget* _newWidget)
{
	if (NULL == _newWidget) {
		return;
	}
	size_t errorControl = m_subWidget.size();
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (_newWidget == m_subWidget[iii].widget) {
			delete(m_subWidget[iii].widget);
			// no remove, this element is removed with the function onObjectRemove  == > it does not exist anymore ...
			if (errorControl == m_subWidget.size()) {
				EWOL_CRITICAL("[" << getId() << "] The number of element might have been reduced ...  == > it is not the case ==> the herited class must call the \"OnObjectRemove\" function...");
				m_subWidget[iii].widget = NULL;
				m_subWidget.erase(m_subWidget.begin()+iii);
			}
			return;
		}
	}
	EWOL_WARNING("[" << getId() << "] Can not remove unExistant widget");
}

void widget::Gird::subWidgetRemove(int32_t _colId, int32_t _rowId) {
	if (_colId<0 || _rowId<0) {
		EWOL_WARNING("[" << getId() << "] try to remove widget with id < 0 col=" << _colId << " row=" << _rowId);
		return;
	}
	size_t errorControl = m_subWidget.size();
	// try to find it ...
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if(    m_subWidget[iii].row == _rowId
		    && m_subWidget[iii].col == _colId) {
			if (m_subWidget[iii].widget == NULL) {
				EWOL_WARNING("[" << getId() << "] remove NULL widget");
				m_subWidget.erase(m_subWidget.begin()+iii);
			} else {
				// The element already exist  == > replace it ...
				if (m_subWidget[iii].widget != NULL) {
					delete(m_subWidget[iii].widget);
					if (errorControl == m_subWidget.size()) {
						EWOL_CRITICAL("[" << getId() << "] The number of element might have been reduced ...  == > it is not the case ==> the herited class must call the \"OnObjectRemove\" function...");
						m_subWidget[iii].widget = NULL;
						m_subWidget.erase(m_subWidget.begin()+iii);
					}
				}
			}
			return;
		}
	}
	EWOL_WARNING("[" << getId() << "] Can not remove unExistant widget");
}

void widget::Gird::subWidgetUnLink(ewol::Widget* _newWidget) {
	if (NULL == _newWidget) {
		return;
	}
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (_newWidget == m_subWidget[iii].widget) {
			m_subWidget[iii].widget = NULL;
			m_subWidget.erase(m_subWidget.begin()+iii);
			return;
		}
	}
}

void widget::Gird::subWidgetUnLink(int32_t _colId, int32_t _rowId) {
	if (_colId<0 || _rowId<0) {
		EWOL_WARNING("[" << getId() << "] try to Unlink widget with id < 0 col=" << _colId << " row=" << _rowId);
		return;
	}
	// try to find it ...
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if(    m_subWidget[iii].row == _rowId
		    && m_subWidget[iii].col == _colId) {
			m_subWidget[iii].widget = NULL;
			m_subWidget.erase(m_subWidget.begin()+iii);
			return;
		}
	}
	EWOL_WARNING("[" << getId() << "] Can not unLink unExistant widget");
}

void widget::Gird::systemDraw(const ewol::DrawProperty& _displayProp) {
	ewol::Widget::systemDraw(_displayProp);
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (NULL != m_subWidget[iii].widget) {
			m_subWidget[iii].widget->systemDraw(_displayProp);
		}
	}
}

void widget::Gird::onRegenerateDisplay(void) {
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (NULL != m_subWidget[iii].widget) {
			m_subWidget[iii].widget->onRegenerateDisplay();
		}
	}
}

ewol::Widget * widget::Gird::getWidgetAtPos(const vec2& _pos) {
	if (true == isHide()) {
		return NULL;
	}
	// for all element in the sizer ...
	for (size_t iii=0; iii<m_subWidget.size(); iii++) {
		if (NULL != m_subWidget[iii].widget) {
			vec2 tmpSize = m_subWidget[iii].widget->getSize();
			vec2 tmpOrigin = m_subWidget[iii].widget->getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				ewol::Widget * tmpWidget = m_subWidget[iii].widget->getWidgetAtPos(_pos);
				if (NULL != tmpWidget) {
					return tmpWidget;
				}
				// stop searching
				break;
			}
		}
	}
	return NULL;
}

void widget::Gird::onObjectRemove(ewol::EObject * _removeObject) {
	// First step call parrent : 
	ewol::Widget::onObjectRemove(_removeObject);
	// second step find if in all the elements ...
	for(int32_t iii=m_subWidget.size()-1; iii >= 0; iii--) {
		if(m_subWidget[iii].widget == _removeObject) {
			EWOL_VERBOSE("[" << getId() << "]={" << getObjectType() << "} remove sizer sub Element [" << iii << "/" << m_subWidget.size()-1 << "]  == > destroyed object");
			m_subWidget[iii].widget = NULL;
			m_subWidget.erase(m_subWidget.begin()+iii);
		}
	}
	if (m_tmpWidget == _removeObject) {
		m_tmpWidget = NULL;
	}
}

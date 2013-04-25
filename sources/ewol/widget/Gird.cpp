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
#define __class__	"Gird"


static ewol::Widget* Create(void)
{
	return new widget::Gird();
}

void widget::Gird::Init(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,&Create);
}

void widget::Gird::UnInit(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,NULL);
}


widget::Gird::Gird(int32_t colNumber) :
	m_sizeRow(0),
	m_tmpWidget(NULL),
	m_gavityButtom(true),
	m_borderSize(0,0)
{
	SetColNumber(colNumber);
	ewol::RequestUpdateSize();
}

widget::Gird::~Gird(void)
{
	EWOL_DEBUG("[" << GetId() << "]={" << GetObjectType() << "} Gird : destroy");
	SubWidgetRemoveAll();
}

void widget::Gird::SetBorderSize(const ivec2& newBorderSize)
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

void widget::Gird::CalculateSize(const vec2& availlable)
{
	//EWOL_DEBUG("Update Size");
	m_size = availlable;
	m_size -= m_borderSize*2;
	
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
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
			// Set the origin :
			m_subWidget[iii].widget->SetOrigin(vec2ClipInt32(tmpOrigin));
			// all time set oll the space .
			m_subWidget[iii].widget->CalculateSize(vec2ClipInt32(vec2(abs(m_sizeCol[m_subWidget[iii].col]), m_uniformSizeRow)));
		}
	}
	m_size += m_borderSize*2;
	EWOL_DEBUG("Calculate size : " << m_size);
	MarkToRedraw();
}

void widget::Gird::CalculateMinMaxSize(void)
{
	for (int32_t iii=0; iii<m_sizeCol.Size(); iii++ ){
		if (m_sizeCol[iii] <= 0) {
			m_sizeCol[iii] = 0;
		}
	}
	//EWOL_DEBUG("Update minimum Size");
	m_minSize = m_userMinSize.GetPixel();
	m_maxSize = m_userMaxSize.GetPixel();
	m_uniformSizeRow = 0;
	m_minSize += m_borderSize*2;
	int32_t lastLineID = 0;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (m_subWidget[iii].row > lastLineID) {
			// change of line : 
			lastLineID = m_subWidget[iii].row;
		}
		if (NULL != m_subWidget[iii].widget) {
			m_subWidget[iii].widget->CalculateMinMaxSize();
			vec2 tmpSize = m_subWidget[iii].widget->GetCalculateMinSize();
			EWOL_DEBUG("     [" << iii << "] subWidgetMinSize=" << tmpSize);
			// for all we get the max Size :
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
	for (int32_t iii=0; iii<m_sizeCol.Size(); iii++ ){
		tmpSizeWidth += abs(m_sizeCol[iii]);
	}
	EWOL_DEBUG("     tmpSizeWidth=" << tmpSizeWidth);
	EWOL_DEBUG("     m_uniformSizeRow=" << m_uniformSizeRow);
	m_minSize += ivec2(tmpSizeWidth, (lastLineID+1)*m_uniformSizeRow);
	
	EWOL_DEBUG("Calculate min size : " << m_minSize);
	
	//EWOL_DEBUG("Vert Result : expand="<< m_userExpand << "  minSize="<< m_minSize);
}

void widget::Gird::SetColNumber(int32_t colNumber)
{
	if (m_sizeCol.Size() > colNumber) {
		int32_t errorControl = m_subWidget.Size();
		// remove subWidget :
		for (int32_t iii=m_subWidget.Size(); iii>=0; iii--) {
			if (m_subWidget[iii].col>(colNumber-1)) {
				// out of bounds : must remove it ...
				if (m_subWidget[iii].widget != NULL) {
					delete (m_subWidget[iii].widget);
					// no remove, this element is removed with the function OnObjectRemove ==> it does not exist anymore ...
					if (errorControl == m_subWidget.Size()) {
						EWOL_CRITICAL("[" << GetId() << "] The number of element might have been reduced ... ==> it is not the case ==> the herited class must call the \"OnObjectRemove\" function...");
						m_subWidget[iii].widget = NULL;
					}
				} else {
					EWOL_WARNING("[" << GetId() << "] Must not have null pointer on the subWidget list ...");
					m_subWidget.Erase(iii);
				}
				errorControl = m_subWidget.Size();
			}
		}
		// just add the col size:
		m_sizeCol.Erase(m_sizeCol.Size()-1, 0x7FFFFFFF);
	} else {
		// just add the col size:
		for (int32_t iii=m_sizeCol.Size()-1; iii<colNumber-1 ; iii++) {
			m_sizeCol.PushBack(0);
		}
	}
}

void widget::Gird::SetColSize(int32_t colId, int32_t size)
{
	if (m_sizeCol.Size() > colId) {
		m_sizeCol[colId] = size;
	} else {
		EWOL_ERROR("Can not set the Colomn size : " << colId+1 << " at " << size << "px  we have "<< m_sizeCol.Size() << " colomn");
	}
}

void widget::Gird::SetRowSize(int32_t size)
{
	m_sizeRow = size;
}

int32_t widget::Gird::GetColSize(int32_t colId)
{
	if (m_sizeCol.Size() > colId) {
		if (m_sizeCol[colId] <= 0) {
			return 0;
		}
		return m_sizeCol[colId];
	}
	EWOL_ERROR("Can not get the Colomn size : " << colId+1 << "  we have "<< m_sizeCol.Size() << " colomn");
	return 0;
}

int32_t widget::Gird::GetRowSize(void)
{
	return m_sizeRow;
}

void widget::Gird::SubWidgetRemoveAll(void)
{
	int32_t errorControl = m_subWidget.Size();
	// the size automaticly decrement with the auto call of the OnObjectRemove function
	while (m_subWidget.Size() > 0 ) {
		if (NULL != m_subWidget[0].widget) {
			delete(m_subWidget[0].widget);
			// no remove, this element is removed with the function OnObjectRemove ==> it does not exist anymore ...
			if (errorControl == m_subWidget.Size()) {
				EWOL_CRITICAL("[" << GetId() << "] The number of element might have been reduced ... ==> it is not the case ==> the herited class must call the \"OnObjectRemove\" function...");
				m_subWidget[0].widget = NULL;
			}
		} else {
			EWOL_WARNING("[" << GetId() << "] Must not have null pointer on the subWidget list ...");
			m_subWidget.Erase(0);
		}
		errorControl = m_subWidget.Size();
	}
	m_subWidget.Clear();
}


void widget::Gird::SubWidgetAdd(int32_t colId, int32_t rowId, ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	GirdProperties prop;
	prop.row = rowId;
	prop.col = colId;
	prop.widget = newWidget;
	
	// need to find the correct position : 
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (m_subWidget[iii].row < prop.row) {
			continue;
		} else if (m_subWidget[iii].row > prop.row) {
			// find a new position;
			m_subWidget.Insert(iii, prop);
			return;
		} else {
			if (m_subWidget[iii].col < prop.col) {
				continue;
			} else if (m_subWidget[iii].col > prop.col) {
				// find a new position;
				m_subWidget.Insert(iii, prop);
				return;
			} else {
				// The element already exist ==> replace it ...
				m_tmpWidget = m_subWidget[iii].widget;
				m_subWidget[iii].widget = newWidget;
				if (NULL != m_tmpWidget) {
					delete(m_tmpWidget);
					if (m_tmpWidget != NULL) {
						EWOL_CRITICAL("[" << GetId() << "] Error while replacing a widget ... ==> never call when free");
						m_tmpWidget = NULL;
					}
				}
			}
		}
	}
	// not find ==> just adding it ...
	m_subWidget.PushBack(prop);
}

void widget::Gird::SubWidgetRemove(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	int32_t errorControl = m_subWidget.Size();
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (newWidget == m_subWidget[iii].widget) {
			delete(m_subWidget[iii].widget);
			// no remove, this element is removed with the function OnObjectRemove ==> it does not exist anymore ...
			if (errorControl == m_subWidget.Size()) {
				EWOL_CRITICAL("[" << GetId() << "] The number of element might have been reduced ... ==> it is not the case ==> the herited class must call the \"OnObjectRemove\" function...");
				m_subWidget[iii].widget = NULL;
				m_subWidget.Erase(iii);
			}
			return;
		}
	}
	EWOL_WARNING("[" << GetId() << "] Can not remove unExistant widget");
}

void widget::Gird::SubWidgetRemove(int32_t colId, int32_t rowId)
{
	if (colId<0 || rowId<0) {
		EWOL_WARNING("[" << GetId() << "] try to remove widget with id < 0 col=" << colId << " row=" << rowId);
		return;
	}
	int32_t errorControl = m_subWidget.Size();
	// try to find it ...
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if(    m_subWidget[iii].row == rowId
		    && m_subWidget[iii].col == colId) {
			if (NULL==m_subWidget[iii].widget) {
				EWOL_WARNING("[" << GetId() << "] Remove NULL widget");
				m_subWidget.Erase(iii);
			} else {
				// The element already exist ==> replace it ...
				if (NULL != m_subWidget[iii].widget) {
					delete(m_subWidget[iii].widget);
					if (errorControl == m_subWidget.Size()) {
						EWOL_CRITICAL("[" << GetId() << "] The number of element might have been reduced ... ==> it is not the case ==> the herited class must call the \"OnObjectRemove\" function...");
						m_subWidget[iii].widget = NULL;
						m_subWidget.Erase(iii);
					}
				}
			}
			return;
		}
	}
	EWOL_WARNING("[" << GetId() << "] Can not remove unExistant widget");
}

void widget::Gird::SubWidgetUnLink(ewol::Widget* newWidget)
{
	if (NULL == newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (newWidget == m_subWidget[iii].widget) {
			m_subWidget[iii].widget = NULL;
			m_subWidget.Erase(iii);
			return;
		}
	}
}

void widget::Gird::SubWidgetUnLink(int32_t colId, int32_t rowId)
{
	if (colId<0 || rowId<0) {
		EWOL_WARNING("[" << GetId() << "] try to Unlink widget with id < 0 col=" << colId << " row=" << rowId);
		return;
	}
	// try to find it ...
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if(    m_subWidget[iii].row == rowId
		    && m_subWidget[iii].col == colId) {
			m_subWidget[iii].widget = NULL;
			m_subWidget.Erase(iii);
			return;
		}
	}
	EWOL_WARNING("[" << GetId() << "] Can not unLink unExistant widget");
}

void widget::Gird::OnDraw(ewol::DrawProperty& displayProp)
{
	// TODO : Only display the Widget inside the view ...
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii].widget) {
			m_subWidget[iii].widget->GenDraw(displayProp);
		}
	}
}

void widget::Gird::OnRegenerateDisplay(void)
{
	// TODO : Only display the Widget inside the view ...
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii].widget) {
			m_subWidget[iii].widget->OnRegenerateDisplay();
		}
	}
}

ewol::Widget * widget::Gird::GetWidgetAtPos(const vec2& pos)
{
	if (true == IsHide()) {
		return NULL;
	}
	// for all element in the sizer ...
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii].widget) {
			vec2 tmpSize = m_subWidget[iii].widget->GetSize();
			vec2 tmpOrigin = m_subWidget[iii].widget->GetOrigin();
			if(    (tmpOrigin.x() <= pos.x() && tmpOrigin.x() + tmpSize.x() >= pos.x())
			    && (tmpOrigin.y() <= pos.y() && tmpOrigin.y() + tmpSize.y() >= pos.y()) )
			{
				ewol::Widget * tmpWidget = m_subWidget[iii].widget->GetWidgetAtPos(pos);
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

void widget::Gird::OnObjectRemove(ewol::EObject * removeObject)
{
	// First step call parrent : 
	ewol::Widget::OnObjectRemove(removeObject);
	// second step find if in all the elements ...
	for(int32_t iii=m_subWidget.Size()-1; iii>=0; iii--) {
		if(m_subWidget[iii].widget == removeObject) {
			EWOL_VERBOSE("[" << GetId() << "]={" << GetObjectType() << "} Remove sizer sub Element [" << iii << "/" << m_subWidget.Size()-1 << "] ==> destroyed object");
			m_subWidget[iii].widget = NULL;
			m_subWidget.Erase(iii);
		}
	}
	if (m_tmpWidget == removeObject) {
		m_tmpWidget = NULL;
	}
}

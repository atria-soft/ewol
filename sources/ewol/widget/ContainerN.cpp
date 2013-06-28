/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/ewol.h>
#include <ewol/widget/ContainerN.h>
#include <ewol/widget/WidgetManager.h>

#undef __class__
#define __class__	"ContainerN"


widget::ContainerN::ContainerN(void) :
	m_lockExpand(false,false),
	m_subExpend(false,false)
{
	// nothing to do ...
}

widget::ContainerN::~ContainerN(void)
{
	SubWidgetRemoveAll();
}


bvec2 widget::ContainerN::CanExpand(void)
{
	bvec2 res = m_userExpand;
	if (false==m_lockExpand.x()) {
		if (true==m_subExpend.x()) {
			res.setX(true);
		}
	}
	if (false==m_lockExpand.y()) {
		if (true==m_subExpend.y()) {
			res.setY(true);
		}
	}
	//EWOL_DEBUG("Expend check : user=" << m_userExpand << " lock=" << m_lockExpand << " sub=" << m_subExpend << " res=" << res);
	return res;
}

void widget::ContainerN::LockExpand(const bvec2& _lockExpand)
{
	if (_lockExpand != m_lockExpand) {
		m_lockExpand = _lockExpand;
		MarkToRedraw();
		ewol::RequestUpdateSize();
	}
}


int32_t widget::ContainerN::SubWidgetAdd(ewol::Widget* _newWidget)
{
	if (NULL == _newWidget) {
		EWOL_ERROR("[" << GetId() << "] {" << GetObjectType() << "} Try to add An empty Widget ... ");
		return -1;
	}
	if (_newWidget!=NULL) {
		_newWidget->SetUpperWidget(this);
	}
	m_subWidget.PushBack(_newWidget);
	MarkToRedraw();
	ewol::RequestUpdateSize();
	// added at the last eelement :
	return _newWidget->GetId();
}

int32_t widget::ContainerN::SubWidgetAddStart(ewol::Widget* _newWidget)
{
	if (NULL == _newWidget) {
		EWOL_ERROR("[" << GetId() << "] {" << GetObjectType() << "} Try to add start An empty Widget ... ");
		return -1;
	}
	if (_newWidget!=NULL) {
		_newWidget->SetUpperWidget(this);
	}
	m_subWidget.PushFront(_newWidget);
	MarkToRedraw();
	ewol::RequestUpdateSize();
	return _newWidget->GetId();
}

void widget::ContainerN::SubWidgetRemove(ewol::Widget* _newWidget)
{
	if (NULL == _newWidget) {
		return;
	}
	int32_t errorControl = m_subWidget.Size();
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (_newWidget == m_subWidget[iii]) {
			m_subWidget[iii]->RemoveUpperWidget();
			delete(m_subWidget[iii]);
			// no remove, this element is removed with the function OnObjectRemove ==> it does not exist anymore ...
			if (errorControl == m_subWidget.Size()) {
				EWOL_CRITICAL("[" << GetId() << "] {" << GetObjectType() << "} The number of element might have been reduced ... ==> it is not the case ==> the herited class must call the \"OnObjectRemove\" function...");
				m_subWidget[iii] = NULL;
				m_subWidget.Erase(iii);
			}
			MarkToRedraw();
			ewol::RequestUpdateSize();
			return;
		}
	}
}

void widget::ContainerN::SubWidgetUnLink(ewol::Widget* _newWidget)
{
	if (NULL == _newWidget) {
		return;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (_newWidget == m_subWidget[iii]) {
			m_subWidget[iii]->RemoveUpperWidget();
			m_subWidget[iii] = NULL;
			m_subWidget.Erase(iii);
			MarkToRedraw();
			ewol::RequestUpdateSize();
			return;
		}
	}
}

void widget::ContainerN::SubWidgetRemoveAll(void)
{
	int32_t errorControl = m_subWidget.Size();
	// the size automaticly decrement with the auto call of the OnObjectRemove function
	while (m_subWidget.Size() > 0 ) {
		if (NULL != m_subWidget[0]) {
			m_subWidget[0]->RemoveUpperWidget();
			delete(m_subWidget[0]);
			// no remove, this element is removed with the function OnObjectRemove ==> it does not exist anymore ...
			if (errorControl == m_subWidget.Size()) {
				EWOL_CRITICAL("[" << GetId() << "] {" << GetObjectType() << "} The number of element might have been reduced ... ==> it is not the case ==> the herited class must call the \"OnObjectRemove\" function...");
				m_subWidget[0] = NULL;
			}
		} else {
			EWOL_WARNING("[" << GetId() << "] {" << GetObjectType() << "} Must not have null pointer on the subWidget list ...");
			m_subWidget.Erase(0);
		}
		errorControl = m_subWidget.Size();
	}
	m_subWidget.Clear();
}
void widget::ContainerN::SubWidgetRemoveAllDelayed(void)
{
	// the size automaticly decrement with the auto call of the OnObjectRemove function
	for(int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->RemoveUpperWidget();
			m_subWidget[iii]->RemoveObject();
			m_subWidget[iii] = NULL;
		} else {
			EWOL_WARNING("[" << GetId() << "] {" << GetObjectType() << "} Must not have null pointer on the subWidget list ...");
		}
	}
	m_subWidget.Clear();
}


ewol::Widget* widget::ContainerN::GetWidgetNamed(const etk::UString& _widgetName)
{
	ewol::Widget* tmpUpperWidget = ewol::Widget::GetWidgetNamed(_widgetName);
	if (NULL!=tmpUpperWidget) {
		return tmpUpperWidget;
	}
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			ewol::Widget* tmpWidget = m_subWidget[iii]->GetWidgetNamed(_widgetName);
			if (NULL != tmpWidget) {
				return tmpWidget;
			}
		}
	}
	return NULL;
}

void widget::ContainerN::OnObjectRemove(ewol::EObject* _removeObject)
{
	// First step call parrent : 
	ewol::Widget::OnObjectRemove(_removeObject);
	// second step find if in all the elements ...
	for(int32_t iii=m_subWidget.Size()-1; iii>=0; iii--) {
		if(m_subWidget[iii] == _removeObject) {
			EWOL_VERBOSE("[" << GetId() << "] {" << GetObjectType() << "} Remove sizer sub Element [" << iii << "/" << m_subWidget.Size()-1 << "] ==> destroyed object");
			m_subWidget[iii] = NULL;
			m_subWidget.Erase(iii);
		}
	}
}

void widget::ContainerN::SystemDraw(const ewol::DrawProperty& _displayProp)
{
	if (true==m_hide){
		// widget is hidden ...
		return;
	}
	// local widget draw
	ewol::Widget::SystemDraw(_displayProp);
	// subwidget draw
	ewol::DrawProperty prop = _displayProp;
	prop.Limit(m_origin, m_size);
	for (int32_t iii=m_subWidget.Size()-1; iii>=0; iii--) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->SystemDraw(prop);
		}
	}
}

void widget::ContainerN::CalculateSize(const vec2& _availlable)
{
	m_size = _availlable;
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->SetOrigin(m_origin+m_offset);
			m_subWidget[iii]->CalculateSize(m_size);
		}
	}
	MarkToRedraw();
}

void widget::ContainerN::CalculateMinMaxSize(void)
{
	m_subExpend.setValue(false, false);
	m_minSize.setValue(0,0);
	m_maxSize.setValue(ULTIMATE_MAX_SIZE,ULTIMATE_MAX_SIZE);
	//EWOL_ERROR("[" << GetId() << "] {" << GetObjectType() << "} Set min size : " <<  m_minSize);
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->CalculateMinMaxSize();
			bvec2 subExpendProp = m_subWidget[iii]->CanExpand();
			if (true == subExpendProp.x()) {
				m_subExpend.setX(true);
			}
			if (true == subExpendProp.y()) {
				m_subExpend.setY(true);
			}
			vec2 tmpSize = m_subWidget[iii]->GetCalculateMinSize();
			m_minSize.setValue( etk_max(tmpSize.x(), m_minSize.x()),
			                    etk_max(tmpSize.y(), m_minSize.y()) );
		}
	}
	//EWOL_ERROR("[" << GetId() << "] {" << GetObjectType() << "} Result min size : " <<  m_minSize);
}

void widget::ContainerN::OnRegenerateDisplay(void)
{
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			m_subWidget[iii]->OnRegenerateDisplay();
		}
	}
}

ewol::Widget* widget::ContainerN::GetWidgetAtPos(const vec2& _pos)
{
	if (true == IsHide()) {
		return NULL;
	}
	// for all element in the sizer ...
	for (int32_t iii=0; iii<m_subWidget.Size(); iii++) {
		if (NULL != m_subWidget[iii]) {
			vec2 tmpSize = m_subWidget[iii]->GetSize();
			vec2 tmpOrigin = m_subWidget[iii]->GetOrigin();
			if(    (tmpOrigin.x() <= _pos.x() && tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    && (tmpOrigin.y() <= _pos.y() && tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				ewol::Widget * tmpWidget = m_subWidget[iii]->GetWidgetAtPos(_pos);
				if (NULL != tmpWidget) {
					return tmpWidget;
				}
				// stop searching
				break;
			}
		}
	}
	return NULL;
};


bool widget::ContainerN::LoadXML(exml::Element* _node)
{
	if (NULL==_node) {
		return false;
	}
	// parse generic properties :
	ewol::Widget::LoadXML(_node);
	// remove previous element :
	SubWidgetRemoveAll();
	
	etk::UString tmpAttributeValue = _node->GetAttribute("lock");
	if (tmpAttributeValue.Size()!=0) {
		m_lockExpand = tmpAttributeValue;
	}
	bool invertAdding=false;
	tmpAttributeValue = _node->GetAttribute("addmode");
	if(tmpAttributeValue.CompareNoCase("invert")) {
		invertAdding=true;
	}
	// parse all the elements :
	for(int32_t iii=0; iii< _node->Size(); iii++) {
		exml::Element* pNode = _node->GetElement(iii);
		if (pNode==NULL) {
			// trash here all that is not element
			continue;
		}
		etk::UString widgetName = pNode->GetValue();
		if (ewol::widgetManager::Exist(widgetName) == false) {
			EWOL_ERROR("[" << GetId() << "] {" << GetObjectType() << "} (l "<<pNode->GetPos()<<") Unknown basic node=\"" << widgetName << "\" not in : [" << ewol::widgetManager::List() << "]" );
			continue;
		}
		EWOL_DEBUG("[" << GetId() << "] {" << GetObjectType() << "} load new element : \"" << widgetName << "\"");
		ewol::Widget *subWidget = ewol::widgetManager::Create(widgetName);
		if (subWidget == NULL) {
			EWOL_ERROR ("[" << GetId() << "] {" << GetObjectType() << "} (l "<<pNode->GetPos()<<") Can not create the widget : \"" << widgetName << "\"");
			continue;
		}
		// add sub element : 
		if (false==invertAdding) {
			SubWidgetAdd(subWidget);
		} else {
			SubWidgetAddStart(subWidget);
		}
		if (false == subWidget->LoadXML(pNode)) {
			EWOL_ERROR ("[" << GetId() << "] {" << GetObjectType() << "} (l "<<pNode->GetPos()<<") can not load widget properties : \"" << widgetName << "\"");
			return false;
		}
	}
	return true;
}


void widget::ContainerN::SetOffset(const vec2& _newVal)
{
	if (m_offset != _newVal) {
		ewol::Widget::SetOffset(_newVal);
		// recalculate the new sise and position of sub widget ...
		CalculateSize(m_size);
	}
}


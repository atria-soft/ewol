/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/PopUp.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/ewol.h>



#undef __class__
#define __class__	"PopUp"

const char* const widget::PopUp::configShaper="shaper";
const char* const widget::PopUp::configRemoveOnExternClick="out-click-remove";
const char* const widget::PopUp::configAnimation="animation";
const char* const widget::PopUp::configLockExpand="lock";

static ewol::Widget* Create(void)
{
	return new widget::PopUp();
}

void widget::PopUp::Init(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,&Create);
}

void widget::PopUp::UnInit(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,NULL);
}


widget::PopUp::PopUp(const etk::UString& _shaperName) :
	m_shaper(_shaperName),
	m_lockExpand(true,true),
	m_closeOutEvent(false)
{
	m_userExpand.setValue(false, false);
	SetMinSize(ewol::Dimension(vec2(80,80),ewol::Dimension::Pourcent));
	RegisterConfig(configShaper, "string", NULL, "The shaper properties");
	RegisterConfig(configRemoveOnExternClick, "bool", NULL, "Remove the widget if the use click outside");
	RegisterConfig(configLockExpand, "bool", NULL, "Lock expand contamination");
	RegisterConfig(configAnimation, "list", "none;increase", "Remove the widget if the use click outside");
	
	SetAnimationMode(animationNone);
}

widget::PopUp::~PopUp(void)
{
	
}

void widget::PopUp::LockExpand(const bvec2& _lockExpand)
{
	if (_lockExpand != m_lockExpand) {
		m_lockExpand = _lockExpand;
		MarkToRedraw();
		ewol::RequestUpdateSize();
	}
}

void widget::PopUp::SetShaperName(const etk::UString& _shaperName)
{
	m_shaper.SetSource(_shaperName);
	MarkToRedraw();
}

void widget::PopUp::CalculateSize(const vec2& _available)
{
	ewol::Widget::CalculateSize(_available);
	if (NULL != m_subWidget) {
		vec2 padding = m_shaper.GetPadding();
		vec2 subWidgetSize = m_subWidget->GetCalculateMinSize();
		if (true == m_subWidget->CanExpand().x()) {
			if (m_lockExpand.x()==true) {
				subWidgetSize.setX(m_minSize.x());
			} else {
				subWidgetSize.setX(m_size.x()-padding.x());
			}
		}
		if (true == m_subWidget->CanExpand().y()) {
			if (m_lockExpand.y()==true) {
				subWidgetSize.setY(m_minSize.y());
			} else {
				subWidgetSize.setY(m_size.y()-padding.y());
			}
		}
		// limit the size of the element :
		//subWidgetSize.setMin(m_minSize);
		// posiition at a int32_t pos :
		subWidgetSize = vec2ClipInt32(subWidgetSize);
		
		// set config to the Sub-widget
		vec2 subWidgetOrigin = m_origin + (m_size-subWidgetSize)/2.0f;
		subWidgetOrigin = vec2ClipInt32(subWidgetOrigin);
		
		m_subWidget->SetOrigin(subWidgetOrigin);
		m_subWidget->CalculateSize(subWidgetSize);
	}
	MarkToRedraw();
}

void widget::PopUp::SystemDraw(const ewol::DrawProperty& _displayProp)
{
	if (true==m_hide){
		// widget is hidden ...
		return;
	}
	ewol::Widget::SystemDraw(_displayProp);
	if (NULL!=m_subWidget) {
		if(    m_shaper.GetNextDisplayedStatus() == -1
		    && m_shaper.GetTransitionStatus() >= 1.0) {
			ewol::DrawProperty prop = _displayProp;
			prop.Limit(m_origin, m_size);
			m_subWidget->SystemDraw(prop);
		}
	}
}


void widget::PopUp::OnDraw(void)
{
	m_shaper.Draw();
}

void widget::PopUp::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		m_shaper.Clear();
		vec2 padding = m_shaper.GetPadding();
		vec2 tmpSize(0,0);
		bvec2 expand = CanExpand();
		bvec2 fill = CanFill();
		if (fill.x()) {
			tmpSize.setX(m_size.x()-padding.x()*2);
		}
		if (fill.y()) {
			tmpSize.setY(m_size.y()-padding.y()*2);
		}
		if (NULL != m_subWidget) {
			vec2 tmpSize = m_subWidget->GetSize();
		}
		tmpSize.setMax(m_minSize);
		vec2 tmpOrigin = (m_size-tmpSize)/2.0f;
		
		m_shaper.SetOrigin(vec2(0,0));
		m_shaper.SetSize(vec2ClipInt32(m_size));
		m_shaper.SetInsidePos(vec2ClipInt32(tmpOrigin-padding));
		m_shaper.SetInsideSize(vec2ClipInt32(tmpSize + padding*2.0f));
	}
	// SUBwIDGET GENERATION ...
	if (NULL != m_subWidget) {
		m_subWidget->OnRegenerateDisplay();
	}
}

ewol::Widget* widget::PopUp::GetWidgetAtPos(const vec2& pos)
{
	ewol::Widget* val = widget::Container::GetWidgetAtPos(pos);
	if (NULL != val) {
		return val;
	}
	return this;
}


bool widget::PopUp::OnSetConfig(const ewol::EConfig& _conf)
{
	if (true == widget::Container::OnSetConfig(_conf)) {
		return true;
	}
	if (_conf.GetConfig() == configShaper) {
		SetShaperName(_conf.GetData());
		return true;
	}
	if (_conf.GetConfig() == configRemoveOnExternClick) {
		SetRemoveOnExternClick(_conf.GetData().ToBool());
		return true;
	}
	if (_conf.GetConfig() == configLockExpand) {
		LockExpand(_conf.GetData());
		return true;
	}
	if (_conf.GetConfig() == configAnimation) {
		if (_conf.GetData().CompareNoCase("increase")==true) {
			SetAnimationMode(animationIncrease);
		} else {
			SetAnimationMode(animationNone);
		}
		return true;
	}
	return false;
}

bool widget::PopUp::OnGetConfig(const char* _config, etk::UString& _result) const
{
	if (true == widget::Container::OnGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configShaper) {
		_result = m_shaper.GetSource();
		return true;
	}
	if (_config == configLockExpand) {
		_result = m_lockExpand;
		return true;
	}
	if (_config == configRemoveOnExternClick) {
		if (GetRemoveOnExternClick()==true) {
			_result = "true";
		} else {
			_result = "false";
		}
		return true;
	}
	if (_config == configAnimation) {
		if (m_animation == animationIncrease) {
			_result = "increase";
		} else {
			_result = "none";
		}
		return true;
	}
	return false;
}


bool widget::PopUp::OnEventInput(const ewol::EventInput& _event)
{
	if (0 != _event.GetId()) {
		if (true==m_closeOutEvent) {
			vec2 padding = m_shaper.GetPadding();
			vec2 tmpSize(0,0);
			if (NULL != m_subWidget) {
				vec2 tmpSize = m_subWidget->GetSize();
			}
			tmpSize.setMax(m_minSize);
			vec2 tmpOrigin = (m_size-tmpSize)/2.0f;
			
			tmpOrigin-=padding;
			tmpSize += padding*2.0f;
			vec2 pos=RelativePosition(_event.GetPos());
			if(    pos.x() < tmpOrigin.x()
			    || pos.y() < tmpOrigin.y()
			    || pos.x() > tmpOrigin.x()+tmpSize.x()
			    || pos.y() > tmpOrigin.y()+tmpSize.y() ) {
				AutoDestroy();
				return true;
			}
		}
	}
	return false;
}

void widget::PopUp::SetAnimationMode(animation_te _animation)
{
	m_animation = _animation;
	if (true == m_shaper.ChangeStatusIn((int32_t)_animation) ) {
		PeriodicCallEnable();
	}
}

void widget::PopUp::PeriodicCall(const ewol::EventTime& _event)
{
	if (false == m_shaper.PeriodicCall(_event) ) {
		PeriodicCallDisable();
	}
	MarkToRedraw();
}


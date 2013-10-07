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

static ewol::Widget* create(void)
{
	return new widget::PopUp();
}

void widget::PopUp::init(ewol::WidgetManager& _widgetManager)
{
	_widgetManager.addWidgetCreator(__class__,&create);
}


widget::PopUp::PopUp(const etk::UString& _shaperName) :
	m_shaper(_shaperName),
	m_lockExpand(true,true),
	m_closeOutEvent(false)
{
	m_userExpand.setValue(false, false);
	setMinSize(ewol::Dimension(vec2(80,80),ewol::Dimension::Pourcent));
	registerConfig(configShaper, "string", NULL, "The shaper properties");
	registerConfig(configRemoveOnExternClick, "bool", NULL, "Remove the widget if the use click outside");
	registerConfig(configLockExpand, "bool", NULL, "Lock expand contamination");
	registerConfig(configAnimation, "list", "none;increase", "Remove the widget if the use click outside");
	
	setAnimationMode(animationNone);
}

widget::PopUp::~PopUp(void)
{
	
}

void widget::PopUp::lockExpand(const bvec2& _lockExpand)
{
	if (_lockExpand != m_lockExpand) {
		m_lockExpand = _lockExpand;
		markToRedraw();
		requestUpdateSize();
	}
}

void widget::PopUp::setShaperName(const etk::UString& _shaperName)
{
	m_shaper.setSource(_shaperName);
	markToRedraw();
}

void widget::PopUp::calculateSize(const vec2& _available)
{
	ewol::Widget::calculateSize(_available);
	if (NULL != m_subWidget) {
		vec2 padding = m_shaper.getPadding();
		vec2 subWidgetSize = m_subWidget->getCalculateMinSize();
		if (true == m_subWidget->canExpand().x()) {
			if (m_lockExpand.x() == true) {
				subWidgetSize.setX(m_minSize.x());
			} else {
				subWidgetSize.setX(m_size.x()-padding.x());
			}
		}
		if (true == m_subWidget->canExpand().y()) {
			if (m_lockExpand.y() == true) {
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
		
		m_subWidget->setOrigin(subWidgetOrigin);
		m_subWidget->calculateSize(subWidgetSize);
	}
	markToRedraw();
}

void widget::PopUp::systemDraw(const ewol::drawProperty& _displayProp)
{
	if (true == m_hide){
		// widget is hidden ...
		return;
	}
	ewol::Widget::systemDraw(_displayProp);
	if (NULL!=m_subWidget) {
		if(    m_shaper.getNextDisplayedStatus() == -1
		    && m_shaper.getTransitionStatus() >= 1.0) {
			ewol::drawProperty prop = _displayProp;
			prop.limit(m_origin, m_size);
			m_subWidget->systemDraw(prop);
		}
	}
}


void widget::PopUp::onDraw(void)
{
	m_shaper.draw();
}

void widget::PopUp::onRegenerateDisplay(void)
{
	if (true == needRedraw()) {
		m_shaper.clear();
		vec2 padding = m_shaper.getPadding();
		vec2 tmpSize(0,0);
		bvec2 expand = canExpand();
		bvec2 fill = canFill();
		if (fill.x()) {
			tmpSize.setX(m_size.x()-padding.x()*2);
		}
		if (fill.y()) {
			tmpSize.setY(m_size.y()-padding.y()*2);
		}
		if (NULL != m_subWidget) {
			vec2 tmpSize = m_subWidget->getSize();
		}
		tmpSize.setMax(m_minSize);
		vec2 tmpOrigin = (m_size-tmpSize)/2.0f;
		
		m_shaper.setOrigin(vec2(0,0));
		m_shaper.setSize(vec2ClipInt32(m_size));
		m_shaper.setInsidePos(vec2ClipInt32(tmpOrigin-padding));
		m_shaper.setInsideSize(vec2ClipInt32(tmpSize + padding*2.0f));
	}
	// SUBwIDGET GENERATION ...
	if (NULL != m_subWidget) {
		m_subWidget->onRegenerateDisplay();
	}
}

ewol::Widget* widget::PopUp::getWidgetAtPos(const vec2& pos)
{
	ewol::Widget* val = widget::Container::getWidgetAtPos(pos);
	if (NULL != val) {
		return val;
	}
	return this;
}


bool widget::PopUp::onSetConfig(const ewol::EConfig& _conf)
{
	if (true == widget::Container::onSetConfig(_conf)) {
		return true;
	}
	if (_conf.getConfig() == configShaper) {
		setShaperName(_conf.getData());
		return true;
	}
	if (_conf.getConfig() == configRemoveOnExternClick) {
		setRemoveOnExternClick(_conf.getData().toBool());
		return true;
	}
	if (_conf.getConfig() == configLockExpand) {
		lockExpand(_conf.getData());
		return true;
	}
	if (_conf.getConfig() == configAnimation) {
		if (_conf.getData().compareNoCase("increase") == true) {
			setAnimationMode(animationIncrease);
		} else {
			setAnimationMode(animationNone);
		}
		return true;
	}
	return false;
}

bool widget::PopUp::onGetConfig(const char* _config, etk::UString& _result) const
{
	if (true == widget::Container::onGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configShaper) {
		_result = m_shaper.getSource();
		return true;
	}
	if (_config == configLockExpand) {
		_result = m_lockExpand;
		return true;
	}
	if (_config == configRemoveOnExternClick) {
		if (getRemoveOnExternClick() == true) {
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


bool widget::PopUp::onEventInput(const ewol::EventInput& _event)
{
	if (0 != _event.getId()) {
		if (true == m_closeOutEvent) {
			vec2 padding = m_shaper.getPadding();
			vec2 tmpSize(0,0);
			if (NULL != m_subWidget) {
				vec2 tmpSize = m_subWidget->getSize();
			}
			tmpSize.setMax(m_minSize);
			vec2 tmpOrigin = (m_size-tmpSize)/2.0f;
			
			tmpOrigin-=padding;
			tmpSize += padding*2.0f;
			vec2 pos = relativePosition(_event.getPos());
			if(    pos.x() < tmpOrigin.x()
			    || pos.y() < tmpOrigin.y()
			    || pos.x() > tmpOrigin.x()+tmpSize.x()
			    || pos.y() > tmpOrigin.y()+tmpSize.y() ) {
				autoDestroy();
				return true;
			}
		}
	}
	return false;
}

void widget::PopUp::setAnimationMode(animation_te _animation)
{
	m_animation = _animation;
	if (true == m_shaper.changeStatusIn((int32_t)_animation) ) {
		periodicCallEnable();
	}
}

void widget::PopUp::periodicCall(const ewol::EventTime& _event)
{
	if (false == m_shaper.periodicCall(_event) ) {
		periodicCallDisable();
	}
	markToRedraw();
}


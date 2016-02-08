/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#include <ewol/debug.h>
#include <ewol/ewol.h>
#include <ewol/widget/Select.h>

#undef __class__
#define __class__ "Select"


// DEFINE for the shader display system :
#define STATUS_UP        (0)
#define STATUS_HOVER     (2)
#define STATUS_PRESSED   (1)
#define STATUS_DOWN      (3)

ewol::widget::Select::Select() :
  signalValueString(*this, "valueString", "select value change in a sring mode"),
  signalValue(*this, "value", "Select value change"),
  m_value(*this, "value", false, "Value of the Select") {
	addObjectType("ewol::widget::Select");
}

void ewol::widget::Select::init(const std::string& _shaperName) {
	ewol::widget::SpinBase::init(ewol::widget::spinPosition_noneRight, _shaperName);
	//m_shaper->setSource(_shaperName);
	//m_shaperIdSize = m_shaper->requestConfig("box-size");
	//m_shaperIdSizeInsize = m_shaper->requestConfig("box-inside");
}

ewol::widget::Select::~Select() {
	
}

void ewol::widget::Select::onParameterChangeValue(const ewol::parameter::Ref& _paramPointer) {
	ewol::widget::SpinBase::onParameterChangeValue(_paramPointer);
	if (_paramPointer == m_value) {
		markToRedraw();
	}
}

bool ewol::widget::Select::loadXML(const std::shared_ptr<const exml::Element>& _node) {
	if (_node == nullptr) {
		return false;
	}
	// parse generic properties:
	ewol::Widget::loadXML(_node);
	// remove previous element:
	//subWidgetRemove();
	// parse all the elements:
	for(size_t iii=0; iii< _node->size(); iii++) {
		std::shared_ptr<const exml::Element> pNode = _node->getElement(iii);
		if (pNode == nullptr) {
			// trash here all that is not element
			continue;
		}
		if (pNode->getValue() != "option") {
			EWOL_ERROR("(l "<<pNode->getPos()<<") Unknown basic node='" << pNode->getValue() << "' not in : [option]" );
			continue;
		}
		std::string valId = pNode->getAttribute("id");
		std::string valIsSelected = pNode->getAttribute("select");
		std::string valText = pNode->getText();
		
		EWOL_WARNING("Add option : id='" << valId << "' select='" << valIsSelected << "' text='" << valText << "'");
	}
	return true;
}


/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/TreeView.hpp>

#include <ewol/compositing/Drawing.hpp>
#include <ewol/compositing/Text.hpp>
#include <ewol/compositing/Image.hpp>
#include <ewol/widget/Manager.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::TreeView);

ewol::widget::TreeView::TreeView():
  ewol::widget::List(),
  propertyOffsetTreeView(this, "offsetTreeView",
                               30,
                               "Offset indentation for each node",
                               &ewol::widget::TreeView::onChangePropertyOffsetTreeView),
  propertyIconTreeViewSize(this, "iconTreeViewSize",
                                 30,
                                 "Size of the icon for treeView",
                                 &ewol::widget::TreeView::onChangePropertyOffsetTreeView) {
	addObjectType("ewol::widget::TreeView");
}

void ewol::widget::TreeView::init() {
	ewol::widget::List::init();
	propertyFill.set(bvec2(true,false));
}

ewol::widget::TreeView::~TreeView() {
	
}

vec2 ewol::widget::TreeView::calculateElementSize(const ivec2& _pos) {
	ewol::compositing::Text tmpText;
	etk::String myTextToWrite = getData(ListRole::Text, _pos).getSafeString();
	float_t treeOffset = 0;
	if (_pos.x() == 0) {
		treeOffset += getData(ListRole::DistanceToRoot, _pos).getSafeNumber() * propertyOffsetTreeView.get();
		#if 0
			bool haveChild = getData(ListRole::HaveChild, _pos).getSafeBoolean();
			if (haveChild == true) {
				treeOffset += propertyOffsetTreeView.get();
			}
		#else
			treeOffset += propertyOffsetTreeView.get();
		#endif
	}
	etk::String iconName;
	float_t iconSize = 0;
	if (_pos.x() == 0) {
		iconName = getData(ListRole::Icon, _pos).getSafeString();
		if (iconName != "") {
			iconSize += propertyIconTreeViewSize.get();
		}
	}
	vec3 textSize = tmpText.calculateSizeDecorated(myTextToWrite);
	ivec2 count = getMatrixSize();
	return vec2(textSize.x() + treeOffset + iconSize,
	            etk::max(textSize.y(), iconSize) + m_paddingSizeY*3
	            );
}

void ewol::widget::TreeView::drawElement(const ivec2& _pos, const vec2& _start, const vec2& _size) {
	vec2 posStart = _start;
	etk::String iconName;
	etk::Color<> fg = getData(ListRole::FgColor, _pos).getSafeColor();
	if (_pos.x() == 0) {
		auto value = getData(ListRole::DistanceToRoot, _pos);
		if (value.isNumber() == true) {
			posStart.setX(posStart.x() + value.getSafeNumber() * propertyOffsetTreeView.get());
		}
		iconName = getData(ListRole::Icon, _pos).getSafeString();
		bool haveChild = getData(ListRole::HaveChild, _pos).getSafeBoolean();
		if (haveChild == true) {
			ewol::compositing::Image * tmpImage = null;
			if ( getData(ListRole::IsExpand, _pos).getSafeBoolean() == false) {
				tmpImage = ETK_NEW(ewol::compositing::Image, "{ewol}THEME:GUI:ChevronRight.svg");
			} else {
				tmpImage = ETK_NEW(ewol::compositing::Image, "{ewol}THEME:GUI:ChevronMore.svg");
			}
			if (tmpImage != null) {
				addOObject(tmpImage);
				tmpImage->setColor(fg);
				tmpImage->setPos(posStart);
				tmpImage->print(vec2(propertyIconTreeViewSize.get(), propertyIconTreeViewSize.get()));
			}
		}
		// move right
		posStart.setX(posStart.x() + propertyIconTreeViewSize.get());
	}
	
	etk::String myTextToWrite = getData(ListRole::Text, _pos).getSafeString();
	auto backgroundVariant = getData(ListRole::BgColor, _pos);
	if (backgroundVariant.isColor() == true) {
		etk::Color<> bg = backgroundVariant.getColor();
		ewol::compositing::Drawing * BGOObjects = ETK_NEW(ewol::compositing::Drawing);
		if (BGOObjects != null) {
			addOObject(BGOObjects);
			BGOObjects->setColor(bg);
			BGOObjects->setPos(_start);
			BGOObjects->rectangleWidth(_size);
		}
	}
	posStart += vec2(m_paddingSizeX, m_paddingSizeY);
	if (iconName != "") {
		ewol::compositing::Image * tmpImage = ETK_NEW(ewol::compositing::Image, iconName);
		if (tmpImage != null) {
			addOObject(tmpImage);
			tmpImage->setColor(fg);
			tmpImage->setPos(posStart);
			tmpImage->print(vec2(propertyIconTreeViewSize.get(), propertyIconTreeViewSize.get()));
		}
		// move right
		posStart.setX(posStart.x() + propertyIconTreeViewSize.get());
	}
	if (myTextToWrite != "") {
		ewol::compositing::Text * tmpText = ETK_NEW(ewol::compositing::Text);
		if (tmpText != null) {
			addOObject(tmpText);
			tmpText->setColor(fg);
			tmpText->setPos(posStart);
			tmpText->printDecorated(myTextToWrite);;
		}
	}
}

void ewol::widget::TreeView::onChangePropertyOffsetTreeView() {
	markToRedraw();
}

bool ewol::widget::TreeView::onItemEvent(const ewol::event::Input& _event, const ivec2& _pos, const vec2& _mousePosition) {
	if (_event.getStatus() != gale::key::status::pressSingle) {
		return false;
	}
	if (_event.getId() != 1) {
		return false;
	}
	if (_pos.x() != 0) {
		return false;
	}
	//EWOL_INFO("event: " << _event);
	vec2 posStart = vec2(0,0);
	bool haveChild = getData(ListRole::HaveChild, _pos).getSafeBoolean();
	if (haveChild == false) {
		return false;
	}
	auto value = getData(ListRole::DistanceToRoot, _pos);
	if (value.isNumber() == true) {
		posStart.setX(posStart.x() + value.getSafeNumber() * propertyOffsetTreeView.get());
	}
	// Inverse the display of Y
	EWOL_VERBOSE("check: " << vec2(_mousePosition.x(), m_listSizeY[_pos.y()] - _mousePosition.y())
	             << " in " << posStart
	             << " -> " << (posStart+vec2(propertyIconTreeViewSize.get(),propertyIconTreeViewSize.get())));
	if (    _mousePosition.x() >= posStart.x()
	     && _mousePosition.x() <= posStart.x()+propertyIconTreeViewSize.get()
	     && m_listSizeY[_pos.y()] - _mousePosition.y() >= posStart.y()
	     && m_listSizeY[_pos.y()] - _mousePosition.y() <= propertyIconTreeViewSize.get() ) {
		onItemExpandEvent(_pos);
		return true;
	}
	return false;
}
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
                               15,
                               "Offset indentation for each node",
                               ewol::widget::TreeView::onChangePropertyOffsetTreeView),
  propertyIconTreeViewSize(this, "iconTreeViewSize",
                                 20,
                                 "Size of the icon for treeView",
                                 ewol::widget::TreeView::onChangePropertyOffsetTreeView),
  propertyTextIsDecorated(this, "textIsDecorated",
                                 true,
                                 "Text is draw as decorated mode",
                                 ewol::widget::TreeView::onChangePropertyTextDecorated) {
	addObjectType("ewol::widget::TreeView");
}

void ewol::widget::TreeView::init() {
	ewol::widget::List::init();
	propertyFill.set(Vector2b(true,false));
	addComposeElemnent("image_ChevronRight", ememory::make<ewol::compositing::Image>("THEME_GUI:///ChevronRight.svg?lib=ewol"));
	addComposeElemnent("image_ChevronMore", ememory::make<ewol::compositing::Image>("THEME_GUI:///ChevronMore.svg?lib=ewol"));
}

ewol::widget::TreeView::~TreeView() {
	
}

Vector2f ewol::widget::TreeView::calculateElementSize( Vector2i _pos) {
	auto tmpText = ememory::staticPointerCast<ewol::compositing::Text>(getComposeElemnent("text"));
	String myTextToWrite = getData(ListRole::Text, _pos).getSafeString();
	float_t treeOffset = 0;
	if (_pos.x() == 0) {
		treeOffset += getData(ListRole::DistanceToRoot, _pos).getSafeNumber() * propertyOffsetTreeView.get();
		#if 0
			boolean haveChild = getData(ListRole::HaveChild, _pos).getSafeBoolean();
			if (haveChild == true) {
				treeOffset += propertyOffsetTreeView.get();
			}
		#else
			treeOffset += propertyOffsetTreeView.get();
		#endif
	}
	String iconName;
	float_t iconSize = 0;
	if (_pos.x() == 0) {
		iconName = getData(ListRole::Icon, _pos).getSafeString();
		if (iconName != "") {
			iconSize += propertyIconTreeViewSize.get();
		}
	}
	Vector3f textSize;
	if (propertyTextIsDecorated.get() == true) {
		textSize = tmpText.calculateSizeDecorated(myTextToWrite);
	} else {
		textSize = tmpText.calculateSize(myTextToWrite);
	}
	Vector2i count = getMatrixSize();
	return Vector2f(textSize.x() + treeOffset + iconSize,
	            etk::max(textSize.y(), iconSize) + this.paddingSizeY*2
	            );
}

void ewol::widget::TreeView::drawElement( Vector2i _pos,  Vector2f _start,  Vector2f _size) {
	Vector2f posStart = _start;
	String iconName;
	etk::Color<> fg = getData(ListRole::FgColor, _pos).getSafeColor();
	if (_pos.x() == 0) {
		auto value = getData(ListRole::DistanceToRoot, _pos);
		if (value.isNumber() == true) {
			posStart.setX(posStart.x() + value.getSafeNumber() * propertyOffsetTreeView.get());
		}
		iconName = getData(ListRole::Icon, _pos).getSafeString();
		boolean haveChild = getData(ListRole::HaveChild, _pos).getSafeBoolean();
		if (haveChild == true) {
			ememory::Ptr<ewol::compositing::Image> tmpImage = null;
			if ( getData(ListRole::IsExpand, _pos).getSafeBoolean() == false) {
				tmpImage = ememory::staticPointerCast<ewol::compositing::Image>(getComposeElemnent("image_ChevronRight"));
			} else {
				tmpImage = ememory::staticPointerCast<ewol::compositing::Image>(getComposeElemnent("image_ChevronMore"));
			}
			if (tmpImage != null) {
				tmpImage.setColor(fg);
				tmpImage.setPos(posStart);
				tmpImage.print(Vector2f(propertyIconTreeViewSize.get(), propertyIconTreeViewSize.get()));
			}
		}
		// move right
		posStart.setX(posStart.x() + propertyIconTreeViewSize.get());
	}
	
	String myTextToWrite = getData(ListRole::Text, _pos).getSafeString();
	auto backgroundVariant = getData(ListRole::BgColor, _pos);
	if (backgroundVariant.isColor() == true) {
		etk::Color<> bg = backgroundVariant.getColor();
		auto BGOObjects = ememory::staticPointerCast<ewol::compositing::Drawing>(getComposeElemnent("drawing"));
		if (BGOObjects != null) {
			BGOObjects.setColor(bg);
			BGOObjects.setPos(_start);
			BGOObjects.rectangleWidth(_size);
		}
	}
	posStart += Vector2f(this.paddingSizeX, this.paddingSizeY);
	if (iconName != "") {
		auto tmpImage = ememory::staticPointerCast<ewol::compositing::Image>(getComposeElemnent(iconName));
		if (tmpImage != null) {
			tmpImage.setColor(fg);
			tmpImage.setPos(posStart);
			tmpImage.print(Vector2f(propertyIconTreeViewSize.get(), propertyIconTreeViewSize.get()));
		} else {
			Log.error("can not get : " + iconName );
		}
		// move right
		posStart.setX(posStart.x() + propertyIconTreeViewSize.get());
	}
	if (myTextToWrite != "") {
		auto tmpText = ememory::staticPointerCast<ewol::compositing::Text>(getComposeElemnent("text"));
		if (tmpText != null) {
			tmpText.setColor(fg);
			tmpText.setPos(posStart);
			if (propertyTextIsDecorated.get() == true) {
				tmpText.printDecorated(myTextToWrite);
			} else {
				tmpText.print(myTextToWrite);
			}
		}
	}
}

void ewol::widget::TreeView::onChangePropertyTextDecorated() {
	markToRedraw();
}

void ewol::widget::TreeView::onChangePropertyOffsetTreeView() {
	markToRedraw();
}

boolean ewol::widget::TreeView::onItemEvent( ewol::event::Input _event,  Vector2i _pos,  Vector2f _mousePosition) {
	if (_event.getStatus() != KeyStatus::pressSingle) {
		return false;
	}
	if (_event.getId() != 1) {
		return false;
	}
	if (_pos.x() != 0) {
		return false;
	}
	//Log.info("event: " + _event);
	Vector2f posStart = Vector2f(0,0);
	boolean haveChild = getData(ListRole::HaveChild, _pos).getSafeBoolean();
	if (haveChild == false) {
		return false;
	}
	auto value = getData(ListRole::DistanceToRoot, _pos);
	if (value.isNumber() == true) {
		posStart.setX(posStart.x() + value.getSafeNumber() * propertyOffsetTreeView.get());
	}
	// Inverse the display of Y
	Log.verbose("check: " + Vector2f(_mousePosition.x(), this.listSizeY[_pos.y()] - _mousePosition.y())
	             + " in " + posStart
	             + " . " + (posStart+Vector2f(propertyIconTreeViewSize.get(),propertyIconTreeViewSize.get())));
	if (    _mousePosition.x() >= posStart.x()
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _mousePosition.x() <= posStart.x()+propertyIconTreeViewSize.get()
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.listSizeY[_pos.y()] - _mousePosition.y() >= posStart.y()
	     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.listSizeY[_pos.y()] - _mousePosition.y() <= propertyIconTreeViewSize.get() ) {
		onItemExpandEvent(_pos);
		return true;
	}
	return false;
}
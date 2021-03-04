


void EwolContext::onClipboardEvent(enum gale::context::clipBoard::clipboardListe _clipboardId) 

EwolContext::Context(EwolApplication* _application) :
EwolContext::~Context() {
	// nothing to do ...
}

void EwolContext::requestUpdateSize() {
	Context context = gale::getContext();
	context.requestUpdateSize();
}

void EwolContext::onPeriod( echrono::Clock _time) {
	this.objectManager.timeCall(_time);
}

void EwolContext::resetIOEvent() {
	this.input.newLayerSet();
}

void EwolContext::setWindows( ewol::widget::WindowsShared _windows) {
	Log.info("set New windows");
	// remove current focus :
	this.widgetManager.focusSetDefault(null);
	this.widgetManager.focusRelease();
	// set the new pointer as windows system
	this.windowsCurrent = _windows;
	// set the new default focus:
	this.widgetManager.focusSetDefault(_windows);
	// display the title of the Windows:
	if (this.windowsCurrent != null) {
		setTitle(this.windowsCurrent.propertyTitle.get());
	}
	// request all the widget redrawing
	forceRedrawAll();
}

ewol::widget::WindowsShared EwolContext::getWindows() {
	return this.windowsCurrent;
};
void EwolContext::onResize( Vector2i _size) {
	Log.verbose("Resize: " + _size);
	forceRedrawAll();
}

void EwolContext::forceRedrawAll() {
	if (this.windowsCurrent == null) {
		return;
	}
	Vector2i size = getSize();
	this.windowsCurrent.setSize(Vector2f(size.x(), size.y()));
	this.windowsCurrent.onChangeSize();
}


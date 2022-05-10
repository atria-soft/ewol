package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
/**
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.aknot.exception.AknotException;
import org.atriasoft.etk.Dimension3f;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector3b;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.Gravity;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.object.EwolObject;
import org.atriasoft.exml.Exml;
import org.atriasoft.exml.exception.ExmlException;
import org.atriasoft.gale.context.ClipboardList;
import org.atriasoft.gale.context.Cursor;
import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeySpecial;

/**
 * the composer widget is a widget that create a link on a string.file to parse the data and generate some widget tree
 */
public class Composer extends Container {
	public static Widget composerGenerateFile(final Uri data) {
		return composerGenerateFile(data, 0);
	}
	
	public static Widget composerGenerateFile(final Uri uri, final long id) {
		final byte[] elemData = Uri.getAllData(uri);
		if (elemData == null) {
			Log.error("Can not read the Stream : " + uri);
			return null;
		}
		final String dataToParse = new String(elemData);
		final Widget tmp = composerGenerateString(dataToParse, id);
		if (tmp == null) {
			Log.error("Faiul to Load data: {}", uri);
		}
		return tmp;
	}
	
	public static Widget composerGenerateString(final String data) {
		return composerGenerateString(data, 0);
	}
	
	public static Widget composerGenerateString(String data, final long id) {
		boolean requestComposer = true;
		if (!data.startsWith("<Composer>")) {
			data = "<Composer>\n" + data + "\n</Composer>";
			requestComposer = false;
		}
		data = data.replace("{ID}", Long.toString(id));
		Composer[] result = null;
		try {
			result = Exml.parse(data, Composer.class, "Composer");//new WidgetXmlFactory());
		} catch (final ExmlException ex) {
			Log.error("Fail to load Data !!! {}", ex.toString());
			ex.printStackTrace();
		} catch (final AknotException ex) {
			Log.error("Fail to load Data !!! {}", ex.toString());
			ex.printStackTrace();
		}
		if (result == null) {
			return null;
		}
		if (requestComposer) {
			return result[0];
		}
		return result[0].getSubWidget();
	}
	
	protected boolean propertyRemoveIfUnderRemove; //!< Remove the composer if sub element request a remove
	
	protected Uri propertySubFile; //!< If loading a sub-file, we must do it here ==> permit to configure it in the xml and not have wrong display
	
	/**
	 * Constructor
	 */
	public Composer() {
		// nothing to do...
		
	}
	
	@Override
	public void calculateMinMaxSize() {
		if (this.subWidget != null) {
			this.subWidget.calculateMinMaxSize();
			return;
		}
		super.calculateMinMaxSize();
	}
	
	@Override
	public void calculateSize() {
		
		if (this.subWidget != null) {
			this.subWidget.calculateSize();
			return;
		}
		super.calculateSize();
	}
	
	@Override
	public Vector3b canExpand() {
		
		if (this.subWidget != null) {
			return this.subWidget.canExpand();
		}
		return super.canExpand();
	}
	
	@Override
	public Vector3b canExpandIfFree() {
		
		if (this.subWidget != null) {
			return this.subWidget.canExpandIfFree();
		}
		return super.canExpandIfFree();
	}
	
	@Override
	public Vector3b canFill() {
		
		if (this.subWidget != null) {
			return this.subWidget.canFill();
		}
		return super.canFill();
	}
	
	@Override
	void changeZoom(final float range) {
		if (this.subWidget != null) {
			this.subWidget.changeZoom(range);
			return;
		}
		super.changeZoom(range);
	}
	
	@Override
	public void checkMaxSize() {
		if (this.subWidget != null) {
			this.subWidget.checkMaxSize();
			return;
		}
		super.checkMaxSize();
	}
	
	@Override
	public void checkMinSize() {
		
		if (this.subWidget != null) {
			this.subWidget.checkMinSize();
			return;
		}
		super.checkMinSize();
	}
	
	@Override
	public Vector3f getCalculateMaxSize() {
		
		if (this.subWidget != null) {
			return this.subWidget.getCalculateMaxSize();
		}
		return super.getCalculateMaxSize();
	}
	
	@Override
	public Vector3f getCalculateMinSize() {
		
		if (this.subWidget != null) {
			return this.subWidget.getCalculateMinSize();
		}
		return super.getCalculateMinSize();
	}
	
	@Override
	public Cursor getCursor() {
		
		if (this.subWidget != null) {
			return this.subWidget.getCursor();
		}
		return super.getCursor();
	}
	
	@Override
	public boolean getGrabStatus() {
		
		if (this.subWidget != null) {
			return this.subWidget.getGrabStatus();
		}
		return super.getGrabStatus();
	}
	
	@Override
	public boolean getKeyboardRepeat() {
		
		if (this.subWidget != null) {
			return this.subWidget.getKeyboardRepeat();
		}
		return super.getKeyboardRepeat();
	}
	
	@Override
	public int getMouseLimit() {
		
		if (this.subWidget != null) {
			return this.subWidget.getMouseLimit();
		}
		return super.getMouseLimit();
	}
	
	@Override
	Vector3f getOffset() {
		if (this.subWidget != null) {
			return this.subWidget.getOffset();
		}
		return super.getOffset();
	}
	
	@Override
	public Vector3f getOrigin() {
		if (this.subWidget != null) {
			return this.subWidget.getOrigin();
		}
		return super.getOrigin();
	}
	
	@Override
	public boolean getPropertyCanFocus() {
		
		if (this.subWidget != null) {
			return this.subWidget.getPropertyCanFocus();
		}
		return super.getPropertyCanFocus();
	}
	
	@Override
	public Vector3b getPropertyExpand() {
		
		if (this.subWidget != null) {
			return this.subWidget.getPropertyExpand();
		}
		return super.getPropertyExpand();
	}
	
	@Override
	public Vector3b getPropertyExpandIfFree() {
		
		if (this.subWidget != null) {
			return this.subWidget.getPropertyExpandIfFree();
		}
		return super.getPropertyExpandIfFree();
	}
	
	@Override
	public Vector3b getPropertyFill() {
		
		if (this.subWidget != null) {
			return this.subWidget.getPropertyFill();
		}
		return super.getPropertyFill();
	}
	
	@Override
	public Gravity getPropertyGravity() {
		
		if (this.subWidget != null) {
			return this.subWidget.getPropertyGravity();
		}
		return super.getPropertyGravity();
	}
	
	@Override
	public boolean getPropertyHide() {
		
		if (this.subWidget != null) {
			return this.subWidget.getPropertyHide();
		}
		return super.getPropertyHide();
	}
	
	@Override
	public Dimension3f getPropertyMaxSize() {
		
		if (this.subWidget != null) {
			return this.subWidget.getPropertyMaxSize();
		}
		return super.getPropertyMaxSize();
	}
	
	@Override
	public Dimension3f getPropertyMinSize() {
		
		if (this.subWidget != null) {
			return this.subWidget.getPropertyMinSize();
		}
		return super.getPropertyMinSize();
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "sub-file")
	@AknotDescription(value = "compose with a subXML file")
	public Uri getPropertySubFile() {
		return this.propertySubFile;
	}
	
	@Override
	public Vector3f getSize() {
		
		if (this.subWidget != null) {
			return this.subWidget.getSize();
		}
		return super.getSize();
	}
	
	@Override
	public EwolObject getSubObjectNamed(final String objectName) {
		if (this.subWidget != null) {
			return this.subWidget.getSubObjectNamed(objectName);
		}
		return super.getSubObjectNamed(objectName);
	}
	
	@Override
	public float getZoom() {
		if (this.subWidget != null) {
			return this.subWidget.getZoom();
		}
		return super.getZoom();
	}
	
	@Override
	public void grabCursor() {
		if (this.subWidget != null) {
			this.subWidget.grabCursor();
			return;
		}
		super.grabCursor();
	}
	
	@Override
	public boolean isFocused() {
		if (this.subWidget != null) {
			return this.subWidget.isFocused();
		}
		return super.isFocused();
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "remove-if-under-remove")
	@AknotDescription(value = "Demand the remove iof the widget if the subObject demand a remove")
	public boolean isPropertyRemoveIfUnderRemove() {
		return this.propertyRemoveIfUnderRemove;
	}
	
	@Override
	public void keepFocus() {
		if (this.subWidget != null) {
			this.subWidget.keepFocus();
			return;
		}
		super.keepFocus();
	}
	
	/**
	 * load a composition with a file
	 * @param _uri Name of the file
	 * @param _id Unique ID that is used in replacing the balise "{ID}" inside the File (do nothing if == 0)
	 * @return true  == > all done OK
	 * @return false  == > some error occured
	 */
	public boolean loadFromFile(final Uri uri) {
		final Widget data = composerGenerateFile(uri, this.getId());
		// check parse is well done.
		if (data == null) {
			return false;
		}
		// keep the real Data (remove subComposer
		if (data instanceof final Composer tmp) {
			setSubWidget(tmp.getSubWidget());
		} else {
			setSubWidget(data);
		}
		// T O D O: Change this with a throw.a..a
		return true;
	}
	
	/**
	 * load a composition with a file
	 * @param composerXmlString xml to parse directly
	 * @param id Unique ID that is used in replacing the balise "{ID}" inside the String (do nothing if == 0)
	 * @return true  == > all done OK
	 * @return false  == > some error occured
	 */
	public boolean loadFromString(final String composerXmlString) {
		final Widget data = composerGenerateString(composerXmlString, this.getId());
		// check parse is well done.
		if (data == null) {
			return false;
		}
		// keep the real Data (remove subComposer
		if (data instanceof final Composer tmp) {
			setSubWidget(tmp.getSubWidget());
		} else {
			setSubWidget(data);
		}
		// T O D O: Change this with a throw.a..a
		return true;
	}
	
	@Override
	public void markToRedraw() {
		
		if (this.subWidget != null) {
			this.subWidget.calculateMinMaxSize();
			return;
		}
		super.markToRedraw();
	}
	
	protected void onChangePropertySubFile() {
		Log.info("Load compositing form external file : " + this.propertySubFile);
		if (this.propertySubFile.isEmpty()) {
			// remove all elements:
			subWidgetRemove();
			return;
		}
		if (!loadFromFile(this.propertySubFile)) {
			Log.error("Can not load Player GUI from file ... " + this.propertySubFile);
		}
	}
	
	@Override
	public void onChangeSize() {
		if (this.subWidget != null) {
			this.subWidget.onChangeSize();
			return;
		}
		super.onChangeSize();
	}
	
	@Override
	public void onEventClipboard(final ClipboardList clipboardID) {
		if (this.subWidget != null) {
			this.subWidget.onEventClipboard(clipboardID);
			return;
		}
		super.onEventClipboard(clipboardID);
	}
	
	@Override
	public boolean onEventShortCut(final KeySpecial special, final Character unicodeValue, final KeyKeyboard kbMove, final boolean isDown) {
		if (this.subWidget != null) {
			return this.subWidget.onEventShortCut(special, unicodeValue, kbMove, isDown);
		}
		return super.onEventShortCut(special, unicodeValue, kbMove, isDown);
	}
	
	@Override
	public void onRegenerateDisplay() {
		if (this.subWidget != null) {
			this.subWidget.onRegenerateDisplay();
			return;
		}
		super.onRegenerateDisplay();
	}
	
	@Override
	public Vector3f relativePosition(final Vector3f pos) {
		if (this.subWidget != null) {
			return this.subWidget.relativePosition(pos);
		}
		return super.relativePosition(pos);
	}
	
	@Override
	public void requestDestroyFromChild(final EwolObject child) {
		super.requestDestroyFromChild(child);
		if (this.propertyRemoveIfUnderRemove) {
			Log.debug("Child widget remove ==> auto-remove");
			autoDestroy();
		}
	}
	
	@Override
	public void requestUpdateSize() {
		if (this.subWidget != null) {
			this.subWidget.requestUpdateSize();
			return;
		}
		super.requestUpdateSize();
	}
	
	@Override
	public boolean rmFocus() {
		if (this.subWidget != null) {
			return this.subWidget.rmFocus();
		}
		return super.rmFocus();
	}
	
	@Override
	public void setCursor(final Cursor newCursor) {
		if (this.subWidget != null) {
			this.subWidget.setCursor(newCursor);
			return;
		}
		super.setCursor(newCursor);
	}
	
	@Override
	public boolean setFocus() {
		if (this.subWidget != null) {
			return this.subWidget.setFocus();
		}
		return super.setFocus();
	}
	
	@Override
	public void setMouseLimit(final int numberState) {
		if (this.subWidget != null) {
			this.subWidget.setMouseLimit(numberState);
			return;
		}
		super.setMouseLimit(numberState);
	}
	
	@Override
	public void setNoMaxSize() {
		if (this.subWidget != null) {
			this.subWidget.setNoMaxSize();
			return;
		}
		super.setNoMaxSize();
	}
	
	@Override
	public void setNoMinSize() {
		if (this.subWidget != null) {
			this.subWidget.setNoMinSize();
			return;
		}
		super.setNoMinSize();
	}
	
	@Override
	public void setOffset(final Vector3f newVal) {
		if (this.subWidget != null) {
			this.subWidget.setOffset(newVal);
			return;
		}
		super.setOffset(newVal);
	}
	
	@Override
	public void setOrigin(final Vector3f pos) {
		if (this.subWidget != null) {
			this.subWidget.setOrigin(pos);
			return;
		}
		super.setOrigin(pos);
	}
	
	@Override
	public void setPropertyCanFocus(final boolean canFocus) {
		if (this.subWidget != null) {
			this.subWidget.setPropertyCanFocus(canFocus);
			return;
		}
		super.setPropertyCanFocus(canFocus);
	}
	
	@Override
	public void setPropertyExpand(final Vector3b value) {
		if (this.subWidget != null) {
			this.subWidget.setPropertyExpand(value);
			return;
		}
		super.setPropertyExpand(value);
	}
	
	@Override
	public void setPropertyExpandIfFree(final Vector3b value) {
		if (this.subWidget != null) {
			this.subWidget.setPropertyExpandIfFree(value);
			return;
		}
		super.setPropertyExpandIfFree(value);
	}
	
	@Override
	public void setPropertyFill(final Vector3b value) {
		if (this.subWidget != null) {
			this.subWidget.setPropertyFill(value);
			return;
		}
		super.setPropertyFill(value);
	}
	
	@Override
	public void setPropertyGravity(final Gravity gravity) {
		if (this.subWidget != null) {
			this.subWidget.setPropertyGravity(gravity);
			return;
		}
		super.setPropertyGravity(gravity);
	}
	
	@Override
	public void setPropertyHide(final boolean value) {
		if (this.subWidget != null) {
			this.subWidget.setPropertyHide(value);
			return;
		}
		super.setPropertyHide(value);
	}
	
	@Override
	public void setPropertyMaxSize(final Dimension3f value) {
		if (this.subWidget != null) {
			this.subWidget.setPropertyMaxSize(value);
			return;
		}
		super.setPropertyMaxSize(value);
	}
	
	@Override
	public void setPropertyMinSize(final Dimension3f value) {
		if (this.subWidget != null) {
			this.subWidget.setPropertyMinSize(value);
			return;
		}
		super.setPropertyMinSize(value);
	}
	
	public void setPropertyRemoveIfUnderRemove(final boolean propertyRemoveIfUnderRemove) {
		if (this.propertyRemoveIfUnderRemove == propertyRemoveIfUnderRemove) {
			return;
		}
		this.propertyRemoveIfUnderRemove = propertyRemoveIfUnderRemove;
	}
	
	public void setPropertySubFile(final Uri propertySubFile) {
		if (this.propertySubFile.equals(propertySubFile)) {
			return;
		}
		this.propertySubFile = propertySubFile;
		onChangePropertySubFile();
	}
	
	@Override
	public void setSize(final Vector3f value) {
		if (this.subWidget != null) {
			this.subWidget.setSize(value);
			return;
		}
		super.setSize(value);
	}
	
	@Override
	public void setZoom(final float newVal) {
		if (this.subWidget != null) {
			this.subWidget.setZoom(newVal);
			return;
		}
		super.setZoom(newVal);
	}
	
	@Override
	public void systemDraw(final DrawProperty displayProp) {
		if (this.subWidget != null) {
			this.subWidget.systemDraw(displayProp);
			return;
		}
		super.systemDraw(displayProp);
	}
	
	@Override
	public void unGrabCursor() {
		if (this.subWidget != null) {
			this.subWidget.unGrabCursor();
			return;
		}
		super.unGrabCursor();
	}
	
}

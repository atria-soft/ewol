/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import java.util.ArrayList;
import java.util.List;
import java.util.ListIterator;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.etk.math.Vector3i;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.compositing.CompositingDrawing;
import org.atriasoft.ewol.context.EwolContext;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.object.EwolObject;
import org.atriasoft.ewol.resource.ResourceColorFile;
import org.atriasoft.gale.backend3d.OpenGL;

/**
 * Windows basic interface
 */
public class Windows extends Widget {
	
	protected int colorBg = -1; //!< Default background color of the windows
	
	protected List<Widget> popUpWidgetList = new ArrayList<>();
	
	@AknotManaged
	@AknotAttribute
	@AknotName("file-color")
	@AknotDescription("File color of the Windows")
	public Uri propertyColorConfiguration = new Uri("THEME", "color/Windows.json", "ewol"); //!< Configuration file of the windows theme
	@AknotManaged
	@AknotAttribute
	@AknotName("title")
	@AknotDescription("Title of the windows")
	public String propertyTitle = "No title"; //!< Current title of the windows
	
	protected ResourceColorFile resourceColor = null; //!< theme color property (name of file in @ref propertyColorConfiguration)
	
	protected Widget subWidget;
	
	// internal event at ewol system:
	
	protected CompositingDrawing vectorialDraw = new CompositingDrawing();
	
	protected Windows() {
		this.propertyCanFocus = true;
		onChangePropertyColor();
	}
	
	//!< List of pop-up displayed
	@Override
	public void drawWidgetTree(int level) {
		super.drawWidgetTree(level);
		level++;
		if (this.subWidget != null) {
			this.subWidget.drawWidgetTree(level);
		}
		for (final Widget it : this.popUpWidgetList) {
			if (it != null) {
				it.drawWidgetTree(level);
			}
		}
	}
	
	public Uri getPropertyColorConfiguration() {
		return this.propertyColorConfiguration;
	}
	
	public String getPropertyTitle() {
		return this.propertyTitle;
	}
	
	@Override
	public EwolObject getSubObjectNamed(final String objectName) {
		EwolObject tmpObject = super.getSubObjectNamed(objectName);
		if (tmpObject != null) {
			return tmpObject;
		}
		// check direct subwidget
		if (this.subWidget != null) {
			tmpObject = this.subWidget.getSubObjectNamed(objectName);
			if (tmpObject != null) {
				return tmpObject;
			}
		}
		// get all subwidget "pop-up"
		for (final Widget it : this.popUpWidgetList) {
			if (it != null) {
				tmpObject = it.getSubObjectNamed(objectName);
				if (tmpObject != null) {
					return tmpObject;
				}
			}
		}
		// not find ...
		return null;
	}
	
	@Override
	public Widget getWidgetAtPos(final Vector3f pos) {
		Log.verbose("Get widget at pos : " + pos);
		// calculate relative position
		final Vector3f relativePos = relativePosition(pos);
		// event go directly on the pop-up
		if (this.popUpWidgetList.size() != 0) {
			return this.popUpWidgetList.get(this.popUpWidgetList.size() - 1).getWidgetAtPos(pos);
			// otherwise in the normal windows
		}
		if (this.subWidget != null) {
			return this.subWidget.getWidgetAtPos(pos);
		}
		// otherwise the event go to this widget ...
		return this;
	}
	
	protected void onChangePropertyColor() {
		this.resourceColor = ResourceColorFile.create(this.propertyColorConfiguration);
		if (this.resourceColor != null) {
			this.colorBg = this.resourceColor.request("background");
		} else {
			Log.warning("Can not open the default color configuration file for the windows: " + this.propertyColorConfiguration);
		}
	}
	
	@Override
	public void onChangeSize() {
		super.onChangeSize();
		if (this.subWidget != null) {
			this.subWidget.calculateMinMaxSize();
			// TODO : do it better ... and manage gravity ...
			this.subWidget.setSize(this.size);
			this.subWidget.setOrigin(Vector3f.ZERO);
			this.subWidget.onChangeSize();
		}
		for (final Widget it : this.popUpWidgetList) {
			if (it != null) {
				it.calculateMinMaxSize();
				it.setSize(this.size);
				it.setOrigin(Vector3f.ZERO);
				it.onChangeSize();
			}
		}
	}
	
	@Override
	public void onRegenerateDisplay() {
		if (this.subWidget != null) {
			this.subWidget.systemRegenerateDisplay();
		}
		for (final Widget it : this.popUpWidgetList) {
			if (it != null) {
				it.systemRegenerateDisplay();
			}
		}
	}
	
	/**
	 * Get the number of pop-up
	 * @return Count of pop-up
	 */
	public int popUpCount() {
		return this.popUpWidgetList.size();
	}
	
	/**
	 * Remove the pop-up on top.
	 */
	public void popUpWidgetPop() {
		if (this.popUpWidgetList.size() == 0) {
			return;
		}
		this.popUpWidgetList.remove(this.popUpWidgetList.size() - 1);
	}
	
	/**
	 * Add a pop-up on the Windows.
	 * @param widget Widget to set on top of the pop-up.
	 */
	public void popUpWidgetPush(final Widget widget) {
		if (widget == null) {
			// nothing to do an error appear :
			Log.error("can not set widget pop-up (null pointer)");
			return;
		}
		this.popUpWidgetList.add(widget);
		widget.setParent(this);
		// force the focus on the basic widget ==> this remove many time the virual keyboard area
		widget.keepFocus();
		// Regenerate the size calculation :
		onChangeSize();
		// TODO : it is dangerous to access directly to the system ...
		EwolObject.getContext().resetIOEvent();
	}
	
	//!< main sub-widget of the Windows.
	@Override
	public void requestDestroyFromChild(final EwolObject child) {
		Log.verbose("A child has been removed");
		ListIterator<Widget> it = this.popUpWidgetList.listIterator();
		while (it.hasNext()) {
			final Widget elem = it.next();
			if (elem == child) {
				Log.verbose("    Find it ...");
				if (elem != null) {
					elem.removeParent();
				}
				it.remove();
				it = this.popUpWidgetList.listIterator();
				markToRedraw();
			}
		}
		if (this.subWidget == child) {
			Log.verbose("    Find it ... 2");
			if (this.subWidget == null) {
				return;
			}
			this.subWidget.removeParent();
			this.subWidget = null;
			markToRedraw();
		}
	}
	
	public void setPropertyColorConfiguration(final Uri propertyColorConfiguration) {
		if (this.propertyColorConfiguration.equals(propertyColorConfiguration)) {
			return;
		}
		this.propertyColorConfiguration = propertyColorConfiguration;
		onChangePropertyColor();
	}
	
	public void setPropertyTitle(final String propertyTitle) {
		if (this.propertyTitle.contentEquals(propertyTitle)) {
			return;
		}
		this.propertyTitle = propertyTitle;
		final EwolContext context = EwolObject.getContext();
		if (context.getWindows() == this) {
			context.setTitle(propertyTitle);
		} else {
			Log.info("Set title is delayed ...");
		}
	}
	
	/**
	 * Set the main widget of the application.
	 * @param widget Widget to set in the windows.
	 */
	public void setSubWidget(final Widget widget) {
		if (this.subWidget != null) {
			Log.info("Remove current main windows Widget...");
			this.subWidget.removeParent();
			this.subWidget = null;
		}
		if (widget != null) {
			this.subWidget = widget;
			this.subWidget.setParent(this);
		}
		// Regenerate the size calculation :
		onChangeSize();
	}
	
	public void sysDraw() {
		//Log.verbose("Draw on " + this.size);
		// set the size of the open GL system
		OpenGL.setViewPort(Vector3f.ZERO, this.size);
		OpenGL.disable(OpenGL.Flag.flag_dither);
		//OpenGL.disable(OpenGL.Flag.flagblend);
		OpenGL.disable(OpenGL.Flag.flag_stencilTest);
		OpenGL.disable(OpenGL.Flag.flag_alphaTest);
		OpenGL.disable(OpenGL.Flag.flag_fog);
		OpenGL.disable(OpenGL.Flag.flag_texture2D);
		OpenGL.disable(OpenGL.Flag.flag_depthTest);
		OpenGL.disable(OpenGL.Flag.flag_cullFace);
		
		OpenGL.enable(OpenGL.Flag.flag_blend);
		//OpenGL.enable(OpenGL.Flag.flag_cullFace);
		OpenGL.blendFuncAuto();
		
		// clear the matrix system :
		OpenGL.setBasicMatrix(Matrix4f.IDENTITY);
		final Vector3i tmpSize = new Vector3i((int) this.size.x(), (int) this.size.y(), (int) this.size.z());
		final DrawProperty displayProp = new DrawProperty(tmpSize, Vector3i.ZERO, tmpSize);
		systemDraw(displayProp);
		OpenGL.disable(OpenGL.Flag.flag_blend);
	}
	
	@Override
	public void systemDraw(final DrawProperty displayProp) {
		super.systemDraw(displayProp);
		// clear the screen with transparency ...
		Color colorBg = Color.GRAY;
		if (this.resourceColor != null) {
			colorBg = this.resourceColor.get(this.colorBg);
		}
		OpenGL.clearColor(colorBg);
		OpenGL.clearColor(Color.PURPLE);
		OpenGL.clear(OpenGL.ClearFlag.clearFlag_colorBuffer);
		OpenGL.clear(OpenGL.ClearFlag.clearFlag_depthBuffer);
		
		//Log.warning(" WINDOWS draw on " + this.currentDrawId);
		// first display the windows on the display
		if (this.subWidget != null) {
			this.subWidget.systemDraw(displayProp);
			//Log.debug("Draw Windows");
		}
		
		// second display the pop-up
		for (final Widget it : this.popUpWidgetList) {
			if (it != null) {
				it.systemDraw(displayProp);
				//Log.debug("Draw Pop-up");
			}
		}
	}
}

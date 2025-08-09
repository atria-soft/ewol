/** @file
 * @author Edouard DUPIN
 * @copyright 2020, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.GravityVertical;
import org.atriasoft.ewol.HighSpeedMode;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.compositing.CompositingSVG;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.gale.key.KeyStatus;
import org.atriasoft.gale.key.KeyType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

class Scroll extends Container {
	private static final Logger LOGGER = LoggerFactory.getLogger(Scroll.class);
	protected static final int SCROLL_BAR_SPACE = 15;
	protected Vector2f propertyLimit = new Vector2f(0.15f, 0.5f); //!< Set the limitation of the ratio in the screen
	
	protected Uri propertyShapeVert = new Uri("THEME_GUI", "WidgetScrolled.json", "ewol"); //!< Vertical shaper name
	
	protected Uri propertyShapeHori = new Uri("THEME_GUI", "WidgetScrolled.json", "ewol"); //!< Horizontal shaper name
	
	protected boolean propertyHover = true; //!< Horizontal shaper name
	
	protected CompositingSVG compositingH = new CompositingSVG();
	protected CompositingSVG compositingV = new CompositingSVG();
	protected float pixelScrolling = 20;
	protected Vector2f highSpeedStartPos = Vector2f.ZERO;
	protected HighSpeedMode highSpeedMode = HighSpeedMode.speedModeDisable;
	protected int highSpeedButton = -1;
	protected KeyType highSpeedType = KeyType.unknow;
	
	public Scroll() {
		onChangePropertyShapeVert();
		onChangePropertyShapeHori();
	}
	
	@Override
	public void calculateMinMaxSize() {
		// Note: No call of container ==> normal case ...
		super.calculateMinMaxSize();
		// call sub classes
		if (this.subWidget != null) {
			this.subWidget.calculateMinMaxSize();
		}
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "limit")
	@AknotDescription(value = "Limit the scroll maximum position [0..1]% represent the free space in the scoll when arrive at the end")
	public Vector2f getPropertyLimit() {
		return this.propertyLimit;
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "shape-hori")
	@AknotDescription(value = "shape for the horizontal display")
	public Uri getPropertyShapeHori() {
		return this.propertyShapeHori;
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "shape-vert")
	@AknotDescription(value = "shape for the vertical display")
	public Uri getPropertyShapeVert() {
		return this.propertyShapeVert;
	}
	
	@Override
	public Widget getWidgetAtPos(final Vector2f pos) {
		final Widget tmpWidget = super.getWidgetAtPos(pos);
		if (tmpWidget != null) {
			return tmpWidget;
		}
		return this;
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName(value = "hover")
	@AknotDescription(value = "The display bar are hover the subWidget")
	public boolean isPropertyHover() {
		return this.propertyHover;
	}
	
	void onChangePropertyLimit() {
		markToRedraw();
	}
	
	protected void onChangePropertyShapeHori() {
		//TODO: this.shaperH.setSource(this.propertyShapeHori);
		markToRedraw();
	}
	
	protected void onChangePropertyShapeVert() {
		//TODO: this.shaperV.setSource(this.propertyShapeVert);
		markToRedraw();
	}
	
	@Override
	public void onChangeSize() {
		// Note: No call of container ==> normal case ...
		super.onChangeSize();
		if (this.propertyHide) {
			return;
		}
		if (this.subWidget == null) {
			return;
		}
		// remove the bar if hover
		Vector2f basicSize = this.size;
		if (!this.propertyHover) {
			basicSize = basicSize.less(SCROLL_BAR_SPACE, SCROLL_BAR_SPACE);
		}
		
		Vector2f origin = this.origin.add(this.offset);
		Vector2f minSize = this.subWidget.getCalculateMinSize();
		final Vector2b expand = this.subWidget.propertyExpand;
		//The gravity is not set on the sub element ==> special use of the widget
		//origin += ewol::gravityGenerateDelta(propertyGravity.get(), minSize - this.size);
		if (expand.x() && minSize.x() < basicSize.x()) {
			minSize = minSize.withX(basicSize.x());
		}
		if (expand.y() && minSize.y() < basicSize.y()) {
			minSize = minSize.withY(basicSize.y());
		}
		this.subWidget.setSize(minSize);
		if (this.propertyGravity.y() == GravityVertical.TOP) {
			origin = origin.add(0.0f, basicSize.y() - minSize.y());
			if (!this.propertyHover) {
				origin = origin.add(0, SCROLL_BAR_SPACE);
			}
		} else if (this.propertyGravity.y() == GravityVertical.BOTTOM) {
			// nothing to do ... origin +=
		} else {
			LOGGER.error(" Not manage other gravity ...");
		}
		this.subWidget.setOrigin(origin);
		this.subWidget.onChangeSize();
	}
	
	@Override
	protected void onDraw() {
		this.compositingH.draw();
		this.compositingV.draw();
	}
	
	@Override
	public boolean onEventInput(final EventInput event) {
		//ewol::event::Input _event = event;
		//_event.setType(KeyType.finger);
		Vector2f relativePos = relativePosition(new Vector2f(event.pos().x(), event.pos().y()));
		Vector2f scrollOffset = Vector2f.ZERO;
		Vector2f scrollSize = Vector2f.ZERO;
		if (this.subWidget != null) {
			scrollOffset = this.subWidget.getOffset();
			scrollSize = this.subWidget.getSize();
		}
		LOGGER.trace("Get Event on scroll : " + event);
		relativePos = relativePos.withY(this.size.y() - relativePos.y());
		if (event.type() == KeyType.mouse
				&& (this.highSpeedType == KeyType.unknow || this.highSpeedType == KeyType.mouse)) {
			if (event.inputId() == 1 && event.status() == KeyStatus.down) {
				// check if selected the scrolling position whth the scrolling bar ...
				if (relativePos.x() >= (this.size.x() - SCROLL_BAR_SPACE)) {
					if (this.size.y() < scrollSize.y() || scrollOffset.y() != 0) {
						this.highSpeedMode = HighSpeedMode.speedModeEnableVertical;
						this.highSpeedType = KeyType.mouse;
						this.highSpeedStartPos = this.highSpeedStartPos.withX(relativePos.x());
						this.highSpeedStartPos = this.highSpeedStartPos
								.withY(scrollOffset.y() / scrollSize.y() * (this.size.y() - SCROLL_BAR_SPACE * 2));
						this.highSpeedButton = 1;
						// force direct scrolling in this case
						scrollOffset = scrollOffset.withY((int) (scrollSize.y() * (relativePos.y() - SCROLL_BAR_SPACE)
								/ (this.size.y() - SCROLL_BAR_SPACE * 2)));
						scrollOffset = scrollOffset.withY(FMath.avg(0.0f, scrollOffset.y(),
								(scrollSize.y() - this.size.y() * this.propertyLimit.y())));
						markToRedraw();
						if (this.subWidget != null) {
							this.subWidget.setOffset(scrollOffset);
						}
						return true;
					}
				} else if (relativePos.y() >= (this.size.y() - SCROLL_BAR_SPACE)) {
					if (this.size.x() < scrollSize.x() || scrollOffset.x() != 0) {
						this.highSpeedMode = HighSpeedMode.speedModeEnableHorizontal;
						this.highSpeedType = KeyType.mouse;
						this.highSpeedStartPos = this.highSpeedStartPos
								.withX(scrollOffset.x() / scrollSize.x() * (this.size.x() - SCROLL_BAR_SPACE * 2));
						this.highSpeedStartPos = this.highSpeedStartPos.withY(relativePos.y());
						this.highSpeedButton = 1;
						// force direct scrolling in this case
						scrollOffset = scrollOffset.withX((int) (scrollSize.x() * (relativePos.x() - SCROLL_BAR_SPACE)
								/ (this.size.x() - SCROLL_BAR_SPACE * 2)));
						scrollOffset = scrollOffset.withY(FMath.avg(0.0f, scrollOffset.x(),
								(scrollSize.x() - this.size.x() * this.propertyLimit.x())));
						markToRedraw();
						if (this.subWidget != null) {
							this.subWidget.setOffset(scrollOffset);
						}
						return true;
					}
				}
				return false;
			} else if (event.inputId() == 4 && event.status() == KeyStatus.up) {
				LOGGER.trace("    mode UP " + this.size.y() + "<" + scrollSize.y());
				if (this.size.y() < scrollSize.y()) {
					scrollOffset = scrollOffset.withY(scrollOffset.y() - this.pixelScrolling);
					scrollOffset = scrollOffset.withY(FMath.avg(0.0f, scrollOffset.y(),
							(scrollSize.y() - this.size.y() * this.propertyLimit.y())));
					markToRedraw();
					if (this.subWidget != null) {
						this.subWidget.setOffset(scrollOffset);
					}
					return true;
				}
			} else if (event.inputId() == 5 && event.status() == KeyStatus.up) {
				LOGGER.trace("    mode DOWN " + this.size.y() + "<" + scrollSize.y());
				if (this.size.y() < scrollSize.y()) {
					scrollOffset = scrollOffset.withY(scrollOffset.y() + this.pixelScrolling);
					scrollOffset = scrollOffset.withY(FMath.avg(0.0f, scrollOffset.y(),
							(scrollSize.y() - this.size.y() * this.propertyLimit.y())));
					markToRedraw();
					if (this.subWidget != null) {
						this.subWidget.setOffset(scrollOffset);
					}
					return true;
				}
			} else if (event.inputId() == 2) {
				if (event.status() == KeyStatus.down) {
					this.highSpeedMode = HighSpeedMode.speedModeInit;
					this.highSpeedType = KeyType.mouse;
					this.highSpeedStartPos = new Vector2f(relativePos.x(), relativePos.y());
					this.highSpeedButton = 2;
					// not really use...  == > just keep some informations
					return false;
				}
			} else if (this.highSpeedMode != HighSpeedMode.speedModeDisable && event.status() == KeyStatus.leave) {
				this.highSpeedMode = HighSpeedMode.speedModeDisable;
				this.highSpeedType = KeyType.unknow;
				markToRedraw();
				return true;
			}
			if (event.inputId() == this.highSpeedButton && this.highSpeedMode != HighSpeedMode.speedModeDisable) {
				if (event.status() == KeyStatus.up) {
					if (this.highSpeedMode == HighSpeedMode.speedModeInit) {
						// TODO : generate back the down event ...
						this.highSpeedMode = HighSpeedMode.speedModeDisable;
						this.highSpeedType = KeyType.unknow;
						return false;
					} else {
						this.highSpeedMode = HighSpeedMode.speedModeGrepEndEvent;
						markToRedraw();
						return true;
					}
				} else if (this.highSpeedMode == HighSpeedMode.speedModeGrepEndEvent) {
					if (event.status() == KeyStatus.pressSingle) {
						this.highSpeedMode = HighSpeedMode.speedModeDisable;
						this.highSpeedType = KeyType.unknow;
						this.highSpeedButton = -1;
						markToRedraw();
					}
					return true;
				} else if (this.highSpeedMode == HighSpeedMode.speedModeInit && event.status() == KeyStatus.move) {
					// wait that the cursor move more than 10 px to enable it :
					if (FMath.abs(relativePos.x() - this.highSpeedStartPos.x()) > 10
							|| FMath.abs(relativePos.y() - this.highSpeedStartPos.y()) > 10) {
						// the scrooling can start :
						// select the direction :
						if (relativePos.x() == this.highSpeedStartPos.x()) {
							this.highSpeedMode = HighSpeedMode.speedModeEnableVertical;
						} else if (relativePos.y() == this.highSpeedStartPos.y()) {
							this.highSpeedMode = HighSpeedMode.speedModeEnableHorizontal;
						} else {
							final float coef = (relativePos.y() - this.highSpeedStartPos.y())
									/ (relativePos.x() - this.highSpeedStartPos.x());
							if (FMath.abs(coef) <= 1) {
								this.highSpeedMode = HighSpeedMode.speedModeEnableHorizontal;
							} else {
								this.highSpeedMode = HighSpeedMode.speedModeEnableVertical;
							}
						}
						if (this.highSpeedMode == HighSpeedMode.speedModeEnableHorizontal) {
							this.highSpeedStartPos = this.highSpeedStartPos
									.withX(scrollOffset.x() / scrollSize.x() * (this.size.x() - SCROLL_BAR_SPACE * 2));
						} else {
							this.highSpeedStartPos = this.highSpeedStartPos
									.withY(scrollOffset.y() / scrollSize.y() * (this.size.y() - SCROLL_BAR_SPACE * 2));
						}
						markToRedraw();
					}
					scrollOffset = scrollOffset.withY(FMath.avg(0.0f, scrollOffset.y(),
							(scrollSize.y() - this.size.y() * this.propertyLimit.y())));
					if (this.subWidget != null) {
						this.subWidget.setOffset(scrollOffset);
					}
					return true;
				}
				if (this.highSpeedMode == HighSpeedMode.speedModeEnableHorizontal && event.status() == KeyStatus.move) {
					scrollOffset = scrollOffset.withX((int) (scrollSize.x() * (relativePos.x() - SCROLL_BAR_SPACE)
							/ (this.size.x() - SCROLL_BAR_SPACE * 2)));
					scrollOffset = scrollOffset.withX(FMath.avg(0.0f, scrollOffset.x(),
							(scrollSize.x() - this.size.x() * this.propertyLimit.x())));
					markToRedraw();
					if (this.subWidget != null) {
						this.subWidget.setOffset(scrollOffset);
					}
					return true;
				}
				if (this.highSpeedMode == HighSpeedMode.speedModeEnableVertical && event.status() == KeyStatus.move) {
					scrollOffset = scrollOffset.withY((int) (scrollSize.y() * (relativePos.y() - SCROLL_BAR_SPACE)
							/ (this.size.y() - SCROLL_BAR_SPACE * 2)));
					scrollOffset = scrollOffset.withY(FMath.avg(0.0f, scrollOffset.y(),
							(scrollSize.y() - this.size.y() * this.propertyLimit.x())));
					markToRedraw();
					if (this.subWidget != null) {
						this.subWidget.setOffset(scrollOffset);
					}
					return true;
				}
			}
		} else if (KeyType.finger == event.type()
				&& (KeyType.unknow == this.highSpeedType || KeyType.finger == this.highSpeedType)) {
			if (1 == event.inputId()) {
				LOGGER.trace("event: " + event);
				if (KeyStatus.down == event.status()) {
					this.highSpeedMode = HighSpeedMode.speedModeInit;
					this.highSpeedType = KeyType.finger;
					this.highSpeedStartPos = new Vector2f(relativePos.x(), relativePos.y());
					LOGGER.trace("SCROOL  == > INIT pos=" + this.highSpeedStartPos + " && curent scrollOffset="
							+ scrollOffset);
					return true;
				} else if (KeyStatus.upAfter == event.status()) {
					this.highSpeedMode = HighSpeedMode.speedModeDisable;
					this.highSpeedType = KeyType.unknow;
					LOGGER.trace("SCROOL  == > DISABLE");
					markToRedraw();
					return true;
				} else if (this.highSpeedMode == HighSpeedMode.speedModeInit && KeyStatus.move == event.status()) {
					// wait that the cursor move more than 10 px to enable it :
					if (FMath.abs(relativePos.x() - this.highSpeedStartPos.x()) > 10
							|| FMath.abs(relativePos.y() - this.highSpeedStartPos.y()) > 10) {
						// the scrooling can start :
						// select the direction :
						this.highSpeedMode = HighSpeedMode.speedModeEnableFinger;
						LOGGER.trace("SCROOL  == > ENABLE");
						markToRedraw();
					}
					return true;
				}
				if (this.highSpeedMode == HighSpeedMode.speedModeEnableFinger && KeyStatus.move == event.status()) {
					LOGGER.trace("SCROOL  == > INIT scrollOffset=" + scrollOffset.y() + " relativePos="
							+ relativePos.y() + " this.highSpeedStartPos=" + this.highSpeedStartPos.y());
					//scrollOffset.x = (int)(scrollSize.x * x / this.size.x);
					if (this.propertyLimit.x() != 0.0f) {
						scrollOffset = scrollOffset
								.withX(scrollOffset.x() + (relativePos.x() - this.highSpeedStartPos.x()));
						scrollOffset = scrollOffset.withX(FMath.avg(0.0f, scrollOffset.x(),
								(scrollSize.x() - this.size.x() * this.propertyLimit.x())));
					}
					if (this.propertyLimit.y() != 0.0f) {
						scrollOffset = scrollOffset
								.withY(scrollOffset.y() - (relativePos.y() - this.highSpeedStartPos.y()));
						scrollOffset = scrollOffset.withY(FMath.avg(0.0f, scrollOffset.y(),
								(scrollSize.y() - this.size.y() * this.propertyLimit.y())));
					}
					// update current position:
					this.highSpeedStartPos = relativePos;
					LOGGER.trace("SCROOL  == > MOVE " + scrollOffset);
					markToRedraw();
					if (this.subWidget != null) {
						this.subWidget.setOffset(scrollOffset);
					}
					return true;
				}
				if (this.highSpeedMode == HighSpeedMode.speedModeEnableFinger) {
					return true;
				}
			} else if (this.highSpeedMode != HighSpeedMode.speedModeDisable && KeyStatus.leave == event.status()) {
				this.highSpeedMode = HighSpeedMode.speedModeDisable;
				this.highSpeedType = KeyType.unknow;
				LOGGER.trace("SCROOL  == > DISABLE");
				markToRedraw();
				return true;
			}
		}
		return false;
	}
	
	@Override
	public void onRegenerateDisplay() {
		if (this.propertyHide) {
			return;
		}
		// call upper class
		super.onRegenerateDisplay();
		if (!needRedraw()) {
			return;
		}
		// clear all previous display
		this.compositingH.clear();
		this.compositingV.clear();
		final Padding paddingVert = new Padding(2, 2, 2, 2); // this.compositingV.getPadding();
		final Padding paddingHori = new Padding(2, 2, 2, 2); // this.compositingH.getPadding();
		Vector2f scrollOffset = Vector2f.ZERO;
		Vector2f scrollSize = Vector2f.ZERO;
		if (this.subWidget != null) {
			scrollOffset = this.subWidget.getOffset();
			scrollSize = this.subWidget.getSize();
		}
		if (this.size.y() < scrollSize.y() || scrollOffset.y() != 0) {
			float lenScrollBar = this.size.y() * this.size.y() / scrollSize.y();
			lenScrollBar = FMath.avg(10.0f, lenScrollBar, this.size.y());
			float originScrollBar = scrollOffset.y() / (scrollSize.y() - this.size.y() * this.propertyLimit.y());
			originScrollBar = FMath.avg(0.0f, originScrollBar, 1.0f);
			originScrollBar *= (this.size.y() - lenScrollBar);

			final Vector2f renderOrigin = new Vector2f(this.size.x() - paddingVert.x(), 0);
			final Vector2f renderSize = new Vector2f(paddingVert.x(), this.size.y());
			//			this.compositingV.setRectangleAsSource((int) renderSize.x(), (int) renderSize.y(), Color.GREEN);
			//			this.compositingV.setPos(renderOrigin);
			//			this.compositingV.print(renderSize);
			//			this.compositingV.flush();
			/*
			this.shaperV.setShape(new Vector2f(this.size.x() - paddingVert.x(), 0),
					new Vector2f(paddingVert.x(), this.size.y()),
					new Vector2f(this.size.x() - paddingVert.right(), this.size.y() - originScrollBar - lenScrollBar),
					new Vector2f(0, lenScrollBar));
			*/
		}
		if (this.size.x() < scrollSize.x() || scrollOffset.x() != 0) {
			float lenScrollBar = (this.size.x() - paddingHori.left()) * (this.size.x() - paddingVert.x())
					/ scrollSize.x();
			lenScrollBar = FMath.avg(10.0f, lenScrollBar, (this.size.x() - paddingVert.x()));
			float originScrollBar = scrollOffset.x() / (scrollSize.x() - this.size.x() * this.propertyLimit.x());
			originScrollBar = FMath.avg(0.0f, originScrollBar, 1.0f);
			originScrollBar *= (this.size.x() - paddingHori.right() - lenScrollBar);

			final Vector2f renderOrigin = Vector2f.ZERO;
			final Vector2f renderSize = new Vector2f(this.size.x() - paddingVert.x(), paddingHori.y());
			//			this.compositingH.setRectangleAsSource((int) renderSize.x(), (int) renderSize.y(), Color.GREEN);
			//			this.compositingH.setPos(renderOrigin);
			//			this.compositingH.print(renderSize);
			//			this.compositingH.flush();
			/*
			this.shaperH.setShape(Vector2f.ZERO, new Vector2f(this.size.x() - paddingVert.x(), paddingHori.y()),
					new Vector2f(originScrollBar, paddingHori.bottom()), new Vector2f(lenScrollBar, 0));
			*/
		}
	}
	
	public void setPropertyHover(final boolean propertyHover) {
		if (propertyHover == this.propertyHover) {
			return;
		}
		this.propertyHover = propertyHover;
	}
	
	public void setPropertyLimit(final Vector2f propertyLimit) {
		final Vector2f tmp = Vector2f.avg(Vector2f.ZERO, propertyLimit, Vector2f.ONE);
		if (tmp.equals(this.propertyLimit)) {
			return;
		}
		this.propertyLimit = propertyLimit;
		onChangePropertyLimit();
	}
	
	public void setPropertyShapeHori(final Uri value) {
		if (this.propertyShapeHori.equals(value)) {
			return;
		}
		this.propertyShapeHori = value;
		onChangePropertyShapeHori();
	}
	
	public void setPropertyShapeVert(final Uri value) {
		if (this.propertyShapeVert.equals(value)) {
			return;
		}
		this.propertyShapeVert = value;
		onChangePropertyShapeVert();
	}
	
	@Override
	public void systemDraw(final DrawProperty displayProp) {
		if (this.propertyHide) {
			return;
		}
		if (this.subWidget != null) {
			final DrawProperty prop = displayProp.withLimit(this.origin, this.size);
			this.subWidget.systemDraw(prop);
		}
		super.systemDraw(displayProp);
	}
}

package org.atriasoft.ewol.widget;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.HighSpeedMode;
import org.atriasoft.ewol.Padding;
import org.atriasoft.ewol.compositing.GuiShape;
import org.atriasoft.ewol.event.EventInput;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.gale.backend3d.OpenGL;
import org.atriasoft.gale.key.KeyStatus;
import org.atriasoft.gale.key.KeyType;

/**
 * Widget to integrate a scrool bar in a widget. This is not a stadalone widget.
 */
class WidgetScrolled extends Widget {
	public enum ScrollingMode {
		scroolModeNormal, //!< No Zoom , can UP and down, left and right
		scroolModeCenter, //!< Zoom enable, no move left and right
		scroolModeGame, //!< Zoom enable, no move left and right
	}
	
	public static final int CALCULATE_SIMULTANEOUS_FINGER = 5;
	protected Uri propertyShapeVert = new Uri("THEME", "shape/WidgetScrolled.json", "ewol"); //!< Vertical shaper name
	protected Uri propertyShapeHori = new Uri("THEME", "shape/WidgetScrolled.json", "ewol"); //!< Horizontal shaper name
	private GuiShape shaperH = null; //!< Compositing theme Horizontal.
	private GuiShape shaperV = null; //!< Compositing theme Vertical.
	protected Vector2f originScrooled = Vector2f.ZERO; //!< pixel distance from the origin of the display (Bottum left)
	protected Vector2f maxSize; //!< Maximum size of the Widget ==> to display scrollbar
	protected Vector2f limitScrolling = Vector2f.ZERO; //!< Mimit scrolling represent the propertion of the minimel scrolling activate (0.2 ==> 20% migt all time be visible)
	// Mouse section :
	private ScrollingMode scroollingMode = ScrollingMode.scroolModeNormal; //!< mode of management of the scrooling
	private float pixelScrolling = 20;
	private Vector2f highSpeedStartPos;
	private HighSpeedMode highSpeedMode = HighSpeedMode.speedModeDisable;
	private int highSpeedButton = -1;
	private KeyType highSpeedType = KeyType.unknow;
	// finger section:
	private boolean singleFingerMode = true; //!< in many case the moving in a subwidget is done with one finger, it is enought ==> the user select...
	private final boolean[] fingerPresent = { false, false, false, false, false };
	private boolean fingerScoolActivated = false;
	private final Vector2f[] fingerMoveStartPos = new Vector2f[CALCULATE_SIMULTANEOUS_FINGER];
	
	/**
	 * Scroll Widget main constructor to be inherited from an other widget (this is not a stand-alone widget)
	 * @param _shaperName Shaper name if the scrolled widget.
	 */
	public WidgetScrolled() {
		onChangePropertyShapeVert();
		onChangePropertyShapeHori();
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName("shape-hori")
	@AknotDescription("shape for the horizontal display")
	public Uri getPropertyShapeHori() {
		return this.propertyShapeHori;
	}
	
	@AknotManaged
	@AknotAttribute
	@AknotName("shape-vert")
	@AknotDescription("shape for the vertical display")
	public Uri getPropertyShapeVert() {
		return this.propertyShapeVert;
	}
	
	/**
	 * Get the single finger capabilities
	 * @return true The single finger mode is active
	 * @return false The To finger mode is active
	 */
	public boolean getSingleFinger() {
		return this.singleFingerMode;
	}
	
	protected void onChangePropertyShapeHori() {
		if (this.shaperH == null) {
			this.shaperH = new GuiShape(this.propertyShapeHori);
		} else {
			this.shaperH.setSource(this.propertyShapeHori);
		}
		markToRedraw();
	}
	
	protected void onChangePropertyShapeVert() {
		if (this.shaperV == null) {
			this.shaperV = new GuiShape(this.propertyShapeVert);
		} else {
			this.shaperV.setSource(this.propertyShapeVert);
		}
		markToRedraw();
	}
	
	@Override
	protected void onDraw() {
		this.shaperH.draw();
		this.shaperV.draw();
	}
	
	@Override
	public boolean onEventInput(final EventInput event) {
		Log.verbose("event XXX {}", event);
		Vector3f relativePos = relativePosition(new Vector3f(event.pos().x(), event.pos().y(), 0.0f));
		// Correction due to the open Gl insertion ...
		relativePos = relativePos.withY(this.size.y() - relativePos.y());
		final Padding paddingV = this.shaperV.getPadding();
		final Padding paddingH = this.shaperH.getPadding();
		if (this.scroollingMode == ScrollingMode.scroolModeNormal) {
			if (event.type() == KeyType.mouse && (this.highSpeedType == KeyType.unknow || this.highSpeedType == KeyType.mouse)) {
				if (event.inputId() == 1 && event.status() == KeyStatus.down) {
					// check if selected the scrolling position with the scrolling bar ...
					if (relativePos.x() >= (this.size.x() - paddingV.x())) {
						if (this.size.y() < this.maxSize.y() || this.originScrooled.y() != 0) {
							this.highSpeedMode = HighSpeedMode.speedModeEnableVertical;
							this.highSpeedType = KeyType.mouse;
							this.highSpeedStartPos = this.highSpeedStartPos.withX(relativePos.x());
							this.highSpeedStartPos = this.highSpeedStartPos.withY(this.originScrooled.y() / this.maxSize.y() * (this.size.y() - paddingV.y()));
							this.highSpeedButton = 1;
							// force direct scrolling in this case
							this.originScrooled = this.originScrooled.withY((int) (this.maxSize.y() * (relativePos.y() - paddingV.bottom()) / (this.size.y() - paddingV.bottom() * 2)));
							this.originScrooled = this.originScrooled.withY(FMath.avg(0.0f, this.originScrooled.y(), (this.maxSize.y() - this.size.y() * this.limitScrolling.y())));
							markToRedraw();
							return true;
						}
					} else if (relativePos.y() >= (this.size.y() - paddingH.y())) {
						if (this.size.x() < this.maxSize.x() || this.originScrooled.x() != 0) {
							this.highSpeedMode = HighSpeedMode.speedModeEnableHorizontal;
							this.highSpeedType = KeyType.mouse;
							this.highSpeedStartPos = this.highSpeedStartPos.withX(this.originScrooled.x() / this.maxSize.x() * (this.size.x() - paddingH.x()));
							this.highSpeedStartPos = this.highSpeedStartPos.withY(relativePos.y());
							this.highSpeedButton = 1;
							// force direct scrolling in this case
							this.originScrooled = this.originScrooled.withX((int) (this.maxSize.x() * (relativePos.x() - paddingH.left()) / (this.size.x() - paddingH.left() * 2)));
							this.originScrooled = this.originScrooled.withX(FMath.avg(0.0f, this.originScrooled.x(), (this.maxSize.x() - this.size.x() * this.limitScrolling.x())));
							markToRedraw();
							return true;
						}
					}
					return false;
				} else if (event.inputId() == 4 && event.status() == KeyStatus.up) {
					if (event.specialKey().getCtrl()) {
						changeZoom(1);
						/*
						float zoom = getZoom()*1.1;
						zoom = FMath.avg(0.1f, zoom, 5000.0f);
						setZoom(zoom);
						*/
					} else {
						if (this.size.y() < this.maxSize.y() || this.originScrooled.y() != 0 || this.size.y() * this.limitScrolling.y() < this.maxSize.y()) {
							this.originScrooled = this.originScrooled.withY(this.originScrooled.y() - this.pixelScrolling);
							this.originScrooled = this.originScrooled.withY(FMath.avg(0.0f, this.originScrooled.y(), (this.maxSize.y() - this.size.y() * this.limitScrolling.y())));
							markToRedraw();
							return true;
						}
					}
				} else if (event.inputId() == 5 && event.status() == KeyStatus.up) {
					if (event.specialKey().getCtrl()) {
						changeZoom(-1);
						/*
						float zoom = getZoom()*0.9;
						zoom = FMath.avg(0.1f, zoom, 5000.0f);
						setZoom(zoom);
						*/
					} else {
						if (this.size.y() < this.maxSize.y() || this.originScrooled.y() != 0 || this.size.y() * this.limitScrolling.y() < this.maxSize.y()) {
							this.originScrooled = this.originScrooled.withY(this.originScrooled.y() + this.pixelScrolling);
							this.originScrooled = this.originScrooled.withY(FMath.avg(0.0f, this.originScrooled.y(), (this.maxSize.y() - this.size.y() * this.limitScrolling.y())));
							markToRedraw();
							return true;
						}
					}
				} else if (event.inputId() == 11 && event.status() == KeyStatus.up) {
					// Scrool Left
					if (this.size.x() < this.maxSize.x() || this.originScrooled.x() != 0 || this.size.x() * this.limitScrolling.x() < this.maxSize.x()) {
						this.originScrooled = this.originScrooled.withX(this.originScrooled.x() - this.pixelScrolling);
						this.originScrooled = this.originScrooled.withX(FMath.avg(0.0f, this.originScrooled.x(), (this.maxSize.x() - this.size.x() * this.limitScrolling.x())));
						markToRedraw();
						return true;
					}
				} else if (event.inputId() == 10 && event.status() == KeyStatus.up) {
					// Scrool Right
					if (this.size.x() < this.maxSize.x() || this.originScrooled.x() != 0 || this.size.x() * this.limitScrolling.x() < this.maxSize.x()) {
						this.originScrooled = this.originScrooled.withX(this.originScrooled.x() + this.pixelScrolling);
						this.originScrooled = this.originScrooled.withX(FMath.avg(0.0f, this.originScrooled.x(), (this.maxSize.x() - this.size.x() * this.limitScrolling.x())));
						markToRedraw();
						return true;
					}
				} else if (event.inputId() == 2) {
					/*
					if (true == ewol::isSetCtrl()) {
						if (KeyStatus.down == typeEvent) {
							float zoom = 1.0;
							setZoom(zoom);
						}
					} else */ {
						if (event.status() == KeyStatus.down) {
							this.highSpeedMode = HighSpeedMode.speedModeInit;
							this.highSpeedType = KeyType.mouse;
							this.highSpeedStartPos = new Vector2f(relativePos.x(), relativePos.y());
							this.highSpeedButton = 2;
							return true;
						}
					}
				} else if (this.highSpeedMode != HighSpeedMode.speedModeDisable && event.status() == KeyStatus.leave) {
					this.highSpeedMode = HighSpeedMode.speedModeDisable;
					this.highSpeedType = KeyType.unknow;
					markToRedraw();
					return true;
				}
				if (event.inputId() == this.highSpeedButton && this.highSpeedMode != HighSpeedMode.speedModeDisable) {
					if (event.status() == KeyStatus.upAfter) {
						this.highSpeedMode = HighSpeedMode.speedModeDisable;
						this.highSpeedType = KeyType.unknow;
						return false;
					} else if (this.highSpeedMode == HighSpeedMode.speedModeGrepEndEvent) {
						if (event.status() == KeyStatus.pressSingle) {
							this.highSpeedMode = HighSpeedMode.speedModeDisable;
							this.highSpeedType = KeyType.unknow;
							this.highSpeedButton = -1;
							markToRedraw();
						}
						return true;
					} else if (event.status() == KeyStatus.up) {
						return true;
					} else if (this.highSpeedMode == HighSpeedMode.speedModeInit && event.status() == KeyStatus.move) {
						// wait that the cursor move more than 10 px to enable it :
						if (FMath.abs(relativePos.x() - this.highSpeedStartPos.x()) > 10 || FMath.abs(relativePos.y() - this.highSpeedStartPos.y()) > 10) {
							// the scrolling can start :
							// select the direction :
							if (relativePos.x() == this.highSpeedStartPos.x()) {
								this.highSpeedMode = HighSpeedMode.speedModeEnableVertical;
							} else if (relativePos.y() == this.highSpeedStartPos.y()) {
								this.highSpeedMode = HighSpeedMode.speedModeEnableHorizontal;
							} else {
								final float coef = (relativePos.y() - this.highSpeedStartPos.y()) / (relativePos.x() - this.highSpeedStartPos.x());
								if (FMath.abs(coef) <= 1) {
									this.highSpeedMode = HighSpeedMode.speedModeEnableHorizontal;
								} else {
									this.highSpeedMode = HighSpeedMode.speedModeEnableVertical;
								}
							}
							if (this.highSpeedMode == HighSpeedMode.speedModeEnableHorizontal) {
								this.highSpeedStartPos = this.highSpeedStartPos.withX(this.originScrooled.x() / this.maxSize.x() * (this.size.x() - paddingV.x()));
							} else {
								this.highSpeedStartPos = this.highSpeedStartPos.withY(this.originScrooled.y() / this.maxSize.y() * (this.size.y() - paddingV.y()));
							}
							markToRedraw();
						}
						this.originScrooled = this.originScrooled.withY(FMath.avg(0.0f, this.originScrooled.y(), (this.maxSize.y() - this.size.y() * this.limitScrolling.y())));
						return true;
					}
					if (this.highSpeedMode == HighSpeedMode.speedModeEnableHorizontal && event.status() == KeyStatus.move) {
						this.originScrooled = this.originScrooled.withX((int) (this.maxSize.x() * (relativePos.x() - paddingH.left()) / (this.size.x() - paddingH.x())));
						this.originScrooled = this.originScrooled.withX(FMath.avg(0.0f, this.originScrooled.x(), (this.maxSize.x() - this.size.x() * this.limitScrolling.x())));
						markToRedraw();
						return true;
					}
					if (this.highSpeedMode == HighSpeedMode.speedModeEnableVertical && event.status() == KeyStatus.move) {
						this.originScrooled = this.originScrooled.withY((int) (this.maxSize.y() * (relativePos.y() - paddingV.bottom()) / (this.size.y() - paddingV.y())));
						this.originScrooled = this.originScrooled.withY(FMath.avg(0.0f, this.originScrooled.y(), (this.maxSize.y() - this.size.y() * this.limitScrolling.y())));
						markToRedraw();
						return true;
					}
				}
			} else if (event.type() == KeyType.finger && (this.highSpeedType == KeyType.unknow || this.highSpeedType == KeyType.finger)) {
				if (!this.singleFingerMode) {
					// ***********************
					// ** Two finger mode : **
					// ***********************
					if (event.inputId() >= 3) {
						return false;
					}
					final int idTable = event.inputId() - 1;
					if (event.status() == KeyStatus.down) {
						this.fingerPresent[idTable] = true;
					} else if (event.status() == KeyStatus.upAfter) {
						this.fingerPresent[idTable] = false;
					}
					if (!this.fingerScoolActivated) {
						this.fingerMoveStartPos[idTable] = new Vector2f(relativePos.x(), relativePos.y());
					}
					if (this.fingerPresent[0] && this.fingerPresent[1] && !this.fingerScoolActivated) {
						this.fingerScoolActivated = true;
						Log.verbose("SCROOL  == > START pos=" + this.fingerMoveStartPos);
					}
					if (this.fingerScoolActivated) {
						// 1: HighSpeedMode...
						// 2: remove all unneeded sub event ... ==> maybe a better methode ...
						if (event.status() == KeyStatus.move) {
							this.originScrooled = this.originScrooled.withX(this.originScrooled.x() - (relativePos.x() - this.fingerMoveStartPos[idTable].x()) * 0.5f);
							this.originScrooled = this.originScrooled.withY(this.originScrooled.y() - (relativePos.y() - this.fingerMoveStartPos[idTable].y()) * 0.5f);
							this.originScrooled = this.originScrooled.withX(FMath.avg(0.0f, this.originScrooled.x(), (this.maxSize.x() - this.size.x() * this.limitScrolling.x())));
							this.originScrooled = this.originScrooled.withY(FMath.avg(0.0f, this.originScrooled.y(), (this.maxSize.y() - this.size.y() * this.limitScrolling.y())));
							this.fingerMoveStartPos[idTable] = new Vector2f(relativePos.x(), relativePos.y());
							Log.verbose("SCROOL  == > MOVE this.originScrooled=" + this.originScrooled + " " + relativePos + " " + this.highSpeedStartPos);
							markToRedraw();
						}
						if (!this.fingerPresent[0] && !this.fingerPresent[1]) {
							if (event.status() == KeyStatus.upAfter) {
								// TODO : Reset event ...
								this.fingerScoolActivated = false;
								// WTF ??? _event.reset();
							}
						}
						return true;
					}
				} else {
					// **************************
					// ** Single finger mode : **
					// **************************
					if (event.inputId() == 1) {
						Log.verbose("event 1  " + event);
						if (event.status() == KeyStatus.down) {
							this.highSpeedMode = HighSpeedMode.speedModeInit;
							this.highSpeedType = KeyType.finger;
							this.highSpeedStartPos = new Vector2f(relativePos.x(), relativePos.y());
							Log.verbose("SCROOL  == > INIT");
							return true;
						} else if (event.status() == KeyStatus.upAfter) {
							this.highSpeedMode = HighSpeedMode.speedModeDisable;
							this.highSpeedType = KeyType.unknow;
							Log.verbose("SCROOL  == > DISABLE");
							markToRedraw();
							return true;
						} else if (this.highSpeedMode == HighSpeedMode.speedModeInit && event.status() == KeyStatus.move) {
							// wait that the cursor move more than 10 px to enable it :
							if (FMath.abs(relativePos.x() - this.highSpeedStartPos.x()) > 10 || FMath.abs(relativePos.y() - this.highSpeedStartPos.y()) > 10) {
								// the scrooling can start :
								// select the direction :
								this.highSpeedMode = HighSpeedMode.speedModeEnableFinger;
								Log.debug("SCROOL  == > ENABLE");
								markToRedraw();
							}
							return true;
						} else if (this.highSpeedMode == HighSpeedMode.speedModeEnableFinger && event.status() == KeyStatus.pressSingle) {
							// Keep all event in the range of moving
							return true;
						} else if (this.highSpeedMode == HighSpeedMode.speedModeEnableFinger && event.status() == KeyStatus.pressDouble) {
							// Keep all event in the range of moving
							return true;
						}
						if (this.highSpeedMode == HighSpeedMode.speedModeEnableFinger && event.status() == KeyStatus.move) {
							//this.originScrooled.x = (int)(this.maxSize.x * x / this.size.x);
							this.originScrooled = this.originScrooled.withX(this.originScrooled.x() - (relativePos.x() - this.highSpeedStartPos.x()));
							this.originScrooled = this.originScrooled.withY(this.originScrooled.y() - (relativePos.y() - this.highSpeedStartPos.y()));
							this.originScrooled = this.originScrooled.withX(FMath.avg(0.0f, this.originScrooled.x(), (this.maxSize.x() - this.size.x() * this.limitScrolling.x())));
							this.originScrooled = this.originScrooled.withY(FMath.avg(0.0f, this.originScrooled.y(), (this.maxSize.y() - this.size.y() * this.limitScrolling.y())));
							this.highSpeedStartPos = new Vector2f(relativePos.x(), relativePos.y());
							Log.verbose("SCROOL  == > MOVE this.originScrooled=" + this.originScrooled + " " + relativePos + " " + this.highSpeedStartPos);
							markToRedraw();
							return true;
						}
					} else if (this.highSpeedMode == HighSpeedMode.speedModeDisable && event.status() == KeyStatus.leave) {
						this.highSpeedMode = HighSpeedMode.speedModeDisable;
						this.highSpeedType = KeyType.unknow;
						Log.verbose("SCROOL  == > DISABLE");
						markToRedraw();
						return true;
					}
				}
			}
		} else if (this.scroollingMode == ScrollingMode.scroolModeCenter) {
			if (event.type() == KeyType.mouse) {
				float tmp1 = this.size.x() / this.maxSize.y();
				final float tmp2 = this.size.y() / this.maxSize.x();
				//Log.info(" elements Zoom : " + tmp1 + " " + tmp2);
				tmp1 = FMath.min(tmp1, tmp2);
				if (event.inputId() == 4 && event.status() == KeyStatus.up) {
					this.zoom -= 0.1;
					if (tmp1 < 1.0) {
						this.zoom = FMath.max(tmp1, this.zoom);
					} else {
						this.zoom = FMath.max(1.0f, this.zoom);
					}
					markToRedraw();
					return true;
				} else if (event.inputId() == 5 && event.status() == KeyStatus.up) {
					this.zoom += 0.1;
					if (tmp1 > 1.0) {
						this.zoom = FMath.min(tmp1, this.zoom);
					} else {
						this.zoom = FMath.min(1.0f, this.zoom);
					}
					markToRedraw();
					return true;
				}
			}
		} else if (this.scroollingMode == ScrollingMode.scroolModeGame) {
			
		} else {
			Log.error("Scrolling mode unknow ... " + this.scroollingMode);
		}
		return false;
	}
	
	@Override
	public void onRegenerateDisplay() {
		this.shaperH.clear();
		this.shaperV.clear();
		if (this.scroollingMode == ScrollingMode.scroolModeGame) {
			// nothing to do ...
			return;
		}
		final Padding paddingVert = this.shaperV.getPadding();
		final Padding paddingHori = this.shaperH.getPadding();
		if (this.size.y() < this.maxSize.y() || this.originScrooled.y() != 0) {
			float lenScrollBar = this.size.y() * this.size.y() / this.maxSize.y();
			lenScrollBar = FMath.avg(10.0f, lenScrollBar, this.size.y());
			float originScrollBar = this.originScrooled.y() / (this.maxSize.y() - this.size.y() * this.limitScrolling.y());
			originScrollBar = FMath.avg(0.0f, originScrollBar, 1.0f);
			originScrollBar *= (this.size.y() - lenScrollBar);
			this.shaperV.setShape(new Vector2f(this.size.x() - paddingVert.x(), 0), new Vector2f(paddingVert.x(), this.size.y()),
					new Vector2f(this.size.x() - paddingVert.right(), this.size.y() - originScrollBar - lenScrollBar), new Vector2f(0, lenScrollBar));
		}
		if (this.size.x() < this.maxSize.x() || this.originScrooled.x() != 0) {
			float lenScrollBar = (this.size.x() - paddingHori.left()) * (this.size.x() - paddingVert.x()) / this.maxSize.x();
			lenScrollBar = FMath.avg(10.0f, lenScrollBar, (this.size.x() - paddingVert.x()));
			float originScrollBar = this.originScrooled.x() / (this.maxSize.x() - this.size.x() * this.limitScrolling.x());
			originScrollBar = FMath.avg(0.0f, originScrollBar, 1.0f);
			originScrollBar *= (this.size.x() - paddingHori.right() - lenScrollBar);
			this.shaperH.setShape(new Vector2f(0, 0), new Vector2f(this.size.x() - paddingVert.x(), paddingHori.y()), new Vector2f(originScrollBar, paddingHori.bottom()),
					new Vector2f(lenScrollBar, 0));
		}
	}
	
	/**
	 * Reset the scoll of the subWidget
	 */
	public void resetScrollOrigin() {
		this.originScrooled = new Vector2f(0, 0);
	}
	
	/**
	 * Specify the mode of scrolling for this windows
	 * @param newMode the selected mode for the scrolling...
	 */
	protected void scroolingMode(final ScrollingMode newMode) {
		this.scroollingMode = newMode;
		if (this.scroollingMode == ScrollingMode.scroolModeGame) {
			// set the scene maximum size :
			this.maxSize = new Vector2f(FMath.max(this.size.x(), this.size.y()), this.maxSize.x());
			this.zoom = 1;
		}
	}
	
	/**
	 * set the scrolling limit when arriving at he end of the widget
	 * @param poucentageLimit pourcent of the limit of view nothing in the widget when arriving at the end ...
	 */
	protected void setLimitScrolling(float poucentageLimit) {
		poucentageLimit = FMath.avg(0.1f, poucentageLimit, 1.0f);
		this.limitScrolling = new Vector2f(poucentageLimit, poucentageLimit);
	}
	
	/**
	 * set the scrolling limit when arriving at he end of the widget
	 * @param poucentageLimit pourcent of the limit of view nothing in the widget when arriving at the end for axis specific...
	 */
	protected void setLimitScrolling(final Vector2f poucentageLimit) {
		this.limitScrolling = new Vector2f(FMath.avg(0.1f, poucentageLimit.x(), 1.0f), FMath.avg(0.1f, poucentageLimit.y(), 1.0f));
	}
	
	/**
	 * set the specific mawimum size of the widget
	 * @param localSize new Maximum size
	 */
	protected void setMaxSize(final Vector2f localSize) {
		this.maxSize = localSize;
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
	
	/**
		 * Request a specific position for the scrolling of the current windows.
		 * @param borderWidth size of the border that requested the element might not to be
		 * @param currentPosition Position that is requested to view
		 * @param _center True if the position might be at the center of the widget
		 */
	protected void setScrollingPositionDynamic(final Vector2f borderWidth, final Vector2f currentPosition) {
		setScrollingPositionDynamic(borderWidth, currentPosition, false);
	}
	
	protected void setScrollingPositionDynamic(Vector2f borderWidth, final Vector2f currentPosition, final boolean center) {
		if (center) {
			borderWidth = new Vector2f(this.size.x() / 2 - borderWidth.x(), this.size.y() / 2 - borderWidth.y());
		}
		// check scrolling in X
		if (currentPosition.x() < (this.originScrooled.x() + borderWidth.x())) {
			this.originScrooled = this.originScrooled.withX(currentPosition.x() - borderWidth.x());
			this.originScrooled = this.originScrooled.withX(FMath.max(0.0f, this.originScrooled.x()));
		} else if (currentPosition.x() > (this.originScrooled.x() + this.size.x() - 2 * borderWidth.x())) {
			this.originScrooled = this.originScrooled.withX(currentPosition.x() - this.size.x() + 2 * borderWidth.x());
			this.originScrooled = this.originScrooled.withX(FMath.max(0.0f, this.originScrooled.x()));
		}
		// check scrolling in Y
		if (currentPosition.y() < (this.originScrooled.y() + borderWidth.y())) {
			this.originScrooled = this.originScrooled.withY(currentPosition.y() - borderWidth.y());
			this.originScrooled = this.originScrooled.withY(FMath.max(0.0f, this.originScrooled.y()));
		} else if (currentPosition.y() > (this.originScrooled.y() + this.size.y() - 2 * borderWidth.y())) {
			this.originScrooled = this.originScrooled.withY(currentPosition.y() - this.size.y() + 2 * borderWidth.y());
			this.originScrooled = this.originScrooled.withY(FMath.max(0.0f, this.originScrooled.y()));
		}
	}
	
	/**
	 * For mouse event when we have a scrolling UP and dows, specify the number of pixel that we scrooled
	 * @param nbPixel number of pixel scrolling
	 */
	protected void setScrollingSize(final float nbPixel) {
		this.pixelScrolling = nbPixel;
	}
	
	/**
	 * Set the single finger capabilities/
	 * @param status True if single inger mode, two otherwise/
	 */
	public void setSingleFinger(final boolean status) {
		if (this.singleFingerMode == status) {
			return;
		}
		this.singleFingerMode = status;
	}
	
	@Override
	public void systemDraw(final DrawProperty displayProp) {
		OpenGL.push();
		if (this.scroollingMode == ScrollingMode.scroolModeCenter) {
			// here we invert the reference of the standard openGl view because the reference in the common display is Top left and not buttom left
			OpenGL.setViewPort(this.origin, this.size);
			final Matrix4f tmpProjection = Matrix4f.createMatrixOrtho(-this.size.x() / 2, this.size.x() / 2, -this.size.y() / 2, this.size.y() / 2, -1, 1);
			final Matrix4f tmpScale = Matrix4f.createMatrixScale(new Vector3f(this.zoom, this.zoom, 1));
			final Matrix4f tmpTranslate = Matrix4f.createMatrixTranslate(new Vector3f(-this.maxSize.x() / 2, -this.maxSize.y() / 2, -1));
			final Matrix4f tmpMat = tmpProjection.multiply(tmpScale).multiply(tmpTranslate);
			// set internal matrix system :
			OpenGL.setMatrix(tmpMat);
			// Call the widget drawing methode
			onDraw();
		}
		if (this.scroollingMode == ScrollingMode.scroolModeGame) {
			// here we invert the reference of the standard openGl view because the reference in the common display is Top left and not buttom left
			OpenGL.setViewPort(this.origin, this.size);
			final Matrix4f tmpProjection = Matrix4f.createMatrixOrtho(-this.size.x() / 2, this.size.x() / 2, -this.size.y() / 2, this.size.y() / 2, -1, 1);
			final Matrix4f tmpTranslate = Matrix4f.createMatrixTranslate(new Vector3f(-this.maxSize.x() / 2, -this.maxSize.y() / 2, -1));
			final Matrix4f tmpMat = tmpProjection.multiply(tmpTranslate);
			// set internal matrix system :
			OpenGL.setMatrix(tmpMat);
			// Call the widget drawing methode
			onDraw();
		} else {
			super.systemDraw(displayProp);
		}
		OpenGL.pop();
	}
}

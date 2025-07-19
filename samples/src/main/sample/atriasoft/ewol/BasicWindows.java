package sample.atriasoft.ewol;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.etk.Dimension2f;
import org.atriasoft.etk.Distance;
import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.widget.Button;
import org.atriasoft.ewol.widget.Container;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.Windows;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class BasicWindows extends Windows {
	private static final Logger LOGGER = LoggerFactory.getLogger(BasicWindows.class);

	private int index = -1;
	private final List<TestWidgetInterface> testedElement = new ArrayList<>();
	private Container container = null;
	private Label title = null;

	public static void staticRequestNext(final BasicWindows self) {
		self.requestNext();
	}

	public static void staticRequestPrevious(final BasicWindows self) {
		self.requestPrevious();
	}

	public void requestNext() {
		LOGGER.info("Request Next");
		this.index++;
		if (this.index >= this.testedElement.size()) {
			this.index = 0;
		}
		updateDisplay();
	}

	public void requestPrevious() {
		LOGGER.info("Request Previous");
		this.index--;
		if (this.index < 0) {
			this.index = this.testedElement.size() - 1;
		}
		updateDisplay();
	}

	public void updateDisplay() {
		final var test = this.testedElement.get(this.index);
		final var titlegenerated = "<b>[" + (this.index + 1) + "/" + this.testedElement.size() + "] " + test.getTitle()
				+ "</b>";
		this.title.setPropertyValue(titlegenerated);
		setPropertyTitle(titlegenerated);
		this.container.setSubWidget(new ModelWidget(test));
	}

	public BasicWindows() {

		final var sizerMain = new Sizer(DisplayMode.VERTICAL);
		sizerMain.setPropertyExpand(Vector2b.TRUE);
		sizerMain.setPropertyFill(Vector2b.TRUE);
		setSubWidget(sizerMain);

		final var menu = new Sizer(DisplayMode.HORIZONTAL);
		menu.setPropertyExpand(Vector2b.TRUE_FALSE);
		menu.setPropertyExpandIfFree(Vector2b.TRUE_FALSE);
		menu.setPropertyFill(Vector2b.TRUE);
		menu.setPropertyLockExpand(Vector2b.TRUE);
		menu.setPropertyMaxSize(new Dimension2f(new Vector2f(9999, 3), Distance.CENTIMETER));
		sizerMain.subWidgetAdd(menu);

		this.container = new Container();
		this.container.setPropertyExpand(Vector2b.TRUE);
		this.container.setPropertyFill(Vector2b.TRUE);
		sizerMain.subWidgetAdd(this.container);

		final var next = Button.createLabelButton("&lt;&lt; Previous");
		next.setPropertyMaxSize(new Dimension2f(new Vector2f(9999, 2), Distance.CENTIMETER));
		menu.subWidgetAdd(next);
		next.signalClick.connectAuto(this, BasicWindows::staticRequestNext);

		this.title = new Label("unknown");
		this.title.setPropertyFill(Vector2b.FALSE);
		this.title.setPropertyExpand(Vector2b.TRUE);
		menu.subWidgetAdd(this.title);

		final var previous = Button.createLabelButton("Next &gt;&gt;");
		previous.setPropertyMaxSize(new Dimension2f(new Vector2f(9999, 2), Distance.CENTIMETER));
		menu.subWidgetAdd(previous);
		previous.signalClick.connectAuto(this, BasicWindows::staticRequestPrevious);

		this.container.setPropertyExpand(Vector2b.TRUE);
		this.container.setPropertyFill(Vector2b.TRUE);
		this.container.setPropertyExpandIfFree(Vector2b.TRUE);

		this.testedElement.add(new TestWidgetEntry());
		this.testedElement.add(new TestWidgetBox());
		this.testedElement.add(new TestWidgetSlider());
		this.testedElement.add(new TestWidgetButton());
		this.testedElement.add(new TestWidgetButtonToggle());
		this.testedElement.add(new TestWidgetCheckBox());
		this.testedElement.add(new TestWidgetImage());
		this.testedElement.add(new TestWidgetLabel());
		requestNext();

	}
}

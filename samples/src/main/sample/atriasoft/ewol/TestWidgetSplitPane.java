package sample.atriasoft.ewol;

import org.atriasoft.etk.Color;
import org.atriasoft.ewol.widget.Box;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.SplitPane;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetSplitPane implements TestWidgetInterface {
	
	@Override
	public Widget getWidget() {
		// Create left panel with colored box and label
		final var leftBox = new Box();
		leftBox.setPropertyColor(Color.LIGHT_BLUE);
		leftBox.expand(true, true).fill(true, true);
		
		final var leftContent = Sizer.vertical().add(Label.create("Left Panel")).add(leftBox).expand(true, true)
				.fill(true, true);
		
		// Create right panel with colored box and label
		final var rightBox = new Box();
		rightBox.setPropertyColor(Color.LIGHT_GREEN);
		rightBox.expand(true, true).fill(true, true);
		
		final var rightContent = Sizer.vertical().add(Label.create("Right Panel")).add(rightBox).expand(true, true)
				.fill(true, true);
		
		// Create horizontal split pane
		final var horizontalSplit = SplitPane.horizontal().splitPosition(0.4f).separatorSize(8).minSizes(80, 80)
				.first(leftContent).second(rightContent)
				.onSplitChange(ratio -> System.out.println("Horizontal split ratio: " + ratio)).expand(true, true)
				.fill(true, true);
		
		// Create top panel for vertical split
		final var topBox = new Box();
		topBox.setPropertyColor(Color.LIGHT_CORAL);
		topBox.expand(true, true).fill(true, true);
		
		final var topContent = Sizer.vertical().add(Label.create("Top Panel")).add(topBox).expand(true, true).fill(true,
				true);
		
		// Create main vertical split pane containing horizontal split at bottom
		final var mainSplit = SplitPane.vertical().splitPosition(0.3f).separatorSize(8).minSizes(60, 100)
				.first(topContent).second(horizontalSplit)
				.onSplitChange(ratio -> System.out.println("Vertical split ratio: " + ratio)).expand(true, true)
				.fill(true, true);
		
		return mainSplit;
	}
	
	@Override
	public String getTitle() {
		return "SplitPane";
	}
}

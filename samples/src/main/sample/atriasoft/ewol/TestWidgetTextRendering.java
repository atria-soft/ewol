package sample.atriasoft.ewol;

import org.atriasoft.etk.math.Vector2b;
import org.atriasoft.ewol.widget.Label;
import org.atriasoft.ewol.widget.ScrollView;
import org.atriasoft.ewol.widget.Sizer;
import org.atriasoft.ewol.widget.Sizer.DisplayMode;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetTextRendering implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		final Sizer content = Sizer.vertical();
		content.setPropertyExpand(Vector2b.TRUE);
		content.setPropertyFill(Vector2b.TRUE);

		final int[] sizes = {8, 10, 12, 14, 16, 18, 20, 24, 28, 32, 40, 48};

		for (final int size : sizes) {
			// Regular
			final Label regular = new Label("Regular " + size + "px: The quick brown fox jumps over the lazy dog");
			regular.setPropertyFontSize(size);
			regular.setPropertyExpand(Vector2b.TRUE_FALSE);
			regular.setPropertyFill(Vector2b.TRUE);
			content.subWidgetAdd(regular);

			// Bold
			final Label bold = new Label("<b>Bold " + size + "px: The quick brown fox jumps over the lazy dog</b>");
			bold.setPropertyFontSize(size);
			bold.setPropertyExpand(Vector2b.TRUE_FALSE);
			bold.setPropertyFill(Vector2b.TRUE);
			content.subWidgetAdd(bold);

			// Italic
			final Label italic = new Label("<i>Italic " + size + "px: The quick brown fox jumps over the lazy dog</i>");
			italic.setPropertyFontSize(size);
			italic.setPropertyExpand(Vector2b.TRUE_FALSE);
			italic.setPropertyFill(Vector2b.TRUE);
			content.subWidgetAdd(italic);

			// Bold + Italic
			final Label boldItalic = new Label("<b><i>Bold Italic " + size + "px: The quick brown fox</i></b>");
			boldItalic.setPropertyFontSize(size);
			boldItalic.setPropertyExpand(Vector2b.TRUE_FALSE);
			boldItalic.setPropertyFill(Vector2b.TRUE);
			content.subWidgetAdd(boldItalic);
		}

		final ScrollView scrollView = ScrollView.create()
				.content(content)
				.showVertical(true)
				.showHorizontal(false);
		scrollView.setPropertyExpand(Vector2b.TRUE);
		scrollView.setPropertyFill(Vector2b.TRUE);
		return scrollView;
	}

	@Override
	public String getTitle() {
		return "Text Rendering";
	}

	@Override
	public String getDescription() {
		return "Font rendering at various sizes with bold, italic and bold-italic";
	}

	@Override
	public String getCategory() {
		return "Basic";
	}
}

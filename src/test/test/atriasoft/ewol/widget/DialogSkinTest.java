package test.atriasoft.ewol.widget;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertSame;

import org.atriasoft.ewol.widget.Dialog;
import org.atriasoft.ewol.widget.DialogSkin;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import test.atriasoft.ewol.EwolTestContext;

/** The dialogs follow the active theme: the default skin is given to every new dialog. */
class DialogSkinTest {
	@BeforeAll
	static void setUpClass() {
		EwolTestContext.init();
	}

	@AfterEach
	void restoreLightSkin() {
		Dialog.setDefaultSkin(DialogSkin.LIGHT);
	}

	@Test
	void aNewDialogIsLightByDefault() {
		assertSame(DialogSkin.LIGHT, Dialog.create().getSkin());
	}

	@Test
	void aNewDialogFollowsTheDefaultSkin() {
		Dialog.setDefaultSkin(DialogSkin.DARK);
		final Dialog dialog = Dialog.create();
		assertSame(DialogSkin.DARK, dialog.getSkin());
		assertEquals(DialogSkin.DARK.overlay(), dialog.getPropertyColor());
	}

	@Test
	void aDialogCanBeSkinnedOnItsOwn() {
		final Dialog dialog = Dialog.create().skin(DialogSkin.DARK);
		assertSame(DialogSkin.DARK, dialog.getSkin());
		assertSame(DialogSkin.LIGHT, Dialog.getDefaultSkin());
	}
}

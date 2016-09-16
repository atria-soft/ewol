/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license GPL v3 (see license file)
 */
//! [ewol_sample_HW_main_all]

//! [ewol_sample_HW_main_include]
#include <etk/types.h>
#include <ewol/ewol.h>
#include <gale/context/commandLine.h>
//! [ewol_sample_HW_main_include]


#include <appl/debug.h>
#include <appl/Windows.h>
#include <ewol/object/Object.h>
#include <ewol/widget/Manager.h>
#include <ewol/context/Context.h>

//! [ewol_sample_HW_main_application]
namespace appl {
	class MainApplication : public ewol::context::Application {
		public:
			//! [ewol_sample_HW_main_create]
			void onCreate(ewol::Context& _context) override {
				APPL_INFO("==> CREATE ... " PROJECT_NAME " (BEGIN)");
				localCreate(_context);
				APPL_INFO("==> CREATE ... " PROJECT_NAME " (END)");
			}
			//! [ewol_sample_HW_main_create]
			void onStart(ewol::Context& _context) override {
				APPL_INFO("==> START ... " PROJECT_NAME " (BEGIN)");
				// nothing to do ...
				APPL_INFO("==> START ... " PROJECT_NAME " (END)");
			}
			void onResume(ewol::Context& _context) override {
				APPL_INFO("==> RESUME ... " PROJECT_NAME " (BEGIN)");
				// nothing to do ...
				APPL_INFO("==> RESUME ... " PROJECT_NAME " (END)");
			}
			void onPause(ewol::Context& _context) override {
				APPL_INFO("==> PAUSE ... " PROJECT_NAME " (BEGIN)");
				// nothing to do ...
				APPL_INFO("==> PAUSE ... " PROJECT_NAME " (END)");
			}
			void onStop(ewol::Context& _context) override {
				APPL_INFO("==> STOP ... " PROJECT_NAME " (START)");
				// nothing to do ...
				APPL_INFO("==> STOP ... " PROJECT_NAME " (END)");
			}
			void onDestroy(ewol::Context& _context) override {
				APPL_INFO("==> DESTROY ... " PROJECT_NAME " (START)");
				// nothing to do ...
				APPL_INFO("==> DESTROY ... " PROJECT_NAME " (END)");
			}
			//! [ewol_sample_HW_main_application]
			void localCreate(ewol::Context& _context) {
				//! [ewol_sample_HW_main_parse_arguments]
				// parse all the argument of the application
				for (int32_t iii=0 ; iii<_context.getCmd().size(); iii++) {
					std::string tmpppp = _context.getCmd().get(iii);
					if (    tmpppp == "-h"
					     || tmpppp == "--help") {
						APPL_INFO("  -h/--help display this help" );
						exit(0);
					}
				}
				//! [ewol_sample_HW_main_parse_arguments]
				//! [ewol_sample_HW_main_set_windows_size]
				// TODO : Remove this: Move if in the windows properties
				_context.setSize(vec2(800, 600));
				//! [ewol_sample_HW_main_set_windows_size]
				//! [ewol_sample_HW_main_set_font_property]
				// eneble the search of the font in the system font path
				_context.getFontDefault().setUseExternal(true);
				// select font preference of der with a basic application size
				_context.getFontDefault().set("FreeSerif;DejaVuSansMono", 19);
				//! [ewol_sample_HW_main_set_font_property]
				//! [ewol_sample_HW_main_set_windows]
				// Create the windows
				ewol::widget::WindowsShared basicWindows = appl::Windows::create();
				// configure the ewol context to use the new windows
				_context.setWindows(basicWindows);
				//! [ewol_sample_HW_main_set_windows]
			}
	};
}

//! [ewol_sample_HW_main_main]
/**
 * @brief Main of the program (This can be set in every case, but it is not used in Andoid...).
 * @param std IO
 * @return std IO
 */
int main(int _argc, const char *_argv[]) {
	// second possibility
	return ewol::run(new appl::MainApplication(), _argc, _argv);
}
//! [ewol_sample_HW_main_main]

//! [ewol_sample_HW_main_all]


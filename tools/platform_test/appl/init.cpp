/** @file
 * @author Edouard DUPIN
 * @copyright 2010, Edouard DUPIN, all right reserved
 * @license BSD v3 (see license file)
 */

#include <etk/types.hpp>
#include <etk/types.hpp>
#include <ewol/ewol.hpp>
#include <ewol/object/Object.hpp>
#include <ewol/context/Context.hpp>
#include <ewol/widget/Manager.hpp>
#include <etk/theme/theme.hpp>

#include <appl/debug.hpp>
#include <appl/MainWindows.hpp>
#include <appl/widget/SizerColor.hpp>

namespace appl {
	class MainApplication : public ewol::context::Application {
		public:
			virtual void onCreate(ewol::Context& _context) {
				APPL_INFO(" == > CREATE ... (START) [" << gale::getBoardType() << "] (" << gale::getCompilationMode() << ") (BEGIN)");
				for( int32_t iii=0 ; iii<_context.getCmd().size(); iii++) {
					etk::String tmpppp = _context.getCmd().get(iii);
					if (    tmpppp == "-h"
					     || tmpppp == "--help") {
						APPL_INFO("  -t c-flags-file-name" );
						APPL_INFO("  -h/--help display this help" );
						exit(0);
					} 
				}
				//etk::theme::setName("COLOR", "color/black/");
				etk::theme::setName("COLOR", "color/white/");
				
				_context.setSize(vec2(800, 600));
				
				_context.setTitle("ewol-tools-visual-test");
				
				// select internal data for font ...
				_context.getFontDefault().setUseExternal(true);
				#ifdef __TARGET_OS__Android
					_context.getFontDefault().set("FreeSerif", 19);
				#else
					_context.getFontDefault().set("FreeSerif;DejaVuSansMono",14);
				#endif
				
				// set the application icon ...
				_context.setIcon("DATA:///icon.png");
				
				appl::widget::SizerColor::createManagerWidget(_context.getWidgetManager());
				
				APPL_INFO("==> CREATE ... (END)");
			}
			
			void onStart(ewol::Context& _context) {
				APPL_INFO("==> START ... (BEGIN)");
				
				ememory::SharedPtr<appl::MainWindows> basicWindows = appl::MainWindows::create();
				if (basicWindows == null) {
					APPL_ERROR("Can not allocate the basic windows");
					return;
				}
				// create the specific windows
				_context.setWindows(basicWindows);
				if (basicWindows == null) {
					APPL_ERROR("Can not allocate the basic windows");
					_context.exit(-1);
					return;
				}
				
				// create the specific windows
				_context.setWindows(basicWindows);
				
				APPL_INFO("==> START ... (END)");
				return;
			}
			
			void onStop(ewol::Context& _context) {
				APPL_INFO("==> STOP ... (START)");
				APPL_INFO("==> STOP ... (END)");
			}
	};
}


/**
 * @brief Main of the program (This can be set in every case, but it is not used in Andoid...).
 * @param std IO
 * @return std IO
 */
int main(int _argc, const char *_argv[]) {
	// second possibility
	return ewol::run(ETK_NEW(appl::MainApplication), _argc, _argv);
}



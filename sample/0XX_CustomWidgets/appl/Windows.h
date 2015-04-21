/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD 3 clauses (see license file)
 */

#ifndef __APPL_WINDOWS_H__
#define __APPL_WINDOWS_H__

#include <ewol/widget/Windows.h>
#include <ewol/widget/Composer.h>

namespace appl {
	class Windows : public ewol::widget::Windows {
		private:
			std::shared_ptr<ewol::widget::Composer> m_composer;
		protected:
			Windows();
			void init();
		public:
			DECLARE_FACTORY(Windows);
		public: // callback functions
			void onCallbackChangeValues();
			void onCallbackAutoMode();
	};
};


#endif
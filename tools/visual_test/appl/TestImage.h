/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __TEST_IMAGE_H__
#define __TEST_IMAGE_H__

#include <appl/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Image.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/Composer.h>

namespace appl {
	class TestImage : public ewol::widget::Composer {
		private:
			std::shared_ptr<ewol::widget::Image> m_testWidget;
		public:
			// Constructeur
			TestImage();
			void init();
		public:
			DECLARE_FACTORY(TestImage);
			virtual ~TestImage() {};
		protected:
			void onCallbackExpendX(const bool& _value);
			void onCallbackExpendY(const bool& _value);
			void onCallbackFillX(const bool& _value);
			void onCallbackFillY(const bool& _value);
			void onCallbackChangeImage();
	};
};

#endif

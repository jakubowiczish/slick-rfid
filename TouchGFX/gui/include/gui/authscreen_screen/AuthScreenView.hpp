#ifndef AUTHSCREENVIEW_HPP
#define AUTHSCREENVIEW_HPP

#include <gui_generated/authscreen_screen/AuthScreenViewBase.hpp>
#include <gui/authscreen_screen/AuthScreenPresenter.hpp>

class AuthScreenView : public AuthScreenViewBase {
	public:
		AuthScreenView();
		virtual ~AuthScreenView() {
		}
		virtual void setupScreen();
		virtual void tearDownScreen();

		void showUid(uint8_t uid_tab[]);

		void showAvatar(uint8_t id);

		void showBitmapAvatar(uint16_t bitmap_value);

		void showAvatarName(std::string state);

		void invalidateAvatar();

		void szczygiHandler();
		void zajmaHandler();
		void dybczakHandler();
		void plotnikHandler();
		void rafalHandler();
		void tomsiaHandler();
		void radojHandler();
		void capalaHandler();

		void confirmChoiceHandler();

		void showWaitingForCardText();

		void hideWaitingTextField();

		uint8_t uidTabBuffer[10];

		uint8_t bufferSize = 16;

		uint8_t buffer[16];

		uint8_t avatarId;

		uint8_t avatarBlockAddress = 1;

		uint8_t size = 30;
		uint8_t sak;

		uint8_t key_tab[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

		std::string avatarName;

		uint8_t clickedAvatarId;

		Unicode::UnicodeChar saveBuf[45];
};

#endif // AUTHSCREENVIEW_HPP

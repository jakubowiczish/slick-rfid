#include <gui/authscreen_screen/AuthScreenView.hpp>
#include "BitmapDatabase.hpp"
#include "term_io.h"
#include "rfid.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "usb_host.h"
#include "dbgu.h"
#include <stdio.h>
#include <sstream>
#include <string>

using namespace touchgfx;

AuthScreenView::AuthScreenView() {

}

void AuthScreenView::setupScreen() {
	AuthScreenViewBase::setupScreen();
}

void AuthScreenView::tearDownScreen() {
	AuthScreenViewBase::tearDownScreen();
}

void AuthScreenView::showUid(uint8_t uid_tab[]) {
	Unicode::snprintf(uidTextFieldBuffer, UIDTEXTFIELD_SIZE, "%d %d %d %d", uid_tab[0], uid_tab[1], uid_tab[2], uid_tab[3]);
	uidTextField.invalidate();
}

void AuthScreenView::showAvatar(uint8_t id) {
	if (id == 1) {
		showBitmapAvatar(BITMAP_SZCZYGI_ID);
	} else if (id == 2) {
		showBitmapAvatar(BITMAP_DYBCZAK_ID);
	} else if (id == 3) {
		showBitmapAvatar(BITMAP_RAV_ID);
	} else if (id == 4) {
		showBitmapAvatar(BITMAP_ZAJMA_ID);
	} else if (id == 5) {
		showBitmapAvatar(BITMAP_PLOTNIK_ID);
	} else if (id == 6) {
		showBitmapAvatar(BITMAP_CAPALA_ID);
	} else if (id == 7) {
		showBitmapAvatar(BITMAP_TOMSIA_ID);
	} else if (id == 8) {
		showBitmapAvatar(BITMAP_RADOJ_ID);
	} else {
		showBitmapAvatar(BITMAP_BLUE_ICONS_REMOVE_48_ID);
	}
}

void AuthScreenView::showBitmapAvatar(uint16_t bitmap_value) {
	currentAvatarImage.setBitmap(touchgfx::Bitmap(bitmap_value));
	currentAvatarImage.setPosition(675, 0, 125, 150);
	invalidateAvatar();
}

void AuthScreenView::saveAvatar(uint8_t clickedId) {
	touchgfx::HAL::getInstance()->taskDelay(1000);

	while (!rfid_is_new_card()) {
		vTaskDelay(50);
	}

	waitingTextField.setVisible(false);
	waitingTextField.invalidate();

	xprintf("\r\n WRITE \r\n");

	rfid_status_t status = rfid_select_tag(uidTabBuffer, &size, &sak);
	xprintf("tag read status: %d \r\n", status);

	status = rfid_authenticate(MIF_AUTHENTB, 1, key_tab, uidTabBuffer);
	xprintf("authenticate WRITE status: %d \r\n", status);

	buffer[0] = clickedId;

	if (status == MI_OK) {
		status = rfid_card_write(1, buffer, bufferSize);
		xprintf("WRITE status: %d \r\n", status);

		if (status == MI_OK) {
			xprintf("Saved \r\n");
			showAvatarName("SAVED");
			rfid_halt();
			rfid_stop_crypto();
		}
	}
}

void AuthScreenView::showWaitingForCardText() {
	waitingTextField.setVisible(true);
	waitingTextField.invalidate();

	Unicode::strncpy(saveBuf, avatarName.c_str(), 45);
	Unicode::snprintf(waitingTextFieldBuffer, WAITINGTEXTFIELD_SIZE, "%s", saveBuf);

	waitingTextField.invalidate();
}

void AuthScreenView::confirmChoiceHandler() {
	xprintf("Choice confirmed, you chose %s \r\n", avatarName.c_str());
	showWaitingForCardText();
	vTaskDelay(300);
	saveAvatar(clickedAvatarId);
}

void AuthScreenView::showAvatarName(std::string state) {
	xprintf("%s avatar: %s \r\n", state.c_str(), avatarName.c_str());

	char statementToPrint[50];
	sprintf (statementToPrint, "%s : %s", state.c_str(), avatarName.c_str());
	Unicode::strncpy(saveBuf, statementToPrint, 45);

	Unicode::snprintf(avatarNameTextFieldBuffer, AVATARNAMETEXTFIELD_SIZE, "%s", saveBuf);
	avatarNameTextField.invalidate();
}

void AuthScreenView::szczygiHandler() {
	xprintf("szczygi clicked \r\n");
	avatarName = "SZCZYGI";
	showAvatarName("CLICKED");
	clickedAvatarId = 1;
}

void AuthScreenView::dybczakHandler() {
	xprintf("dybczak clicked \r\n");
	avatarName = "DYBCZAK";
	showAvatarName("CLICKED");
	clickedAvatarId = 2;
}

void AuthScreenView::rafalHandler() {
	xprintf("rafal clicked \r\n");
	avatarName = "RAFAL";
	showAvatarName("CLICKED");
	clickedAvatarId = 3;
}

void AuthScreenView::zajmaHandler() {
	xprintf("zajma clicked \r\n");
	avatarName = "ZAJMA";
	showAvatarName("CLICKED");
	clickedAvatarId = 4;
}

void AuthScreenView::plotnikHandler() {
	xprintf("plotnik clicked \r\n");
	avatarName = "PLOTNIK";
	showAvatarName("CLICKED");
	clickedAvatarId = 5;
}

void AuthScreenView::capalaHandler() {
	xprintf("capala clicked \r\n");
	avatarName = "CAPALA";
	showAvatarName("CLICKED");
	clickedAvatarId = 6;
}

void AuthScreenView::tomsiaHandler() {
	xprintf("tomsia clicked \r\n");
	avatarName = "TOMSIA";
	showAvatarName("CLICKED");
	clickedAvatarId = 7;
}

void AuthScreenView::radojHandler() {
	xprintf("radoj clicked \r\n");
	avatarName = "RADOJ";
	showAvatarName("CLICKED");
	clickedAvatarId = 8;
}

void AuthScreenView::invalidateAvatar() {
	currentAvatarImage.invalidate();
}

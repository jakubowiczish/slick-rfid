#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "rfid.h"
#include "term_io.h"
#include <stdbool.h>
#include "cmsis_os.h"
#include "fatfs.h"
#include "usb_host.h"
#include "dbgu.h"

Model::Model() : modelListener(0) {
	isAuthenticated = false;
	bufferSize = 18;
	avatarBlockAddress = 1;
}

void Model::tick() {
	if (!isAuthenticated && rfid_is_new_card()) {
		uint8_t size = 30;
		uint8_t sak;

		rfid_status_t status = rfid_select_tag(uidTabBuffer, &size, &sak);
		xprintf("tag read status: %d \r\n", status);

		uint8_t key_tab[6];
		for (int i = 0; i < 6; ++i) {
			key_tab[i] = 0xff;
		}

		uint8_t buffer[18];

		if (status == MI_OK) {
			status = rfid_authenticate(MIF_AUTHENTA, 1, key_tab, uidTabBuffer);
			xprintf("authenticate READ status: %d \r\n", status);

			if (status == MI_OK) {
				isAuthenticated = true;
				status = rfid_card_read(avatarBlockAddress, buffer, &bufferSize);
				xprintf("READ status: %d \r\n", status);

				avatarId = buffer[1];

				modelListener->showAuthScreen();
			}
		}
	}
}

//void Model::saveUidAndAvatar(uint8_t uid_tab[], uint8_t avatar_id) {
//	std::string uid_str = convertUidToString(uid_tab);
//
//	for (int i = 0; i < storage_size; ++i) {
//		if (saved_uids[i].compare(uid_str) == 0) break;
//		if (saved_uids[i].compare("-1") == 0) {
//			saved_uids[i] = uid_str;
//			avatar_ids_for_uids[i] = avatar_id;
//		}
//	}
//}
//
//uint8_t Model::getAvatarForUid(std::string uid) {
//	for (uint8_t i = 0; i < storage_size; ++i) {
//		if (saved_uids[i].compare(uid) == 0) {
//			return avatar_ids_for_uids[i];
//		}
//	}
//
//	return -1;
//}
//
//std::string Model::convertUidToString(uint8_t uid_tab[]) {
//	std::string uid_str;
//	for (int i = 0; i < 4; ++i) {
//		uid_str += std::to_string(uid_tab[i]);
//	}
//
//	return uid_str;
//}

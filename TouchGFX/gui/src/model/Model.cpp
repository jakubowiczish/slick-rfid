#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "rfid.h"
#include "term_io.h"
#include <stdbool.h>

Model::Model() : modelListener(0) {

	for (int i = 0; i < storage_size; ++i) {
		saved_uids[i] = "-1";
		avatar_ids_for_uids[i] = -1;
	}
}

void Model::tick() {
	if (rfid_is_new_card()) {
		uint8_t size = 30;
		uint8_t sak;

		rfid_status_t status = rfid_select_tag(uid_tab_buffer, &size, &sak);
		xprintf("tag read status: %d \r\n", status);

		if (status == MI_OK) {
			xprintf("card is present");
			modelListener->notifyThatNewCardIsPresent(uid_tab_buffer);
		}

		uint8_t key_tab[6];
		for (int i = 0; i < 6; ++i) {
			key_tab[i] = 0xff;
		}

		uint8_t buffer[18];
		uint8_t bufferSize = 18;
		uint8_t blockAddress = 1;

		bool isWaiting = false;

		if (status == MI_OK) {
			modelListener->notifyThatNewCardIsPresent(uid_tab_buffer);
			isWaiting = true;

			status = rfid_authenticate(MIF_AUTHENTB, 1, key_tab, uid_tab_buffer);
			xprintf("authenticate status: %d \r\n", status);

			uint8_t cardAvatarId = buffer[1];
			uint8_t avatarId = modelListener->handleAvatarChoice(cardAvatarId);
			status = rfid_card_write(blockAddress, buffer, &bufferSize);
		}
	}
}

void Model::saveUidAndAvatar(uint8_t uid_tab[], uint8_t avatar_id) {
	std::string uid_str = convertUidToString(uid_tab);

	for (int i = 0; i < storage_size; ++i) {
		if (saved_uids[i].compare(uid_str) == 0) break;
		if (saved_uids[i].compare("-1") == 0) {
			saved_uids[i] = uid_str;
			avatar_ids_for_uids[i] = avatar_id;
		}
	}
}

uint8_t Model::getAvatarForUid(std::string uid) {
	for (uint8_t i = 0; i < storage_size; ++i) {
		if (saved_uids[i].compare(uid) == 0) {
			return avatar_ids_for_uids[i];
		}
	}

	return -1;
}

std::string Model::convertUidToString(uint8_t uid_tab[]) {
	std::string uid_str;
	for (int i = 0; i < 4; ++i) {
		uid_str += std::to_string(uid_tab[i]);
	}

	return uid_str;
}

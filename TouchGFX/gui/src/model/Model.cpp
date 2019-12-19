#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "rfid.h"
#include "term_io.h"

Model::Model() :
		modelListener(0) {
}

void Model::tick() {
	if (rfid_is_new_card()) {
		uint8_t size = 30;
		uint8_t sak;

		rfid_status_t status = rfid_select_tag(uid_tab_buffer, &size, &sak);
		modelListener->notifyThatNewCardIsPresent(uid_tab_buffer);
		modelListener->handleAvatarChoice(uid_tab_buffer);
	}
}

#ifndef MODEL_HPP
#define MODEL_HPP
#include "stdint.h"
#include <string>

class ModelListener;

/**
 * The Model class defines the data model in the model-view-presenter paradigm.
 * The Model is a singular object used across all presenters. The currently active
 * presenter will have a pointer to the Model through deriving from ModelListener.
 *
 * The Model will typically contain UI state information that must be kept alive
 * through screen transitions. It also usually provides the interface to the rest
 * of the system (the backend). As such, the Model can receive events and data from
 * the backend and inform the current presenter of such events through the modelListener
 * pointer, which is automatically configured to point to the current presenter.
 * Conversely, the current presenter can trigger events in the backend through the Model.
 */
class Model {
	public:
		Model();

		/**
		 * Sets the modelListener to point to the currently active presenter. Called automatically
		 * when switching screen.
		 */
		void bind(ModelListener *listener) {
			modelListener = listener;
		}

		/**
		 * This function will be called automatically every frame. Can be used to e.g. sample hardware
		 * peripherals or read events from the surrounding system and inject events to the GUI through
		 * the ModelListener interface.
		 */
		void tick();

//		void saveUidAndAvatar(uint8_t uid_tab[], uint8_t avatar_id);
//
//		uint8_t getAvatarForUid(std::string uid);
//
//		std::string convertUidToString(uint8_t uid_tab[]);

		uint8_t uidTabBuffer[10];

		uint8_t bufferSize = 18;

		uint8_t buffer[18];

		uint8_t avatarId;

		bool isAuthenticated = false;

		uint8_t avatarBlockAddress = 1;

		uint8_t size = 30;
		uint8_t sak;

		uint8_t key_tab[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

	protected:
		/**
		 * Pointer to the currently active presenter.
		 */
		ModelListener *modelListener;
};

#endif /* MODEL_HPP */

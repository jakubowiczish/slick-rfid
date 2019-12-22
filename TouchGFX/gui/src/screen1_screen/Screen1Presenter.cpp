#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

Screen1Presenter::Screen1Presenter(Screen1View &v) :
		view(v) {
}

void Screen1Presenter::activate() {

}

void Screen1Presenter::deactivate() {

}

void Screen1Presenter::notifyThatNewCardIsPresent(uint8_t uid_tab[]) {
	view.setUidText(uid_tab);
}

uint8_t Screen1Presenter::handleAvatarChoice(uint8_t id) {
	return view.chooseAvatar(id);
}

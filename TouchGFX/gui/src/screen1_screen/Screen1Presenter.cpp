#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

Screen1Presenter::Screen1Presenter(Screen1View &v) :
		view(v) {
}

void Screen1Presenter::activate() {

}

void Screen1Presenter::deactivate() {

}

void Screen1Presenter::notify_new_card_present(uint8_t uid_tab[], uint8_t size) {
	view.set_uid_text(uid_tab, size);
}

#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

Screen1Presenter::Screen1Presenter(Screen1View &v) : view(v) {
}

void Screen1Presenter::activate() {
	initalizeArrays();
	model->isAuthenticated = false;
}

void Screen1Presenter::deactivate() {

}

void Screen1Presenter::showAuthScreen() {
	view.showAuthScreen();
}

void Screen1Presenter::initalizeArrays() {
	for (int i = 0; i < 10; ++i) model->uidTabBuffer[i] = 0;
	for (int i = 0; i < 18; ++i) model->buffer[i] = 0;
	for (int i = 0; i < 6; ++i) model->key_tab[i] = 0xff;
}

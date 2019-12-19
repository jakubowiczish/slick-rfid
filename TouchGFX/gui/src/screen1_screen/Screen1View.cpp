#include <gui/screen1_screen/Screen1View.hpp>
#include <iostream>
#include <sstream>
#include "term_io.h"
#include <bits/stdc++.h>

Screen1View::Screen1View() {

}

void Screen1View::setupScreen() {
	Screen1ViewBase::setupScreen();
	uidTextField.setVisible(false);
}

void Screen1View::tearDownScreen() {
	Screen1ViewBase::tearDownScreen();
}

void Screen1View::set_uid_text(uint8_t uid_tab[], uint8_t size) {
	Unicode::snprintf(uidTextFieldBuffer, UIDTEXTFIELD_SIZE, "%d %d %d %d", uid_tab[0], uid_tab[1], uid_tab[2], uid_tab[3]);
	uidTextField.setVisible(true);
	uidTextField.invalidate();
}

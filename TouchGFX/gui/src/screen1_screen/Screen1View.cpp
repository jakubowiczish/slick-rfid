#include <gui/screen1_screen/Screen1View.hpp>
#include <iostream>
#include <sstream>
#include "term_io.h"
#include <bits/stdc++.h>

Screen1View::Screen1View() {

}

void Screen1View::setupScreen() {
	Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen() {
	Screen1ViewBase::tearDownScreen();
}

void Screen1View::set_uid_text(uint8_t uid_tab[], uint8_t size) {
	std::ostringstream os;
	for (int i = 0; i < size; ++i) {
//		xprintf("%d \r\n", i);
		os << uid_tab[i];
	}

	std::string str(os.str());

//	int n = str.length();
//	char char_arr[n + 1];
//
//	strcpy(char_arr, str.c_str());

//	xprintf(char_arr);

	Unicode::snprintf(uidTextFieldBuffer, UIDTEXTFIELD_SIZE, "%s", str);
	uidTextField.invalidate();
}

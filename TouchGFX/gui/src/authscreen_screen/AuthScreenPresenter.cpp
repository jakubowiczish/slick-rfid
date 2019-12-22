#include <gui/authscreen_screen/AuthScreenView.hpp>
#include <gui/authscreen_screen/AuthScreenPresenter.hpp>
#include "term_io.h"

AuthScreenPresenter::AuthScreenPresenter(AuthScreenView &v) : view(v)  {
}

void AuthScreenPresenter::activate() {

}

void AuthScreenPresenter::deactivate() {

}

void AuthScreenPresenter::showUid() {
	xprintf("uid buffer %d %d %d %d \r\n", model->uidTabBuffer[0], model->uidTabBuffer[1], model->uidTabBuffer[2], model->uidTabBuffer[3]);
	view.showUid(model->uidTabBuffer);
}

void AuthScreenPresenter::showAvatar() {
	xprintf("avatarId %d \r\n", model->avatarId);
	view.showAvatar(model->avatarId);
}

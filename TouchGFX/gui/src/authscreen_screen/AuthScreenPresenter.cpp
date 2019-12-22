#include <gui/authscreen_screen/AuthScreenView.hpp>
#include <gui/authscreen_screen/AuthScreenPresenter.hpp>

AuthScreenPresenter::AuthScreenPresenter(AuthScreenView& v)
    : view(v)
{
}

void AuthScreenPresenter::activate()
{

}

void AuthScreenPresenter::deactivate()
{

}

void AuthScreenPresenter::showUid() {
	view.showUid(model->uidTabBuffer);
}

void AuthScreenPresenter::showAvatar() {
	view.showAvatar(model->avatarId);
}

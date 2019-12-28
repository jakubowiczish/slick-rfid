#ifndef AUTHSCREENPRESENTER_HPP
#define AUTHSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class AuthScreenView;

class AuthScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    AuthScreenPresenter(AuthScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~AuthScreenPresenter() {};

    virtual void showUid();

    virtual void showAvatar();

    virtual void saveAvatar();

    virtual void hideWaitingTextField();

    virtual void showSavedAvatarName();

    uint8_t clickedId;

private:
    AuthScreenPresenter();

    AuthScreenView& view;
};


#endif // AUTHSCREENPRESENTER_HPP

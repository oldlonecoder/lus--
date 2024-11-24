//#ifndef TUX_STRING_H
//#define TUX_STRING_H
/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all Code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/


#pragma once
#include <lus++/tui/widgets/window.h>



namespace lus::ui
{
class application;

}


namespace lus::ui::terminal
{


/*!
 * @brief screen's double back-buffer to be studied...
 */
class LUS_API screen : public widget
{
    CLASSNAME(screen)

    terminal::vchar::back_buffer _screen_buffer_{nullptr};

    std::vector<rectangle> _dirty_stack_{};
    std::list<window*> _windows_{};

    static screen* the_screen;
    void refresh_zorder();
    void render_screen(const ui::rectangle& rect);

    terminal::vchar::string::iterator peek_sb(ui::cxy xy);
public:
    screen();
    explicit screen(const std::string& _id);
    ~screen() override;

    screen& operator *() override{return *this;}
    log::code update() override;
    log::code draw() override;
    log::code show() override;
    log::code setup_ui() override;
    ui::status_bar* statusbar();

    log::code add_window(window* w);
    log::code show_window(window* w);
    log::code hide_window(window* w);
    log::code remove_window(window* w);
    log::code render_window(window* w, const rectangle& r={});


protected:
    friend class lus::ui::window;
    friend class lus::ui::widget;
    friend class lus::ui::application;

    log::code dirty(const rectangle& _dirty_rect) override;

private:
    log::code blit_window(window* _window);
    log::code blit_buffer(const rectangle& rect);
    log::code setup();

    status_bar* _status_bar_{nullptr};

};

}

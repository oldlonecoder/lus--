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

#include <lus++/tui/widget.h>
#include <lus++/tui/widgets/status_bar.h>
//#include <tuxtui/ui/widgets/menu_bar.h>

namespace lus::ui
{


/*!
 * \brief Public window class.
 *      Toplevel widgets
 */
class LUS_API window : public widget
{
    CLASSNAME(window)

    terminal::screen* _desktop_{nullptr}; ///< Pointer to the 'desktop' window.
    friend class terminal::screen;
    status_bar* _status_bar_{nullptr};

public:
    window() : widget() {}
    ~window() override;  // nothing to do right now.

    window(object* _parent, const std::string& _id, ui::components::type _components);

    log::code hide() override;
    log::code show() override;
    log::code setup_ui() override;

    //status_bar* statusbar(){ return _status_bar_; }


    //...
protected:
    log::code dirty(const rectangle& _dirty_rect) override;

private:
    int _tli_{-1}; ///< current index number on the screen windows (z-order) stack. Refreshed on every screen's windows stack operations.

};


}
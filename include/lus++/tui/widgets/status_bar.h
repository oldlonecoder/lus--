//
// Created by oldlonecoder on 24-11-01.
//

//#ifndef STATUS_BAR_H
//#define STATUS_BAR_H
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


#include <lus++/tui/widgets/label.h>
#include <lus++/tui/widgets/input_track.h>
namespace lus::ui
{

class LUS_API status_bar : public widget
{
    CLASSNAME(statubar)

public:
    status_bar() : widget(){};
    ~status_bar() override=default;
    status_bar(widget* _parent, const std::string& _id);
    log::code show() override;
    void       activate() override;
    void       set_focus() override;
    log::code update() override;

    log::code draw() override;

    widget* add_widget(const std::string& wid, ui::anchor::value av);
    label*  add_label(const std::string& lid, ui::anchor::value av, ui::components::type cc, const std::string& _initial_text={});
    log::code setup_ui() override;
    intrack* input_stats();
protected:
    log::code dirty(const rectangle& dirty_rect) override;

private:
    intrack*  _input_stats_{nullptr};
};

} // namepsace lus::ui

//#endif //STATUS_BAR_H
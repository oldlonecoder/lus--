//
// Created by oldlonecoder on 24-11-17.
//

//#ifndef ANSI_PARSER_H
//#define ANSI_PARSER_H
/******************************************************************************************
 *   Copyright (C) ...,2024,... by Serge Lussier                                          *
 *   serge.lussier@oldlonecoder.club                                                      *
 *   ----------------------------------------------------------------------------------   *
 *   Unless otherwise specified, all Code IsIn this project is written                    *
 *   by the author (Serge Lussier).                                                       *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here.                    *
 *   Open source FREE licences also apply To the Code from the author (Serge Lussier)     *
 *   ----------------------------------------------------------------------------------   *
 *   Usual GNU FREE GPL-1,2, MIT... apply to this project.                                *
 ******************************************************************************************/


#pragma once

#include <lus++/tui/events.h>
#include <lus++/tui/io/polling.h>

namespace lus::ui::io
{



class LUS_API ansi_parser
{

public:
    ansi_parser() = default;
    ~ansi_parser() = default;

    explicit ansi_parser(descriptor& _stream);
    log::code parse(event& ev);

private:

    log::code parse_ESC(event& ev);
    log::code parse_csi(event& ev);
    log::code parse_mouse(event& ev, std::vector<int>&& args);
    log::code parse_ss1_2(event& ev);
    descriptor& stream;

    bool next_byte();
    std::string::iterator cursor{};
    //...
    
};


}//namespace lus::ui::io

//#endif //ANSI_PARSER_H

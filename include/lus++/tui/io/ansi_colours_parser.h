//
// Created by oldlonecoder on 24-11-12.
//

//#ifndef ANSI_PARSER_H
//#define ANSI_PARSER_H
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

#include <fstream>
#include <lus++/tools/sscan.h>
#include <lus++/tui/vchar.h>

namespace lus::io
{

class LUS_API ansi_colours_parser
{
    CLASSNAME_START(ansi_colours_parser)
    lus::ui::terminal::vchar::back_buffer _product_{nullptr};
    std::string _filename_{};
    std::string _text_{};
    std::vector<std::string> _lines_{};
    std::ifstream _file{};
    std::mutex _log_lock{};
    lus::ui::terminal::vchar::string _line{};
    lus::ui::terminal::vchar::string::iterator _cursor{};
    std::string::const_iterator _input_{};
    lus::ui::terminal::vchar::back_buffer _production_{nullptr};
    //lus::ui::terminal::vchar::
public:
    ansi_colours_parser();
    virtual ~ansi_colours_parser();

    log::code parse(const char* _filename);
    log::code parse(const std::string& _filename);
    log::code parse_text(const std::string& _text);


private:

    #pragma region parsers
    using parser = auto(ansi_colours_parser::*)()->log::code; // hehehe yeah it compiles ;)

    auto _fg() -> log::code;
    auto _bg() -> log::code;;
    auto _rgb() -> log::code;;
    auto _eos() -> log::code;; // End Of Sequence
    auto _string() -> log::code;
    auto _scan_num() -> log::code;
    #pragma endregion parsers
};


} // namespace lus::io

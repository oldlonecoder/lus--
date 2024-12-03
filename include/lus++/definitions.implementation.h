//
// Created by oldlonecoder on 24-12-03.
//

//#ifndef DEFINITIONS_IMPLEMENTATION_H
//#define DEFINITIONS_IMPLEMENTATION_H
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

#include <lus++/defs.h>
#include <lus++/tools/colors.h>
#include <lus++/tools/glyphes.h>

namespace lus::definitions
{
std::string LUS_API to_string(rem::type::type enum_type);
std::string LUS_API to_string(rem::code enum_code);
std::string LUS_API to_string(rem::fn   enum_fn);
std::string LUS_API to_string(rem::action enum_action);
std::pair<glyph::type, color::pair> LUS_API type_attributes         (rem::type enum_type);
std::pair<glyph::type, color::pair> LUS_API return_code_attributes  (rem::code enum_code);
std::pair<glyph::type, color::pair> LUS_API function_attributes     (rem::fn enum_fn);
std::pair<glyph::type, color::pair> LUS_API action_attributes       (rem::action enum_action);


}


operator bool(rem::code enum_code);
operator bool(rem::action enum_action);

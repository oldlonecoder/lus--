/******************************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                                        *
 *   serge.lussier@oldlonecoder.club                                                      *
 *                                                                                        *
 *                                                                                        *
 *   Unless otherwise specified, all Code IsIn this project is written                    *
 *   by the author (Serge Lussier)                                                        *
 *   and no one else then not even {copilot, chatgpt, or any other AI calamities}         *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here                     *
 *   Open source FREE licences also apply To the Code from the same author (Serge Lussier)*
 *   ----------------------------------------------------------------------------------   *
 ******************************************************************************************/

//#pragma once
#include <lus++/tui/vchar.h>




namespace lus::ui::terminal
{

#define _eol_ color::pair(color::reset, color::reset)()


[[maybe_unused]] vchar::vchar(U32 Ch) : d(Ch){}

vchar::vchar(vchar* C): d(C->d){}

vchar::vchar(color::pair cc)
{
    set_colors(cc);
    d |= ASCII | 0x20&0xFF;
}


vchar &vchar::set_fg(color::code fg) { d = d & ~FGMask | static_cast<U8>(fg) << FGShift; return *this; }
vchar &vchar::set_bg(color::code bg) { d = d & ~BGMask | static_cast<U8>(bg) << BGShift; return *this; }
[[maybe_unused]] vchar &vchar::set_attributes(U32 ch) { d = (d & ~AttrMask) | ch; return *this; }
//[[maybe_unused]] vchar &vchar::set_colors(color::pair &&Ch) { return set_fg(Ch.fg).set_bg(Ch.bg); }

[[maybe_unused]] vchar &vchar::set_colors(const color::pair &CP)
{
    d = d & ~ColorsMask | static_cast<U8>(CP.fg) << FGShift | static_cast<U8>(CP.bg) << BGShift;
    return *this;
}

vchar &vchar::operator=(U32 Ch) { d = Ch; return *this; }

vchar &vchar::operator=(const U32* Ch) { d = *Ch; return *this; }


std::string vchar::get_utf_string() const
{
    if(d & UTFBITS)
    {
        switch(d & UTFBITS)
        {
        case UGlyph:
            return  glyph::data[icon_id()];
        case Accent:
            return  accent_fr::data[accent_id()];
        case Frame:
            return cadre()[frame_id()];
        default: break;
            //throw log::exception() [log::fatal() << " Memory corruption error into vchar data!"];
        }
    }
    std::string s;
    s += ascii();
    return s;
}


vchar &vchar::operator=(char Ch)
{
    d = d & ~(CharMask|UGlyph|Accent|Frame) | d & (CMask|Underline|Stroke|Blink|TRGB)  | ASCII | Ch & 0xff;
    return *this;
}

color::code vchar::foreground() const { return static_cast<color::code>((d & FGMask) >> FGShift); }
color::code vchar::background() const { return static_cast<color::code>((d & BGMask) >> BGShift); }
[[maybe_unused]] color::pair vchar::colors() const { return {foreground(),background()}; }

[[maybe_unused]] glyph::type vchar::icon_id() const
{
    if (!(d & UGlyph))
        throw log::exception() [ log::except() << log::code::rejected << "attempt to use this vchar cell as a glyph which is not,"];

    auto Ic = d & CharMask;
    if (Ic > glyph::log)
        throw log::exception()[log::except() << log::fn::func << log::code::oob << ':' << Ic];
    return  Ic;
}


[[maybe_unused]] accent_fr::type vchar::accent_id() const
{
    auto AID = d & CharMask;
    if (AID > accent_fr::Ucirc)
        throw log::exception()[log::fatal() << log::fn::func << log::code::oob << ':' << AID];

    return static_cast<accent_fr::type>(AID);
}




cadre::index vchar::frame_id() const
{
    auto c = d & 0xff;
    if(c > 11)
        throw log::exception() [log::except() << log::code::oob << log::fn::func << " invalid frame index: " << color::red4 << c];

    return static_cast<cadre::index>(d & 0xFF);
}

char vchar::ascii() const                           { return static_cast<char>(d & 0xff); }
[[maybe_unused]] uint16_t vchar::attributes() const { return (d & AttrMask) >> ATShift; }
vchar &vchar::operator<<(glyph::type gl)            { d = (d & ~(UtfMask|CharMask)) | (d&ColorsMask)|UGlyph | (gl&0xFF); return *this; }
vchar &vchar::operator<<(accent_fr::type ac)        { d = (d& ~(AttrMask|CharMask)) | (d & (Underline|Stroke|Blink|ColorsMask)) | Accent | ac; return *this; }
vchar& vchar::operator<<(cadre::index fr)           { d = (d & ~(UtfMask|CharMask)) | (d&ColorsMask)|(fr&0xFF) | Frame; return *this; }
vchar& vchar::operator<<(color::pair cp)            { d = (d & ~ColorsMask) | static_cast<U8>(cp.fg) << FGShift | static_cast<U8>(cp.bg) << BGShift; return *this; }
vchar& vchar::operator<<(char Ch)                   { d = (d & ~(UTFBITS|CharMask)) | (d & (Underline|Stroke|Blink|ColorsMask)) | ASCII | (Ch & 0xff); return *this; }


[[maybe_unused]] std::string vchar::render_colors() const
{
    std::string str;
    str += color::render({background(),foreground()});
    return str;
}

vchar::operator std::string() const { return details(); }



// std::string vchar::render_line(const vchar::string& _string)
// {
//     return "vchar::render_line(const vchar::string& _string) is not implemented";
// }


std::string vchar::render_line(vchar::string::iterator _it, std::size_t count)
{
    color::pair current_colors = _it->colors();
    //log::debug() << "iterator details:" << _it->details() << log::eol;
    std::string _o = current_colors();
    auto c = _it;
    for(int x =0; x< count; x++)
    {
        vchar ch = *c++;
        auto  [fg, bg] = ch.colors();
        if(current_colors.bg != bg)
        {
            current_colors.bg = bg;
            _o += color::render_background(current_colors.bg);
        }
        if(current_colors.fg != fg)
        {
            current_colors.fg = fg;
            _o += color::render(current_colors.fg);
        }
        if(ch.d & UTFBITS)
        {
            if(ch.d & Frame)
                _o += cadre()[ch.frame_id()];
            else
                if(ch.d & Accent)
                    _o += accent_fr::data[ch.accent_id()];
                else
                    if(ch.d & UGlyph)
                    {
                        _o += glyph::data[ch.icon_id()];
                        _o += '\x08';
                    }
        }
        else
            _o += ch.ascii();
    }
    _o += _eol_;
    return _o;
}




// std::string vchar::render_line(const vchar* _blk, int _width)
// {
//
//     color::pair current_colors = _blk->colors();
//     std::string _o = current_colors();
//     const vchar* c = _blk;
//     for(int x =0; x< _width; x++)
//     {
//         vchar ch       = *c++;
//         auto  [fg, bg] = ch.colors();
//         if(current_colors.bg != bg)
//         {
//             current_colors.bg = bg;
//             _o += color::render_background(current_colors.bg);
//         }
//         if(current_colors.fg != fg)
//         {
//             current_colors.fg = fg;
//             _o += color::render_line(current_colors.fg);
//         }
//         if(ch.d& UTFBITS)
//         {
//             if(ch.d & Frame)
//                 _o += cadre()[ch.frame_id()];
//             else
//                 if(ch.d & Accent)
//                     _o += accent_fr::data[ch.accent_id()];
//                 else
//                     if(ch.d & UGlyph)
//                         _o += glyph::data[ch.icon_id()];
//         }
//         else
//             _o += ch.ascii();
//     }
//     _o += _eol_;
//     //std::cout << _o << std::flush;
//     return _o;
// }

// log::code vchar::render_string(vchar::string::iterator start, vchar::string::iterator end)
// {
//     std::string str;
//     auto it = start;
//     str = start->colors()();
//     //log::log() << color::lightsteelblue3 <<" ::render_string: " << start->details();
//     color::pair curcolors = it->colors();
//     while(it != end)
//     {
//         auto check = it->colors();
//         if(curcolors.bg != check.bg)
//         {
//             curcolors.bg = check.bg;
//             str += color::render_background(curcolors.bg);
//         }
//         if(curcolors.fg != check.fg)
//         {
//             curcolors.fg = check.fg;
//             str += color::render_line(curcolors.fg);
//         }
//         if(it->d& UTFBITS)
//         {
//             if(it->d & Frame)
//             {
//                 str += cadre()[it->frame_id()];
//                 ++it;
//                 continue;
//             }
//             else
//                 if(it->d & Accent)
//                 {
//                     str += accent_fr::data[it->accent_id()];
//                     ++it;
//                     continue;
//                 }
//                 else
//                     if(it->d & UGlyph)
//                     {
//                         str += glyph::data[it->icon_id()];
//                         str += (char)0x08;
//                         ++it;
//                         continue;
//                     }
//         }
//         else
//             str += (it++)->ascii();
//     }
//     std::cout << str <<  _eol_;
//     return log::code::done;
// }


std::string vchar::details() const
{

    lus::string infos;
    lus::string utf_info{};
    if(d & UTFBITS)
    {
        switch(d & UTFBITS)
        {
        case UGlyph:
            utf_info | "icon index:" | icon_id();
            break;
        case Accent:
            utf_info | "accent (fr) index:" | accent_id();
            break;
        case Frame:
            utf_info | "frame index:" | frame_id();
            break;
        default:break;
            //throw rem::exception() [rem::fatal() << " Memory corruption into screen::vchar data!"];
        }
    }
    else
        utf_info | ascii();
    infos | "| foreground color:" | foreground() | color::name(foreground()) | color::reset | "| background color:" | background() | color::name(background()) | color::reset;
    infos | " char:['" | colors() | utf_info() | color::reset |  "']";
    if(d & Underline) infos | "|Underline";
    if(d & Stroke) infos | "|Stroke";
    if(d & Blink) infos |"|Blink";

    return infos();
}



}// namespace lusin::ui

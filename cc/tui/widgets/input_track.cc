
#include <lus++/tui/widgets/input_track.h>



namespace lus::ui
{

intrack::intrack():widget(){}

intrack::intrack(widget* _parent, const std::string& a_id): widget(_parent, a_id){}



/**
 * @brief refresh the ui with the new event data.
 *
 * @param e Reference to the event data.
 * @return log::code
 */
log::action intrack::update_event_data(const event& e)
{
    ev = e;

    if((ev.event_type == event::type::key_command) || (ev.event_type == event::type::CHARACTER))
    {
        _key_stat_painter_.clear();
        if(ev.event_type == event::type::key_command)
        {
            _key_stat_painter_ << color::yellow << std::format("{:>20s}",e.data.kev.description);

            dirty(_geometry_.tolocal());
            return log::action::continu;
        }
        else
        {
            if(ev.event_type == event::type::CHARACTER)
            {
                _key_stat_painter_ << color::yellow << std::format("{:>20c}",e.data.kev.code);
                dirty(_geometry_.tolocal());
                return log::action::continu;
            }
            return log::action::continu;;
        }
    }
    if(ev.event_type != event::type::MOUSE)
    {
        log::message() << " triggering " << log::code::rejected << " for unhandled event data.";
        //log::write() << "seq: " << color::yellow << lus::string::bytes(e.)
        return log::action::continu;;
    }

    // glyph::type dir{};
    // if(e.data.mev.dxy.x < 0) dir = glyph::arrow_left;
    // else if(e.data.mev.dxy.x > 0) dir = glyph::arrowright;
    // else if(e.data.mev.dxy.y < 0) dir = glyph::arrow_up;
    // else if(e.data.mev.dxy.y > 0) dir = glyph::arrow_down;
    // else dir=glyph::cat;
    char dir{};
    if(e.data.mev.dxy.x < 0) dir = 'l';
    else if(e.data.mev.dxy.x > 0) dir = 'r';
    else if(e.data.mev.dxy.y < 0) dir = 'u';
    else if(e.data.mev.dxy.y > 0) dir = 'd';
    else dir='-';

    _mouse_stat_painter_.clear() << "["

    << color::orangered1 << std::format("{:>3d}", e.data.mev.xy.x)
    << color::reset << ','
    << color::orangered1 << std::format("{:<3d}",e.data.mev.xy.y)
    << color::reset << "]"
    << (e.data.mev.button.left   ? color::orangered1 : color::reset)  << (e.data.mev.button.left    ?'L' : 'l') << color::reset << "|"
    << (e.data.mev.button.middle ? color::lime : color::reset)   << (e.data.mev.button.middle  ?'M' : 'm') << color::reset << "|"
    << (e.data.mev.button.right  ? color::red4 : color::reset)   << (e.data.mev.button.right   ?'R' : 'r') << color::reset << "|"
    << (e.data.mev.move          ? color::yellow : color::reset) << dir
    << color::reset << "["
    << color::orangered1 << std::format("{:>3d}",e.data.mev.dxy.x)
    << color::reset << ','
    << color::orangered1 << std::format("{:<3d}",e.data.mev.dxy.y)
    << color::reset << "]";
    dirty(_geometry_.tolocal());
    log::debug() << pretty_id() << " ---- " << log::eol;
    return log::action::continu;;
}

log::code intrack::setup_ui()
{
    // auto positioning and dimensions.
    set_geometry({{0,0},ui::size{49,1}});
    set_anchor(anchor::Right|anchor::Bottom);
    _key_stat_painter_ = begin_draw(_key_area_);
    _mouse_stat_painter_ = begin_draw(_mouse_area_);

    return log::code::done;
}


log::code intrack::draw()
{
    return widget::draw();
}

}

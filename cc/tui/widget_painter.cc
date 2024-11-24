#include <lus++/tui/widget.h>






namespace lus::ui
{


#define _SYNC_ at(_geometry_.cursor);






/*!
 * \brief Instance public constructor widgets::painter_dc::painter_dc
 * \param parent_widget
 * \param r
 */
widget::painter_dc::painter_dc(widget *parent_widget, const rectangle &r):_widget_(parent_widget), _geometry_(r)
{
    _colors_ = _widget_->_colors_;
    if(!r)
        _geometry_ = _widget_->_geometry_.tolocal();
    home();

    //...
}

log::code widget::painter_dc::at(cxy xy)
{
    //log::debug() << color::lime << _widget_->id() << color::reset << "->painter_dc invokes  at(" << color::yellow << (xy +  _geometry_.a) << color::reset << "):";
    _geometry_.goto_xy(xy);
    _widget_->peek_xy(xy +  _geometry_.a);
    _iterator_ = _widget_->_iterator_;
    return log::code::accepted;
}



/*!
 * \brief widgets::painter_dc::sync
 *  Make sure that our internal _iterator_ is in sync with the internal _geometry_.cursor position.
 * \return result from at(_geometry_.cursor);
 */
log::code widget::painter_dc::sync()
{
    return at(_geometry_.cursor);
}




/*!
 * \brief Instance public widgets::painter_dc::clear
    clears the (sub-)area \c r with the current colors attributes.
 * \param r
 * \return ref to self.
 * \note r must be local coordinates (offset from {0,0}).
 */
widget::painter_dc &widget::painter_dc::clear(const rectangle &r)
{
    auto area = r;
    // log::debug() << log::fn::fun << "arg (rectangle r) =  " << color::yellow << r << color::reset;
    if(!area)
        area = _geometry_.tolocal();


    for(int y = 0; y < area.dwh.h; y++)
    {
        at(ui::cxy{0,y});

        std::fill(_iterator_, _iterator_ + area.dwh.w, terminal::vchar(_colors_));
         // log::write() << "check(first char on line #"
         //             << color::yellow
         //             << y<< color::reset << ":"
         //             << color::lime << _widget_->class_name()
         //             << color::yellow << "::" << color::lightsteelblue3 << _widget_->id() << color::reset
         //             << _iterator_->details();
    }
    return home();

}

widget::painter_dc& widget::painter_dc::home()
{
    _geometry_.home();
    at(_geometry_.cursor);
    return *this;
}



/*!
 * \brief Instance public widgets::painter_dc::set_background_color
 * \param bgcol ...
 * \return ref to self.
 */
widget::painter_dc &widget::painter_dc::set_background_color(color::code bgcol)
{
    _colors_.bg = bgcol;
    return *this;
}


log::code widget::painter_dc::operator ++()
{
    if(_iterator_ >= _widget_->_back_buffer_->end())
        return log::code::rejected;
    ++_iterator_;
    ++_geometry_; // advance internal xy cursor
    return log::code::accepted;
}

log::code widget::painter_dc::operator +=(size_t _offset)
{
    if(!_geometry_.goto_xy(_geometry_.cursor + ui::cxy{static_cast<int>(_offset),0})) return log::code::rejected;
    return at(_geometry_.cursor);
}


widget::painter_dc& widget::painter_dc::draw_frame(cadre::frame_matrix fmatx)
{
    cadre::push(fmatx);
    _widget_->draw_frame(_geometry_);
    cadre::pop();
    return *this;
}


widget::painter_dc& widget::painter_dc::operator << (ui::cxy new_xy)
{
    if(!at(new_xy))
        throw log::exception()
        [
            log::except() <<  log::code::oob << color::red4 << new_xy << color::reset << " > " << color::yellow << _widget_->_geometry_.tolocal()
        ];

    return *this;
}

widget::painter_dc& widget::painter_dc::operator << (char ch)
{
    (*_iterator_) << ch << _colors_;
    ++**this;
    return *this;
}

widget::painter_dc& widget::painter_dc::operator << (const char* str)
{
    auto z = std::strlen(str);
    auto start = _iterator_;
    auto cz = *_geometry_.width() - _geometry_.cursor.x;
    cz = std::min(static_cast<int>(z), cz);

    //log::debug()  << color::lime << _widget_->pretty_id() << color::reset << " computed width:" << color::hotpink4 << cz << log::eol;
    //log::debug() << _widget_->pretty_id() << _colors_ << " colors [" << str << " ]" << log::eol;
    for (int c = 0; c < cz ; c++)
    {
        (*_iterator_) << str[c] << _colors_;
        ++**this;
    }
    return *this;
}

widget::painter_dc& widget::painter_dc::operator << (const std::string& str)
{
    return **this << str.c_str();
}

widget::painter_dc& widget::painter_dc::operator << (lus::string str)
{
    return **this << str().c_str();
    //return *this;
}

widget::painter_dc& widget::painter_dc::operator << (color::code fgcol)
{
    if(fgcol==color::reset)
        _colors_.fg = _widget_->_colors_.fg;
    else
        _colors_.fg = fgcol;

    //_iterator_->set_fg(fgcol);

    return *this;
}

widget::painter_dc& widget::painter_dc::operator << (color::pair colp)
{
    if(colp.fg==color::reset)
        _colors_.fg = _widget_->_colors_.fg;
    else
        _colors_.fg = colp.fg;

    if(colp.bg==color::reset)
        _colors_.bg = _widget_->_colors_.bg;
    else
        _colors_.bg = colp.bg;

    //_iterator_->set_colors(colp);
    return *this;
}

widget::painter_dc& widget::painter_dc::operator << (cadre::index ci)
{
    (*_iterator_) << ci << _colors_;
    ++**this;
    return *this;
}

widget::painter_dc& widget::painter_dc::operator << (glyph::type ic)
{
    (*_iterator_) << ic << _colors_;
    ++**this;
    return *this;
}

widget::painter_dc& widget::painter_dc::operator << (accent_fr::type fr)
{
    (*_iterator_) << fr << _colors_;
    ++**this;
    return *this;
}

widget::painter_dc& widget::painter_dc::operator << (terminal::vchar vch)
{
    return *this;
}

widget::painter_dc& widget::painter_dc::operator << (terminal::vchar::string strvch)
{
    return *this;
}



} // namespace lus::ui

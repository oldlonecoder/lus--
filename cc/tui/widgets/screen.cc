#include <lus++/tui/widgets/screen.h>

#include "lus++/tui/application.h"


namespace lus::ui::terminal
{


screen* screen::the_screen{nullptr};


void screen::refresh_zorder()
{
    int z = 0;
    for(auto wit = _windows_.begin(); wit != _windows_.end(); ++wit, z++) (*wit)->_tli_ = z;
}


screen::screen():widget(){}

screen::screen(const std::string &_id):widget(nullptr,_id)
{
    if(screen::the_screen)
    {
        log::error() << log::fn::func << pretty_id() << " cannot create another screen widgets  : use '" << screen::the_screen->id() << "' instead and destroy this non-usable instance.";
        return ;
    }
    //...
    screen::the_screen = this;
}

screen::~screen()
= default;


log::code screen::setup()
{
    set_geometry(terminal::geometry());
    // We will provide the option to select the theme in future config layout...
    set_theme("C128");
    // --------------------------
    clear();

    // Duplicate back_buffer into _screen_buffer_, where all draws are put before rendering it.
    _screen_buffer_ = std::make_shared<terminal::vchar::string>(_geometry_.dwh.area(), terminal::vchar(_colors_) << ' ');
    // ------------------------------------------------------------------------------------------------------------------
    //Of course we set ourselves being a top-level window, but as the screen widget.
    _uistyle_ |= ui::uistyle::Window;

    log::write() << pretty_id() << " is the screen : Thus it owns its widget-counterpart's back_buffer and the _screen_buffer_." << log::eol;
    dirty(_geometry_);

    return log::code::done;
}



log::code screen::update()
{
    if(_dirty_stack_.empty()) return log::code::empty;

    //rectangle rect = _dirty_stack_[0];
    for(auto r : _dirty_stack_)
    {
        blit_buffer(r);
        for(auto i: _windows_) render_window(i,r);
        render_screen(r);
    }
    //blit_buffer(rect);

    _dirty_stack_.clear();
    _dirty_area_={};
    return log::code::notimplemented;
}


log::code screen::draw()
{
    widget::draw();
    dirty(_geometry_);
    return log::code::done;
}


log::code screen::show()
{
    render_screen(_geometry_);
    return log::code::ok;
}


log::code screen::setup_ui()
{
    if(has_status_bar())
    {
        _status_bar_ = new status_bar(this,id()+"_status_bar");
        _status_bar_->set_theme("C64");
        _status_bar_->auto_fit();
        if(_uicomponents_& ui::components::InputStats)
            _status_bar_->add_component(ui::components::InputStats);
        _status_bar_->setup_ui();
    }

    return widget::setup_ui();
}


ui::status_bar* screen::statusbar() { return _status_bar_; }


log::code screen::add_window(window* w)
{
    auto it = std::find(_windows_.begin(), _windows_.end(), w);
    if(it != _windows_.end()) return log::code::rejected;
    w->_tli_ = static_cast<int>(_windows_.size());
    _windows_.push_back(w);
    w->_desktop_ = this;
    //refresh_zorder();
    log::debug() <<  w->pretty_id() << " added to screen " << pretty_id() << log::eol;
    return log::code::accepted;

}


log::code screen::show_window(window* w)
{
    auto it = std::find(_windows_.begin(), _windows_.end(), w);
    if(it == _windows_.end())
    {
        log::warning() << w->pretty_id() << " not in the screen's windows list..." << log::eol;
        return log::code::rejected;
    }
    blit_window(w);
    render_screen(w->_geometry_);
    return log::code::accepted;
}


log::code screen::hide_window(window* w)
{
    remove_window(w);
    w->_uistate_ &= ui::uistate::Visible|ui::uistate::Active;
    //@todo "Iconify window" ;)
    return log::code::accepted;
}


log::code screen::remove_window(window* w)
{
    auto it = std::find(_windows_.begin(), _windows_.end(), w);
    if(it == _windows_.end()) return log::code::rejected;
    _windows_.erase(it);
    refresh_zorder();
    w->_tli_ = -1;
    w->_desktop_ = nullptr;
    dirty(w->_geometry_);
    update();
    return log::code::accepted;
}


log::code screen::render_window(window* w, const rectangle& rect)
{
    ui::rectangle r = rect;
    if(!rect)
        r = _geometry_ & w->_geometry_;

    //log::debug() << "rendering " << w->pretty_id() << " at " << r << log::eol;

    if(!r)
    {
        log::message() << w->pretty_id() << " not within the screen' boundaries: " << application::screen()->geometry() << log::eol;
        return log::code::rejected;
    }

    auto it = std::find(_windows_.begin(), _windows_.end(), w);
    if(it == _windows_.end())
    {
        log::warning() << w->pretty_id() << " not found getting its iterator?..." << log::eol;
        return log::code::rejected;
    }

    auto wr = r & w->_geometry_;

    if(!wr)
        return log::code::rejected;

    for(int y = 0; y < wr.height(); y++)
    {
        auto lr = wr - w->_geometry_.a;
        w->peek_xy({lr.a.x,lr.a.y+y});
        std::copy_n(w->_iterator_, lr.dwh.w,peek_sb(wr.a + ui::cxy{0,y}));
    }
    return log::code::accepted;
}


/*!
 * @brief Renders the rect region of the screen's _screen_buffer_.
 * @param rect re-validated rectangle.
 */
void screen::render_screen(const ui::rectangle& rect)
{
    auto r = _geometry_ & rect;
    if(!r)
    {
        log::debug() << "rectangle " << color::hotpink4 << rect << color::reset << "is " << log::code::oob << log::eol;
        return;
    }

    //log::debug() << pretty_id() << " rect to render_line : " << color::hotpink4 << r << log::eol;
    for(int y = 0; y < r.height(); y++)
    {
        terminal::cursor({r.a.x,r.a.y+y});
        std::cout << terminal::vchar::render_line(peek_sb({r.a.x,y+r.a.y}),r.dwh.w);
    }
    std::cout << std::flush;
    _dirty_area_ = {};
    _dirty_stack_.clear();
}


/*!
 * @brief Peek to the screen back buffer at the given coords...
 * @param xy
 * @return the (w-r) terminal::vchar::string::iterator.
 */
terminal::vchar::string::iterator screen::peek_sb(ui::cxy xy)
{
    if(!_screen_buffer_) return _back_buffer_->begin();
    if(!_geometry_[xy]) return _screen_buffer_->end();
    return _screen_buffer_->begin() + xy.y * *_geometry_.width() + xy.x;
}


log::code screen::dirty(const rectangle& _dirty_rect)
{
    auto area = _geometry_ & _dirty_rect;
    if(!area)
        return log::code::rejected;

    _dirty_stack_.emplace_back(area); // So all rectangles in the dirty stack are pre-validated.
    return log::code::accepted;
}


/*!
 * 
 * @param _window 
 * @return 
 */
log::code screen::blit_window(window* _window)
{
    if(!_screen_buffer_) return log::code::rejected;
    auto r = _geometry_ & _window->_geometry_;
    if(!r) return log::code::rejected;
    auto rlocal  = r - _window->_geometry_.a;

    for(int y = 0; y < r.height(); y++)
    {
        _window->peek_xy(rlocal.a + cxy{0,y});
        std::copy_n(_window->_iterator_, r.dwh.w, peek_sb(r.a + cxy{0,y}));
    }
    return log::code::accepted;
}


log::code screen::blit_buffer(const rectangle& rect)
{
    if(!_screen_buffer_) return log::code::rejected;
    auto r = _geometry_ & rect;
    if(!r) return log::code::rejected;
    for(int y = 0; y < r.height(); y++)
    {
        peek_xy(r.a + cxy{0,y});
        std::copy_n(_iterator_, r.dwh.w, peek_sb({r.a.x, r.a.y+y}));
    }
    return log::code::done;
}

}

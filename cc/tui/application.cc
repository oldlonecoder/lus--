

#include <lus++/tui/application.h>
#include <lus++/tui/terminal.h>
#include <csignal>
#include <utility>


//#include <memory>


namespace lus::ui
{

application* application::_app_{nullptr};



application::application(std::string  a_id, int argc, char** argv): _app_name_(std::move(a_id))
{
    if(argc && argv)
        _args_ = lus::string::string_view_list(argc,argv);

    if(application::_app_ == nullptr)
    {
        application::_app_ = this;
    }
    else
        abort();
    setup();
}


application::~application()
{
    //...
    _args_.clear();
    ///...
}


void application::sig_interrupted(int)
{
    log::interrupted() << log::eol;
    terminal::end();
    log::purge(nullptr);
    //application::app().terminate();
    exit(0);
}



void application::sig_aborted(int)
{
    log::aborted() << log::eol;
    terminal::end();
    log::purge(nullptr);
    //application::app().terminate();
    exit(0);
}

void application::sig_crash(int)
{
    log::segfault()  << log::eol;
    //terminal::end();
    //log::purge(nullptr);
    //application::app().terminate();
    exit(0);
}


void application::sig_winch(int)
{
    terminal::query_winch() ;
    //...
}



application& application::app()
{
    if(!application::_app_)
        throw log::exception() [log::except() << log::fn::func << log::fn::endl << " no instance of application!"];

    return *application::_app_;
}


terminal::screen* application::screen() { return application::_app_->_terminal_screen_; }


std::string application::app_name() const
{
    lus::string text{};
    auto [gh,colors] = log::return_code_attributes(log::code::object_ptr);
    text
        | colors()
        | gh
        | class_name()
        | color::reset | "::";
    auto [gl, cc] = log::return_code_attributes(log::code::object_id);
    text
        | cc()
        | gl
        | _app_name_
        | color::reset | ' ';

    return text();
}


// desktop *application::screen_desktop()
// {
//     return desktop::instance();
// }

log::code application::install_signals()
{

    std::signal(SIGSEGV, &application::sig_crash);
    log::jnl() << "signal SIGSEV installed."  << log::eol;
    std::signal(SIGABRT, &application::sig_aborted);
    log::jnl() << "signal SIGABRT installed."  << log::eol;
    std::signal(SIGINT, &application::sig_interrupted);
    log::jnl() << "signal SIGINT installed."  << log::eol;
    std::signal(SIGWINCH, &application::sig_winch);
    log::jnl() << "signal SIGWINCH installed."  << log::eol;
    // std::signal(SIGHUP, &application::sig_winch);
    std::signal(SIGKILL, &application::sig_interrupted);
    log::jnl() << "signal SIGKILL installed." << log::eol;
    std::signal(SIGTERM, &application::sig_interrupted);
    log::jnl() << "signal SIGTERM installed." << log::eol;

    return log::code::done;
}


/*!
 *
 * @return log::code result .
 */
log::code application::start_events_listening()
{
    _polling.new_descriptor() = {
        .poll_bits = POLLIN | POLLPRI | POLLHUP,
        .max_length = 1024,
        .fd = STDIN_FILENO
    };
    auto d = _polling.descriptors().back();
    d->init();
    log::debug() << "link descriptor handle [" << d->config().fd << "] to tddv::std_input() delegate/slot/handler..." << log::eol;
    d->pollin_action().connect(this, &application::std_infile_no);
    d->activate();

    // Blocking in this
    return _polling.run();
}




log::code application::setup()
{
    //...
    log::init();
    install_signals();
    terminal::begin();


    return log::code::done;
}


log::code application::setup_ui()
{
    _terminal_screen_ = new terminal::screen("{terminal screen}");
    _terminal_screen_->setup();
    //...
    return log::code::done;
}


log::code application::terminate()
{
    terminal::end();
    delete application::_terminal_screen_;

    log::end();
    return log::code::reimplement;
}



log::action application::std_infile_no(ui::io::descriptor& _d)
{
    log::debug() << "tddv::std_input{" << lus::string::bytes(_d.buffer()) << "}" << log::eol;
    auto r = ui::io::ansi_parser{_d}.parse(_events_q.push());
    if (!r)
    {
        log::error() << " ansi_parser returns" << r << " leaving and return to the stdin polling iteration..." << log::eol;
        return log::action::continu;
    }

    log::debug() << " ansi_parser returned: " << r << log::eol;
    // auto& ev = *_events_q;
    // if (ev[ui::event::command_key])
    // {
    //     log::debug() << " ansi_parser key command" << log::eol;
    //     if (ev.data.kev.code == ui::key_event::ESC) return log::action::end;
    // }
    // log::debug() << " screen stuff:" << log::eol;

    return log::action::commit;
}



}

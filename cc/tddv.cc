//
// Created by oldlonecoder on 24-11-05.
//

#include "tddv.h"

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




namespace lus
{
log::action tddv::std_input(ui::io::descriptor& _d)
{
    ui::event ev{};
    //parsers_pool.enqueu(ev);
    return log::action::end;
}


tddv::~tddv() = default;
// {
//     // Can't delete test widgets here because the logger is already closed.
// }


tddv::tddv(std::string app_name, int argc, char** argv): ui::application(std::move(app_name), argc, argv){}


/*!
 * 
 * @return 
 */
log::code tddv::tddv_setup()
{
    //application::setup();
    log::debug() << log::eol;
    //...
    _polling.add_descriptor() = {
        .poll_bits = POLLIN | POLLPRI | POLLHUP,
        .max_length = 1024,
        .fd = STDIN_FILENO
    };
    _polling.descriptors().back()->pollin_action().connect(this, &tddv::std_input);

    return setup_ui();
}


/*!
 * 
 * @return 
 */
log::code tddv::setup_ui()
{

    application::setup_ui();
    application::screen()->add_component(ui::components::StatusBar|ui::components::InputStats);
    // --- In dev so the UI setup looks a bit messed...
    application::screen()->setup_ui();

    _test_widget_ = new ui::widget(application::screen(), "_test_widget_");
    log::test() << _test_widget_->pretty_id() << "has been created..." << log::eol;
    _test_widget_->set_geometry({{0,34},ui::size{90,5}});
    _test_widget_->set_theme("C64");
    _test_widget_->set_components(ui::components::Frame|ui::components::InputStats);
    _test_widget_->setup_ui();
    _intrack = new ui::intrack(_test_widget_, "_intrack");
    _intrack->set_theme("C64");
    _intrack->setup_ui();
    _test_widget_->anchor_widget(_intrack);


    _label_ = new ui::label(application::screen(),"_label");
    // --- Mandatory strict order of config for all widgets :
    _label_->set_theme("C128");
    _label_->set_text("_label_:I Love Penguins");
    _label_->set_geometry({{0,0},ui::size{80,1}});
    // Then the components :
    _label_->set_prefix_icon(glyph::arch);
    // Now we can draw :

    log::jnl() << app_name() << " ui..." << log::code::done << log::eol;
    log::jnl() << app_name() << " now test auto_fit on sub-sub widgets: " << log::code::done << log::eol;
    auto blb = new ui::label(_test_widget_, _test_widget_->id()+ "_label");
    blb->set_theme("C128");
    blb->set_geometry({{0,0},ui::size{30,1}});
    blb->set_anchor(ui::anchor::Top|ui::anchor::Right);
    blb->set_prefix_icon(glyph::windows);
    blb->set_text(blb->id());
    _test_widget_->anchor_widget(blb);
    blb->draw();
    blb->show();
    application::screen()->update();
    _window = new ui::window(nullptr, "_window", ui::components::Caption|ui::components::Frame);
    _window->set_theme("C64");
    _window->set_geometry({{1,16},ui::size{120,20}});
    auto lbl = new ui::label(_window, _window->id()+"->lbl");
    lbl->set_theme("C64");
    lbl->set_anchor(ui::anchor::Bottom|ui::anchor::Width);
    lbl->set_geometry({{2,20},ui::size{20,1}});
    _window->anchor_widget(lbl);
    lbl->set_text(lbl->id() + " ...");
    _window->draw();
    _window->show();


    _window2 = new ui::window( nullptr, "_window2", ui::components::Caption|ui::components::Frame);
    _window2->set_theme("C64");
    _window2->set_geometry({{66,21},ui::size{10,20}});
    _window2->draw();
    _window2->show();
    _window2->update();
    application::screen()->draw();
    application::screen()->update();
    return log::code::done;
}


/*!
 * 
 * @return 
 */
log::code tddv::run()
{
    try
    {
        tddv_setup();
        log::jnl() << log::fn::stamp << app_name() << log::eol;
        ui::event ev{};
        auto c = log::code::ok;

        _polling.start_polling();

    //     while(ui::event::get_stdin_event(ev,{65000,0})  != log::code::cancel)
    //     {
    //         //application::screen()->statusbar()->input_stats()->update_event_data(ev);
    //         //application::screen()->statusbar()->update();
    //         _intrack->update_event_data(ev);
    //         // tests:
    //         application::screen()->update();
    //     }
    }
    catch(log::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    terminate();
    return log::code::notimplemented;
}


log::code tddv::terminate()
{
    //...
    delete _window;
    delete _window2;
    return application::terminate();
}



} // namespace lus


auto main(int argc, char** argv) -> int
{
    lus::tddv tddv{"tuxic_vision:tddv application!", argc,argv};
    return static_cast<int>(tddv.run());
}


# lus project


# Roadmap of this hobby project:
### Exploring, Learning to write a simple C++ framework of libraries


## tools support classes
---
    - [class lus::string]
        Composite std::string accumulator for serializing and formatting supported components
    - [lus::color]
        text attr obj and colors [mnemonics] conversion util.
    - [logger app journal and logger]
        (incoming Journaling) messages accumulator and logs.
        Note:   The lus::log class is the kernel of the later/projected logger book API
    - [class lus::signals::action / ]
        C++ template header for basic signal-slots. note: I intentionally name it "notify" because in my opinion, the "signal" name belongs to the unix/linux system mechanism.
        -> No multi-threads yet, but I am starting to explore using a working notify signals across threads.
    - [class lus::ui::application]
        Base application class..., will holds and manage the config data of the components, and will manage a threads pool.
    - [ lus::ui::geometry classes ]
        {lus::ui::cxy, lus::ui::size, lus::ui::rectangle, lus::ui::string2d} Base 2D geometry components.
    - [class  lus::object]
        Base Object class on the parent-child relational logistic ( smart-pointers not used there. Too much overload (yet?) )
    -[class  lus::glyph and  lus::accent_fr]
        Actually, some hardcoded unicode and french accent glyphes. Waiting to find a coherant and definitive unicode API.
    -------------------------------------------------------------------------------------------------------------------
    -[ lus::cmd::cmd_switch lus::cmd::line,  with notify-hook]
        Developping my own command-line arguments processor.
        -> It is very hard to create a coherent logistic-model. POSIX or not, the syntax is a mess because there is NO real strict format rules.
    -------------------------------------------------------------------------------------------------------------------
***


***
## tui : (lus::ui):  Standard Ansi Console UI using ...Standard Ansi Codes.
---
    -[namespace lus::terminal]
        Related console/terminal for init, rendering ui::widget components.
    -[class lus::terminal::vchar ]
        Backbuffer packing
        32 bits map packed char to represent the character and its attributes on screen
    -[class lus::ui::widget **]
        - Base ui element (widget) object
    -[ui classes]
        All derived from ui::element (widget), they draw, act/interact according to their name.
---
### Note:
* Backbuffer commits on console are converted to ansi256 (true color) ESC sequences for rendering the text and attributes.
* Not very compatible to non-graphical 16 colors raw tty terminals.
* Management of the ui::widget/windows hierarchy is at application level, not at system level.
* tui is NOT a text console processes/desktop/window manager.
* tui is an Application TUI API using elements (Widgets) on the console for its user interface.
* Not a fast UI renderer especially on Microsoft Windows Terminal where it is very,very, v-e-r-y slow.


***
## dlapi : Basic support for dynamic dll/so components (future) library loader.
---
    Very basic plugins like support manager. [not started yet]
    -[class lus::dlapi]
        File handle object to the live disk shared-object/dll/dylib contents
    -[class lus::dl::plugin ]
        Abstract bridge object between the library loader and the C++ components

    Since there is absolutely NO framework model (such as Windows/COM;OLE, Qt,KDE, [IOS/OSX]:Cocoa ), lus::dlapi must be implemented in higher derived implementation of object instances, using std::function, lambdas and variadic template/ params.
    Maybe there will be a kind of plugin framework implemented in the lus .. Stay tuned ...:)
***

## todo:
---
    ... lots todo! 
---
* Unless otherwise specified, all code in this project is written by the author (Serge Lussier)  and no one else (...unless otherwise specified...).

copyrights 2023,2024, Serge Lussier 



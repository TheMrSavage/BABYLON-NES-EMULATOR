#ifndef EVENTS
    #define EVENTS

    #include <any>
#include <optional>
    enum EVENT_ENUM {
        INTERFACE_NO_EVENT,
        INTERFACE_CLOSE_EVENT,
        INTERFACE_ROOM_OPEN_EVENT,
        INTERFACE_CHANGE_DEBUGGER_STATE,
        INTERFACE_STEP_BY_STEP_DEBUGGER,
        INTERFACE_NEXT_STEP_DEBUGGER
    };
    
    // TODO: Refactor this to remove the std::any to a template (much more secure).
    // I'll do this after having a basic functional code.
    struct event_return {
        EVENT_ENUM e;
        std::optional<std::any> data;
    };
#endif

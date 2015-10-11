#include "CppUTest/TestHarness.h"

TEST_GROUP(mytests)
{
    void setup()
    {

    }

    void teardown()
    {

    }
};

class State
{
public:
    enum id
    {
        CLOSE = 0,
        CONNECTING,
        CONNECTED,
        CLOSING
    };
};

class Event
{
public:
    enum id
    {
        CLOSE = 0,
        NEW,
    };
};

State::id curr_state = State::CLOSE;
Event::id curr_event = Event::CLOSE;

template<State::id state, Event::id event>
bool state_machine_handler()
{
    curr_state = state;
    curr_event = event;

    return true;
}

template<>
bool state_machine_handler<State::CONNECTED, Event::NEW>()
{
    curr_state = State::CLOSING;
    curr_event = Event::CLOSE;

    return true;
}

template<State::id state>
void sm_action(Event::id event)
{
    switch(event)
    {
    case Event::CLOSE:
        state_machine_handler<state, Event::CLOSE>();
        break;
    case Event::NEW:
        state_machine_handler<state, Event::NEW>();
        break;
    default:
        break;
    }
}
void sm_action(State::id state, Event::id event)
{
    switch(state)
    {
    case State::CLOSE:
        sm_action<State::CLOSE>(event);
        break;
    case State::CONNECTING:
        sm_action<State::CONNECTING>(event);
        break;
    case State::CONNECTED:
        sm_action<State::CONNECTED>(event);
        break;
    case State::CLOSING:
        sm_action<State::CLOSING>(event);
        break;
    default:
        break;
    }
}

TEST(mytests, non_type_template_argument)
{
    state_machine_handler<State::CLOSE, Event::NEW>();
    LONGS_EQUAL(State::CLOSE, curr_state);
    LONGS_EQUAL(Event::NEW, curr_event);
}

TEST(mytests, non_type_template_argument_indirect)
{
    sm_action(State::CONNECTED, Event::NEW);
    LONGS_EQUAL(State::CLOSING, curr_state);
    LONGS_EQUAL(Event::CLOSE, curr_event);
}


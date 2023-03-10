#include "event.h"
#include <iostream>

Event::Event(int id){
    event_id = id;
};

EVENT_STATES Event::get_event_state()
{
    return _event_state;
};

void Event::display(){
    printf("%s, id: %d", __PRETTY_FUNCTION__, event_id);
};
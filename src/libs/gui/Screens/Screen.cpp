#include "Screen.hpp"

void Screen::pushQueueElement(struct event_return event) {
    this->event_pool.push(event);
}

struct event_return Screen::popQueueElement() {
    struct event_return top = this->event_pool.front();
    this->event_pool.pop();
    
    return top;
}

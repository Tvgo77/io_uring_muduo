#include <include/EventLoop.h>
#include <stdio.h>

EventLoop::EventLoop() 
  : uring(new Ring(this)),
    activateEvents(EventList(MAX_EVENT)),
    currentEvent(nullptr) {
    ::printf("Log: EventLoop Construct\n");
}

EventLoop::~EventLoop() {
    ::printf("Log: EventLoop destoryed");
}

void EventLoop::loop() {
    quitFlag = false;
    /* start loop*/
    while (!quitFlag) {
        /* Log loop start*/
        ++iteration;
        ::printf("Log: Loop %d start!\n", iteration);

        /* Clear activateEvents*/
        activateEvents.clear();

        /* Use Ring to monitor events occurring */
        uring->monitor(waitTimeSec, &activateEvents);

        /* Handle received Events*/
        for (Event event: activateEvents) {
            currentEvent = event;

            /* Get event assocaited Channel */
            EventOwner* eventOwner = (EventOwner*) currentEvent->user_data;
            currentChannel = eventOwner->channel;

            /* Set receivedEvent of Channel */
            currentChannel->set_receivedEvent(eventOwner->eventType);

            /* Set returnVal of Channel */
            currentChannel->set_returnVal(event->res);

            /* Unset isMonitoring flag of this event type */
            currentChannel->set_isMonitoring(eventOwner->eventType, false);

            /* Handle event*/
            currentChannel->handle_event();
        }

        /* Log loop end*/
        ::printf("Log: Loop %d end!\n", iteration);
    }
}

void EventLoop::quit() { quitFlag = true; }
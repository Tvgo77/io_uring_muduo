#include <include/EventLoop.h>
#include <stdio.h>

EventLoop::EventLoop() 
  : uring(new Ring(this)),
    activateEvents(nullptr),
    numEvents(0),
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
        ::printf("Log: ----------Loop %d start!----------\n", iteration);

        // /* Clear activateEvents*/
        // activateEvents.clear();

        /* Use Ring to monitor events occurring */
        uring->monitor(waitTimeSec, &activateEvents, &numEvents);

        /* Handle received Events*/
        // for (Event event: activateEvents) {
        for (int i = 0; i < numEvents; i++) {
            currentEvent = activateEvents[i];

            /* Get event assocaited Channel */
            EventOwner* eventOwner = (EventOwner*) currentEvent->user_data;
            currentChannel = eventOwner->channel;

            /* Set receivedEvent of Channel */
            currentChannel->set_receivedEvent(eventOwner->eventType);

            /* Set returnVal of Channel */
            currentChannel->set_returnVal(currentEvent->res);

            /* Unset isMonitoring flag of this event type */
            currentChannel->set_isMonitoring(eventOwner->eventType, false);

            /* Handle event*/
            currentChannel->handle_event();

            /* Mark this cqe as seen */
            auto ring = uring->get_io_uring_ptr();
            ::io_uring_cqe_seen(ring, currentEvent);
        }

        /* Log loop end*/
        ::printf("Log: -----------Loop %d end!------------\n", iteration);
    }
}

void EventLoop::quit() { quitFlag = true; }
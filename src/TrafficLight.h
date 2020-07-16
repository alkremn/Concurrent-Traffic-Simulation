#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <queue>
#include <mutex>
#include <condition_variable>

#include "TrafficObject.h"

enum class TrafficLightPhase
{
    red,
    green
};

// Message Queue template class from inter-thread communication
template <class T>
class MessageQueue
{
public:
    MessageQueue(){};
    T recieve();
    void send(T &&);

private:
    std::deque<T> _queue;
    std::mutex _mtx;
    std::condition_variable _cond;
};

class TrafficLight : public TrafficObject
{
public:
    TrafficLight() : _currentPhase(TrafficLightPhase::red), _messages(std::make_shared<MessageQueue<TrafficLightPhase>>()){}
    ~TrafficLight() {}

    void waitForGreen();
    void simulate();
    TrafficLightPhase getCurrentPhase() const { return _currentPhase; };

private:
    //private methods
    void cycleThroughPhases();

    // private members
    TrafficLightPhase _currentPhase;
    std::shared_ptr<MessageQueue<TrafficLightPhase>> _messages;
};

#endif
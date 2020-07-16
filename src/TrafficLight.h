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
    void send(T &&msg)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _queue.emplace_back(std::move(msg));
        _cond.notify_one();
    }
    T recieve()
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _cond.wait(lock, [this]() { return !_queue.empty(); });
        T msg = std::move(_queue.back());
        _queue.pop_back();
        return msg;
    }

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
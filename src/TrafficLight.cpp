#include <random>
#include "TrafficLight.h"

template <class T>
T MessageQueue<T>::recieve()
{
    std::unique_lock<std::mutex> lock(_mtx);
    _cond.wait(lock, [this]() { return !_queue.empty(); });
    T msg = std::move(_queue.back());
    _queue.pop_back();
    return msg;
}

template <class T>
void MessageQueue<T>::send(T &&msg)
{
    std::lock_guard<std::mutex> lock(_mtx);
    _queue.emplace_back(std::move(msg));
    _cond.notify_one();
}

void TrafficLight::waitForGreen()
{
    while (true)
    {
        //
        if (_messages->recieve() == TrafficLightPhase::green && _currentPhase == TrafficLightPhase::green)
            break;
    }
}
void TrafficLight::simulate()
{
    _threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

void TrafficLight::cycleThroughPhases()
{
    double cycleDuration = 4 + (int)(3.0 * rand() / (RAND_MAX + 1.0));
    // init stop watch
    auto lastUpdate = std::chrono::system_clock::now();

    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        long timeSinceUpdate = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - lastUpdate).count();

        if (timeSinceUpdate >= cycleDuration)
        {
            _currentPhase = (_currentPhase == TrafficLightPhase::red) ? TrafficLightPhase::green : TrafficLightPhase::red;
            TrafficLightPhase newPhase = _currentPhase;
            _messages->send(std::move(newPhase));

            // reset stop watch for next cycle
            lastUpdate = std::chrono::system_clock::now();
        }
    }
}
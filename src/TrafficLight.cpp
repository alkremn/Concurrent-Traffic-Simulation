#include "TrafficLight.h"


void TrafficLight::waitForGreen()
{
}
void TrafficLight::simulate()
{
}

void TrafficLight::cycleThroughPhases()
{
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;
    
    // init stop watch
    lastUpdate = std::chrono::system_clock::now();

    while (true)
    {
        
    }
}
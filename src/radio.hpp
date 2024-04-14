#pragma once

#include "mygl/clock.hpp"

class Radio 
{
    public:
        int activation_number;
        int max_activation;
    
        float listening_time;
        float max_listening_time;

        Radio() 
        {
            
        };

    private:
        Clock clock;
};
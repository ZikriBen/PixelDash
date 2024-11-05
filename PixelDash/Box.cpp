#include "Box.h"

void Box::hit(float currentTime) {
    // Only respond to hit if the cooldown has passed
    if (currentTime - lastHitTime >= cooldownTime) {
        lastHitTime = currentTime;  // Update the last hit time
        std::cout << "Box hit!" << std::endl;
        // Trigger any hit response, e.g., reduce health, play animation, etc.
    }
}



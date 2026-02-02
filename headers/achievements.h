
#pragma once
#include "utils.h"
#include <iostream>

class AchievementSystem : public IObserver {
    bool noviceUnlocked = false;
    bool expertUnlocked = false;
public:
    void onNotify(const std::string& eveniment, double valoare) override {
        if (eveniment == "UPDATE_SCOR") {
            int scor = static_cast<int>(valoare);
            if (scor >= 5 && !noviceUnlocked) {
                std::cout << "\n[ACHIEVEMENT] Incepator: Ai strans 5 stele!" << std::endl;
                noviceUnlocked = true;
            }
            if (scor >= 10 && !expertUnlocked) {
                std::cout << "\n[ACHIEVEMENT] Expert: Ai strans 10 stele!" << std::endl;
                expertUnlocked = true;
            }
        }
    }
};
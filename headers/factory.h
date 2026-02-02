#pragma once
#include "elemente.h"
#include <memory>
#include <cstdlib>


class ObstacolFactory {
public:
    static std::unique_ptr<ObiectJoc> creeazaObstacol(int tip, float y) {
        switch (tip) {
            case 0: return std::make_unique<ObstacolRotativ>(y);
            case 1: return std::make_unique<ObstacolPatrat>(y);
            case 2: return std::make_unique<ObstacolDublu>(y);
            case 3: return std::make_unique<ObstacolDouaElice>(y);
            case 4: {
                bool spreDreapta = (std::rand() % 2 == 0);
                return std::make_unique<ObstacolBanda>(y, spreDreapta);
            }
            default: return std::make_unique<ObstacolRotativ>(y);
        }
    }
};
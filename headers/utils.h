#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

template <typename T>
bool esteInInterval(T val, T centru, T raza) {
    return (val >= centru - raza) && (val <= centru + raza);
}

class IObserver {
public:
    virtual void onNotify(const std::string& eveniment, double valoare) = 0;
    virtual ~IObserver() {}
};

class ISubiect {
    std::vector<IObserver*> observatori_;
public:
    void adaugaObservator(IObserver* obs) {
        observatori_.push_back(obs);
    }
    void stergeObservator(IObserver* obs) {
        observatori_.erase(std::remove(observatori_.begin(), observatori_.end(), obs), observatori_.end());
    }
protected:
    void notifica(const std::string& eveniment, double valoare) {
        for (auto* obs : observatori_) {
            obs->onNotify(eveniment, valoare);
        }
    }
};


template <typename T>
class Statistica : public ISubiect {
private:
    T curent_;
    T maxim_;
    std::string nume_;

public:
    Statistica(std::string nume) : curent_(0), maxim_(0), nume_(nume) {}

    void adauga(T val) {
        curent_ += val;
        if (curent_ > maxim_) maxim_ = curent_;
        notifica("UPDATE_" + nume_, static_cast<double>(curent_));
    }

    void setValoare(T val) {
        curent_ = val;
        if (curent_ > maxim_) maxim_ = curent_;
    }

    T getCurent() const { return curent_; }
    T getMaxim() const { return maxim_; }

    void reset() { curent_ = 0; }
};
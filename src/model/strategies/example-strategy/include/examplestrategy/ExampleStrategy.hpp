//
// Copyright 2019 Technische Universität Darmstadt - Fahrzeugtechnik
//

#ifndef MY_OSI_FMU_STRATEGY_HPP
#define MY_OSI_FMU_STRATEGY_HPP

#include <model/include/strategy.hpp>

using namespace model;

class ExampleStrategy : public Strategy {
public:
    using Strategy::Strategy;
    void apply(osi3::SensorData &sensor_data) override;
};
#endif //MY_OSI_FMU_STRATEGY_HPP

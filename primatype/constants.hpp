//Lithio
//January 8, 2021

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

const float kFramerate = 30.0;
const float kPlayerSpeedWalking = 100.0/kFramerate;
const float kPlayerSpeedFlying = 200.0/kFramerate;
const float kPlayerSpeedIncrementWalking = 50.0/kFramerate;
const float kPlayerSpeedIncrementFlying = 10.0/kFramerate;
const float kPlayerDragWalking = kPlayerSpeedIncrementWalking / (kPlayerSpeedWalking + kPlayerSpeedIncrementWalking);
const float kPlayerDragFlying = kPlayerSpeedIncrementFlying / (kPlayerSpeedFlying + kPlayerSpeedIncrementFlying);

#endif

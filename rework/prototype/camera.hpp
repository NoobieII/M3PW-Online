//Lithio
//February 27, 2022

//Basic camera class

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "pwvec2.hpp"

class Player;

namespace camera{
	PWVec2 offset(Player *p);
}

#endif

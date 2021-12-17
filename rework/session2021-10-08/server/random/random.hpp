#ifndef RANDOM_HPP
#define RANDOM_HPP

/*random number functions using Marsenne Twister 19937*/

namespace random{
	void init_rng();
	double rand_double();
	int    rand_int();
}

#endif
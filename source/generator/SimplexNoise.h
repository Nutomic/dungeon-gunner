/*
 * SimplexNoise.h
 *
 *  Created on: 14.06.2013
 *      Author: Felix
 */

#ifndef DG_SIMPLEXNOISE_H_
#define DG_SIMPLEXNOISE_H_

#include <array>
#include <map>

#include "../util/Vector.h"

/**
 * Caching simplex noise generator.
 *
 * The actual simplex noise generator is simplexnoise1234.h/cpp
 * from http://staffwww.itn.liu.se/~stegu/aqsis/aqsis-newnoise/ . See
 * that implementation for more details.
 */
class SimplexNoise {
public:
    SimplexNoise();
    float getNoise(int x, int y);
    float getNoise(const Vector2i& v);


private:
    float noise(float x, float y) const;
    float grad(int hash, float x, float y) const;
    int fastFloor(float f) const;

private:
    std::array<unsigned char, 512> mPerm;
    std::map<int, std::map<int, float> > mCache;
};

#endif /* DG_SIMPLEXNOISE_H_ */

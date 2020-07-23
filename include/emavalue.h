/**
 *  EMAValue.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include <cmath>

class EMAValue
{
private:
    /**
     *  The value
     */
    float _value;

    /**
     *  The alpha
     */
    float _alpha;

public:
    /**
     *  @param  value
     *  @param  alpha
     */
    EMAValue(float value, float alpha=1) : _value(value), _alpha(alpha) {}

    /**
     *  Add to the value
     *  @param  value
     */
    EMAValue &operator+=(float value)
    {
        // update the ema
        _value = _alpha * _value + (1 - _alpha) * value;

        // return ourself for chaining
        return *this;
    }

    /**
     *  Convert to the current value
     */
    operator float() const { return _value; }
};
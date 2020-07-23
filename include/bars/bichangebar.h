/**
 *  BiChangeBarProcessor.h
 *  
 *  @todo incorporate bid/ask
 *
 *  @author Michael van der Werve
 */

#pragma once

#include "processor.h"
#include <cmath>

class BiChangeBarProcessor : public Processor
{
private:
    /**
     *  Max up/down in bips (e.g. 100 = 1%)
     */
    size_t _up = 0;
    size_t _down = 0;

    /**
     *  Function to override, whether or not an element fits
     *  @param  trade
     *  @return bool
     */
    virtual bool fits(const Bar &bar, const Quote &trade) const
    {
        // always allow first trade
        if (bar.size() == 0) return true;

        // calculate the diff
        float diff = bar.trade(0).price() - trade.price();

        // check up
        if (diff >= 0 && diff <= bar.trade(0).price() * (_up / 10000.0)) return true;
        
        // check down
        else if (diff < 0 && diff > bar.trade(0).price() * (_down / -10000.0)) return true;

        // nope, does not dif
        return false;
    }

public:
    /**
     *  Construction for a timebar
     *  @param  up
     *  @param  down
     */
    BiChangeBarProcessor(size_t up, size_t down) : _up(up), _down(down) {}
};
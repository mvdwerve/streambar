/**
 *  ChangeBarProcessor.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include "processor.h"
#include <cmath>

class ChangeBarProcessor : public Processor
{
private:
    /**
     *  Max up/down in bips (e.g. 100 = 1%)
     */
    size_t _bips = 0;

    /**
     *  Function to override, whether or not an element fits
     *  @param  trade
     *  @return bool
     */
    virtual bool fits(const Bar &bar, const Quote &trade) const
    {
        // always allow first trade
        if (bar.size() == 0) return true;

        // bar needs to be at least 500 milliseconds
        if (trade.time() - bar.trade(0).time() < 500) return true;

        // we cannot get in-between milliseconds, so if there is a lot of movement, we need to process everything in this millisecond (at least)
        if (bar.trade(bar.size() - 1).time() == trade.time()) return true;

        // open price of the bar
        float open = bar.trade(0).price();

        // check the first trade with this trade
        return fabs(open - trade.price()) <= (open * (_bips / 10000.0));
    }

public:
    /**
     *  Construction for a timebar
     *  @param  bips    basis points
     */
    ChangeBarProcessor(size_t bips) : _bips(bips) {}
};
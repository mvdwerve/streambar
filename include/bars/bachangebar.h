/**
 *  BAChangeBarProcessor.h
 *  
 *  Bid/ask change bar, for when the midpoint has shifted a certain amount of basis points.
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include "processor.h"
#include <cmath>

class BAChangeBarProcessor : public Processor
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
        // trades always fit in this bar type, we only change on bid/ask divergence
        return true;
    }

    /**
     *  Function to override, whether or not the bid/ask fits
     *  @param  bar
     *  @param  quote
     *  @return bool
     */
    virtual bool fitsBid(const Bar &bar, const Quote &quote) const
    {
        // always allow first trade
        if (bar.size() == 0) return true;

        // bar needs to be at least 500 milliseconds
        if (quote.time() - bar.trade(0).time() < 500) return true;

        // we cannot get in-between milliseconds, so if there is a lot of movement, we need to process everything in this millisecond (at least)
        if (bar.trade(bar.size() - 1).time() == quote.time()) return true;

        // open price of the bar
        float open = (bar.bid(0).price() + bar.ask(0).price()) / 2;

        // check the first trade with this trade
        return fabs(open - quote.price()) <= (open * (_bips / 10000.0));
    }

    /**
     *  Function to override, whether or not the bid/ask fits
     *  @param  bar
     *  @param  quote
     *  @return bool
     */
    virtual bool fitsAsk(const Bar &bar, const Quote &quote) const
    {
        // always allow first trade
        if (bar.size() == 0) return true;

        // bar needs to be at least 500 milliseconds
        if (quote.time() - bar.trade(0).time() < 500) return true;

        // we cannot get in-between milliseconds, so if there is a lot of movement, we need to process everything in this millisecond (at least)
        if (bar.trade(bar.size() - 1).time() == quote.time()) return true;

        // open price of the bar
        float open = (bar.bid(0).price() + bar.ask(0).price()) / 2;

        // check the first trade with this trade
        return fabs(open - quote.price()) <= (open * (_bips / 10000.0));
    }

public:
    /**
     *  Construction for a timebar
     *  @param  bips    basis points
     */
    BAChangeBarProcessor(size_t bips) : _bips(bips) {}
};
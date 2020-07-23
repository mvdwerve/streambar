/**
 *  TickBarProcessor.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include "processor.h"

class TickBarProcessor : public Processor
{
private:
    /**
     *  Maximumum bar size
     */
    size_t _max = 0;

    /**
     *  Function to override, whether or not an element fits
     *  @param  trade
     *  @return bool
     */
    virtual bool fits(const Bar &bar, const Quote &trade) const
    {
        // only fit if the bar is still small enough
        return bar.size() < _max;
    }

public:
    /**
     *  Construction for a timebar
     *  @param  max
     */
    TickBarProcessor(size_t max) : _max(max) {}
};
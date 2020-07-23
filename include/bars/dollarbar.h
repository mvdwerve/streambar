/**
 *  DollarBarProcessor.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include "processor.h"

class DollarBarProcessor : public Processor
{
private:
    /**
     *  Maximumum bar size (volume * price)
     */
    double _max = 0;

    /**
     *  Running volume * price for current bar
     */
    double _running = 0;
    
    /**
     *  Function to override, whether or not an element fits
     *  @param  trade
     *  @return bool
     */
    virtual bool fits(const Bar &bar, const Quote &trade) const
    {
        // only fit if the bar is still small enough
        return _running < _max;
    }

    /**
     *  Method that is called when a quote was really added to a bar
     *  @param  trade
     */
    virtual void onAdded(const Bar &bar, const Quote &trade) 
    {
        // add to the volume
        _running += trade.size() * trade.price();
    }

    /**
     *  Method that is called when the object resets
     *  @param  bar
     */
    virtual void onCompleted(const Bar &bar) { _running = 0; }

public:
    /**
     *  Construction for a dollarbar
     *  @param  max
     */
    DollarBarProcessor(double max) : _max(max) {}
};
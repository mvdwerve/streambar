/**
 *  VolumeBarProcessor.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include "processor.h"

class VolumeBarProcessor : public Processor
{
private:
    /**
     *  Maximumum bar size
     */
    size_t _max = 0;

    /**
     *  Running volume for current bar
     */
    size_t _running = 0;

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
        _running += trade.size();
    }

    /**
     *  Method that is called when the object resets
     *  @param  bar
     */
    virtual void onCompleted(const Bar &bar) 
    { 
        // we simply reset the running count
        _running = 0; 
    }

public:
    /**
     *  Construction for a timebar
     *  @param  max
     */
    VolumeBarProcessor(size_t max) : _max(max) {}
};
/**
 *  TimeBarProcessor.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include "processor.h"

class TimeBarProcessor : public Processor
{
private:
    /**
     *  Number of seconds that should be in the time bar
     */
    size_t _seconds = 60;
    
    /**
     *  Current time box
     */
    size_t _box = 0;

    /**
     *  Function to override, whether or not an element fits
     *  @param  trade
     *  @return bool
     */
    virtual bool fits(const Bar &bar, const Quote &trade) const
    {
        // only fit if the bar is still small enough
        return bar.size() == 0 || box(trade) == _box;
    }

    /**
     *  Method that is called when a quote was really added to a bar
     *  @param  trade
     */
    virtual void onAdded(const Bar &bar, const Quote &trade) 
    {
        // set the box
        _box = box(trade);
    }

    /**
     *  Method that is called when the object resets
     *  @param  bar
     */
    virtual void onCompleted(const Bar &bar) 
    { 
        // no idea which box any more now, presumably the next
        // @todo fix addition of missing boxes
        _box = 0; 
    }


    /**
     *  Box the time should fall in
     *  @return size_t
     */
    size_t box(const Quote &quote) const
    {
        // divide by the number of seconds
        return quote.time() / (_seconds * 1000);
    }

public:
    /**
     *  Construction for a timebar
     *  @param  seconds
     */
    TimeBarProcessor(size_t seconds) : _seconds(seconds) {}
};
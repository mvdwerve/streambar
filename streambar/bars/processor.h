/**
 *  Processor.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include "../bar.h"
#include "../quote.h"
#include <memory>

/**
 *  Interface for a bar type
 */
class Processor
{
public:
    /**
     *  Function to override, whether or not an element fits
     *  @param  trade
     *  @return bool
     */
    virtual bool fits(const Bar &bar, const Quote &trade) const = 0;

    /**
     *  Function to override, whether or not the bid/ask fits
     *  @param  bar
     *  @param  quote
     *  @return bool
     */
    virtual bool fitsBid(const Bar &bar, const Quote &quote) const { return true; }
    virtual bool fitsAsk(const Bar &bar, const Quote &quote) const { return true; }

    /**
     *  Method that is called when a quote was really added to a bar
     *  @param  trade
     */
    virtual void onAdded(const Bar &bar, const Quote &trade) {}

    /**
     *  Method that is called when the bar is completed
     *  @param bar
     */
    virtual void onCompleted(const Bar &bar) {}
};
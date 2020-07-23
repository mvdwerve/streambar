/**
 *  ImbalanceBarProcessor.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include "processor.h"
#include "../emavalue.h"

class ImbalanceBarProcessor : public Processor
{
protected:
    /**
     *  Exponential moving average of T of previous bars used to approximate expectation of bar length.
     */
    EMAValue _T;

    /**
     *  Exponential moving average of the right term
     */ 
    EMAValue _b;
    
    /**
     *  Expectation for theta_T, given the known factors
     */
    float _E_theta_T = 0;

    /**
     *  Current theta_T, will rise and fall as ticks come in.
     */
    float _theta_T = 0;

    /**
     *  Whether or not we're still on the initial bar
     */
    bool _initial = true;

    /**
     *  Function to override, whether or not an element fits
     *  @param  trade
     *  @return bool
     */
    virtual bool fits(const Bar &bar, const Quote &trade) const
    {
        // on the initial bar, we want _at least_ EMA_T ticks in there
        if (_initial && bar.size() < _T) return true; 

        // check if still within bounds
        return fabs(_theta_T) < _E_theta_T;
    }

    /**
     *  Method that is called when a quote was really added to a bar
     *  @param  trade
     */
    virtual void onAdded(const Bar &bar, const Quote &trade) = 0;

    /**
     *  Called when a bar was completed
     *  @param  bar
     */
    virtual void onCompleted(const Bar &bar)
    {
        // change the ema for T
        _T += bar.size();

        // reset theta_T
        _theta_T = 0;

        // recalculate E_theta_T right now
        _E_theta_T = _T * fabs(_b);

        // no longer on initial bar
        _initial = false;
    }

    /**
     *  Constructor
     *  @param  T
     *  @param  b
     */
    ImbalanceBarProcessor(EMAValue T, EMAValue b) : _T(T), _b(b) {}
};
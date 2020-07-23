/**
 *  TickRunsBarProcessor.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include "processor.h"
#include <algorithm>
#include "../emavalue.h"

class RunsBarProcessor : public Processor
{
protected:
    /**
     *  Exponential moving average of T of previous bars used to approximate expectation of bar length.
     */
    EMAValue _T = 0;

    /**
     *  Exponential moving average of the number of buys/sells from all previous bars
     */ 
    EMAValue _buys = 0;
    EMAValue _sells = 0;
    
    /**
     *  Expectation for theta_T, given the known factors
     */
    float _E_theta_T = 0;

    /**
     *  Number bought/sold in the current bar
     */
    float _bought = 0;
    float _sold = 0;

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
        return std::max(_bought, _sold) < _E_theta_T;
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

        // update the EMAs
        _buys += _bought;
        _sells += _sold;

        // reset bought & sold
        _bought = 0;
        _sold = 0;

        // no longer on initial bar
        _initial = false;
    }

public:
    /**
     *  Construction for a timebar
     *  @param  T   
     *  @param  buys
     *  @param  sells
     */
    RunsBarProcessor(EMAValue T=100, EMAValue buys=100, EMAValue sells=100) : 
        _T(T), _buys(buys), _sells(sells) {}
};
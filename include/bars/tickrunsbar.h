/**
 *  TickRunsBarProcessor.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include "runsbar.h"

class TickRunsBarProcessor : public RunsBarProcessor
{
private:
    /**
     *  Method that is called when a quote was really added to a bar
     *  @param  trade
     */
    virtual void onAdded(const Bar &bar, const Quote &trade) 
    {
        // apply the tick rule
        int b_t = bar.tick(bar.size() - 1);

        // add the imbalance
        if (b_t > 0) _bought += b_t;
        else if (b_t < 0) _sold += -b_t;

        // if this was the initial bar, and we are now on the threshold, we calculate the expected value
        if (!_initial || bar.size() != _T) return;
        
        // calculate the total number
        float total = _buys + _sells;

        // recalculate
        _E_theta_T = _T * fmax(_buys / total, _sells / total);
    }

    /**
     *  Called when a bar was completed
     *  @param  bar
     */
    virtual void onCompleted(const Bar &bar)
    {
        // call the base
        RunsBarProcessor::onCompleted(bar);

        // calculate the total number
        float total = _buys + _sells;
        
        // recalculate E_theta_T
        _E_theta_T = _T * fmax(_buys / total, _sells / total);
    }

public:
    /**
     *  Construction for a timebar
     *  @param  T   
     *  @param  buys
     *  @param  sells
     */
    TickRunsBarProcessor(EMAValue T=100, EMAValue buys=100, EMAValue sells=100) : 
        RunsBarProcessor(T, buys, sells) {}
};
/**
 *  VolumeRunsBarProcessor.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include "runsbar.h"

class VolumeRunsBarProcessor : public RunsBarProcessor
{
private:
    /**
     *  Method that is called when a quote was really added to a bar
     *  @param  trade
     */
    virtual void onAdded(const Bar &bar, const Quote &trade) 
    {
        // apply the tick rule
        int b_t = bar.tick(bar.size() - 1) * trade.size();

        // add the imbalance
        if (b_t > 0) _bought += b_t;
        else if (b_t < 0) _sold += -b_t;

        // if this was the initial bar, and we are now on the threshold, we calculate the expected value
        if (!_initial || bar.size() != _T) return;
        
        // calculate the total number
        float total = _buys + _sells;

        // recalculate
        _E_theta_T = _T * fmax(_buys / total * _buys, _sells / total * _sells);
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
        _E_theta_T = _T * fmax(_buys / total * _buys, _sells / total * _sells);
    }

public:
    /**
     *  Construction for a timebar
     *  @param  T   
     *  @param  buys
     *  @param  sells
     */
    VolumeRunsBarProcessor(EMAValue T=100, EMAValue buys=1000, EMAValue sells=1000) : 
        RunsBarProcessor(T, buys, sells) {}
};
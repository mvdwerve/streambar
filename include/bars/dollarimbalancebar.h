/**
 *  DollarImbalanceBarProcessor.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include "imbalancebar.h"

class DollarImbalanceBarProcessor : public ImbalanceBarProcessor
{
private:
    /**
     *  Method that is called when a quote was really added to a bar
     *  @param  trade
     */
    virtual void onAdded(const Bar &bar, const Quote &trade) 
    {
        // apply the tick rule
        int b_t = bar.tick(bar.size() - 1) * trade.size() * trade.price();

        // add the imbalance
        _theta_T += b_t;

        // change b
        _b += b_t;

        // if this was the initial bar, and we are now on the threshold, we calculate the expected value
        if (_initial && bar.size() == _T) _E_theta_T = _T * fabs(_b);
    }

    /**
     *  Called when a bar was completed
     *  @param  bar
     */
    virtual void onCompleted(const Bar &bar)
    {
        // call base method
        ImbalanceBarProcessor::onCompleted(bar);

        // recalculate E_theta_T right
        _E_theta_T = _T * fabs(_b);
    }

public:
    /**
     *  Construction for a timebar
     *  @param  E_T         Expected number of ticks in the bar, used as minimum number of ticks in first bar
     *  @param  alpha_b     EMA factor of (2v+ - E[v_t]) (EMA_b)
     */
    DollarImbalanceBarProcessor(float E_T=100, float alpha_b=0.95) : 
        ImbalanceBarProcessor(E_T, EMAValue(0, alpha_b)) {}
};
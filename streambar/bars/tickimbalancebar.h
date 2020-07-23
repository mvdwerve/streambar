/**
 *  TickImbalanceBarProcessor.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include "processor.h"
#include "imbalancebar.h"

class TickImbalanceBarProcessor : public ImbalanceBarProcessor
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

        // recalculate E_theta_T
        _E_theta_T = _T * fabs(_b);
    }

public:
    /**
     *  Construction for a timebar
     *  @param  E_T         Expected number of ticks in the bar, used as minimum number of ticks in first bar
     *  @param  P_b         Probability of buys
     *  @param  alpha_b     EMA factor of 2p[b = 1] - 1 (EMA_b)
     */
    TickImbalanceBarProcessor(float E_T=100, float P_b=0.5, float alpha_b=0.95) : 
        ImbalanceBarProcessor(E_T, EMAValue(2 * P_b - 1, alpha_b)) {}
};
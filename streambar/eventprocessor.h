/**
 *  EventProcessor
 */

#pragma once

class EventProcessor
{
public:
    /**
     *  Process a trade
     *  @param  time
     *  @param  price
     *  @param  size
     */
    virtual void onTrade(const Quote &trade) = 0;

    /**
     *  Process a bid
     *  @param  time
     *  @param  price
     *  @param  size
     */
    virtual void onBid(const Quote &bid) = 0;

    /**
     *  Process an ask
     *  @param time
     *  @param price
     *  @param size
     */
    virtual void onAsk(const Quote &ask) = 0;
};
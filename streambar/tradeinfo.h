/**
 *  TradeInfo.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include "quote.h"

class TradeInfo 
{
private:
    /**
     *  Actual trade
     */
    Quote _trade;

    /**
     *  Best bid at the time of the trade
     */
    Quote _bid;

    /**
     *  Best ask at the time of the trade
     */
    Quote _ask;

    /**
     *  Whether or not it's UP (1) or DOWN (-1)
     */
    int8_t _tick;

public:
    /**
     *  Constructor for the actual trade
     *  @param  trade
     *  @param  bid
     *  @param  ask
     *  @param  tick
     */
    TradeInfo(const Quote &trade, const Quote &bid, const Quote &ask, int8_t tick) : _trade(trade), _bid(bid), _ask(ask), _tick(tick) {}

    /**
     *  Get the trade/bid/ask
     *  @return const Quote&
     */
    const Quote &trade() const { return _trade; }
    const Quote &ask()   const { return _ask; }
    const Quote &bid()   const { return _bid; }

    /**
     *  Get the tick
     */
    int8_t tick() const { return _tick; }
};
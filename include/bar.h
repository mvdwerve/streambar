/**
 *  Bar.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include <vector>
#include "tradeinfo.h"
#include <memory>

class Bar
{
public:
    class Handler
    {
    public:
        /**
         *  Called when a bar is fully done.
         *  @param  bar
         */
        virtual void onBar(const std::shared_ptr<Bar> &bar) = 0;
    };
private:
    /**
     *  Trades in the bar.
     */
    std::vector<TradeInfo> _trades;

    /**
     *  Last bar
     */
    std::shared_ptr<Bar> _last;

    /**
     *  Tick rule
     *  @param  price
     *  @return int
     */
    int tickrule(float price)
    {
        // if we neither have a previous bar, we cannot know
        if (_trades.size() == 0 && !_last) return 0;

        // last price (might be from previous bar)
        float last = (_trades.size() == 0) ? _last->trade(_last->size() - 1).price() : trade(size() - 1).price();

        // if the price is the same as the last price, return the last action
        if (price == last) return (_trades.size() == 0) ? _last->tick(_last->size() - 1) : tick(size() - 1);

        // if the price is higher, return 'buy', otherwise, 'sell
        return (price > last) ? 1 : -1;
    }

public:
    /**
     *  A single bar
     *  @param  bar     previous bar (needed for tick rule)
     */
    Bar(const std::shared_ptr<Bar> &bar = nullptr) : _last(bar) {}

    /**
     *  Add to the bar
     *  @param  trade
     *  @param  bid
     *  @param  ask
     */
    void add(const Quote &trade, const Quote &bid, const Quote &ask)
    {
        // append to the trades
        _trades.emplace_back(trade, bid, ask, tickrule(trade.price()));
    }

    /**
     *  Get the bar length
     *  @return size_t
     */
    size_t size() const { return _trades.size(); }

    /**
     *  Get the trades
     *  @return std::vector
     */
    const std::vector<TradeInfo> &trades() const { return _trades; }

    /**
     *  Get the trade/bid/ask at specific position
     *  @param  size_t
     */
    const Quote &trade(size_t idx) const { return _trades[idx].trade(); }
    const Quote &bid(size_t idx) const { return _trades[idx].bid(); } 
    const Quote &ask(size_t idx) const { return _trades[idx].ask(); } 

    /**
     *  Get access to the tick info
     *  @param  size_t
     */
    int8_t tick(size_t idx) const { return _trades[idx].tick(); }
};
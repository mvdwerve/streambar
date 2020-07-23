
#pragma once

#include "eventprocessor.h"

class NegSpreads : public EventProcessor
{
private:
    /**
     *  output stream
     */
    std::ostream &_output;
    
    /**
     *  Last bid/ask
     */
    Quote _ask;
    Quote _bid;

public:
    // 5 hours from GMT
    NegSpreads(std::ostream &output) : _output(output)
    {
        // write header to the output
        _output << "event,time,price,size,bid,ask\n";
    }

    /**
     *  Process a trade
     *  @param  quote
     */
    virtual void onTrade(const Quote &quote) override
    {
        // don't do anything yet if the bid/ask is not valid
        if (!_bid.valid() || !_ask.valid()) return;

        // check the prices
        if (_bid.price() <= _ask.price()) return;

        // output trade line
        _output << "1," << quote.time() << "," << quote.price() << "," << quote.size() << "," << _bid.price() << "," << _ask.price() << "\n";
    }
    
    /**
     *  Process a bid
     *  @param  bid
     */
    virtual void onBid(const Quote &bid) override
    {        
        _bid = bid;
    }

    /**
     *  Process an ask
     *  @param ask
     */
    virtual void onAsk(const Quote &ask) override
    {
        _ask = ask;
    }
};
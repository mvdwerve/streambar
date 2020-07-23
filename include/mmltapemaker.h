
#pragma once

#include "eventprocessor.h"

class MmlTapeMaker : public EventProcessor
{
private:
    /**
     *  output stream
     */
    std::ostream &_output;

    /**
     *  Offset in the day
     */
    size_t _offset = 0;

    /**
     *  Start and end timestamps
     */
    size_t _start = 0;
    size_t _end = 0;
    
    /**
     *  Last bid/ask
     */
    Quote _ask;
    Quote _bid;

public:
// 5 hours from GMT
    MmlTapeMaker(std::ostream &output, size_t offset, size_t start, size_t end) : _output(output), _offset(offset - 3600*1000*5), _start(start), _end(end)
    {
        // write header to the output
        _output << "event,time,price,size\n";
    }

    /**
     *  Process a trade
     *  @param  quote
     */
    virtual void onTrade(const Quote &quote) override
    {
        // ignore any trades before our 'start' and after our 'end'
        if (quote.time() < _start || quote.time() > _end) return;

        // don't do anything yet if the bid/ask is not valid
        if (!_bid.valid() || !_ask.valid()) return;

        // output trade line
        _output << "1," << (quote.time() + _offset) << "," << quote.price() << "," << quote.size() << "\n";
    }
    
    /**
     *  Process a bid
     *  @param  bid
     */
    virtual void onBid(const Quote &bid) override
    {        
        _bid = bid;

        // ignore any trades before our 'start' and after our 'end'
        if (bid.time() < _start || bid.time() > _end) return;

        // output bid line
        _output << "2," << (bid.time() + _offset) << "," << bid.price() << "," << bid.size() << "\n";
    }

    /**
     *  Process an ask
     *  @param ask
     */
    virtual void onAsk(const Quote &ask) override
    {
        _ask = ask;

        // ignore any trades before our 'start' and after our 'end'
        if (ask.time() < _start || ask.time() > _end) return;

        // output trade line
        _output << "3," << (ask.time() + _offset) << "," << ask.price() << "," << ask.size() << "\n";
    }
};
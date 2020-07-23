/**
 *  BarMaker.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include "bar.h"
#include "quote.h"
#include "bars/processor.h"
#include "eventprocessor.h"
#include <memory>

class BarMaker : public EventProcessor
{
private:
    /**
     *  Bar handler
     */
    Bar::Handler *_handler; 

    /**
     *  Actual processor
     */
    Processor *_processor;

    /**
     *  The shared pointer to the bar
     */
    std::shared_ptr<Bar> _bar;


    /**
     *  Last bid/ask
     */
    Quote _ask;
    Quote _bid;

    /**
     *  Helper method to reset the BarMaker.
     */
    void reset()
    {
        // if there is a bar, emit it
        if (_bar) 
        {
            // call the handler
            _handler->onBar(_bar);

            // and the processor
            _processor->onCompleted(*_bar);
        }

        // create a new bar
        _bar = std::make_shared<Bar>(_bar);
    }

public:
    /**
     *  Constructor for the BarMaker
     *  @param  handler
     *  @param  processor
     */
    BarMaker(Bar::Handler *handler, Processor *processor) :
        _handler(handler), _processor(processor) {}

    /**
     *  Destructor, will emit the last bar if there is still an open one
     */
    virtual ~BarMaker()
    {
        // if there is still a bar open, emit it now
        if (_bar) _handler->onBar(_bar);
    }

    /**
     *  Process a trade
     *  @param  time
     *  @param  price
     *  @param  size
     */
    virtual void onTrade(const Quote &trade) override
    {
        // don't do anything yet if the bid/ask is not valid
        if (!_bid.valid() || !_ask.valid()) return;

        // if the quote is not within 5% of it, we drop it (it is suspect), and leap out
        if (trade.price() < _bid.price() * 0.95 || trade.price() > _ask.price() * 1.05) return;

        // if there is no current bar, or it does not fit in the current bar,
        // emit the bar and reset the object (creates a new bar) 
        if (!_bar || !_processor->fits(*_bar, trade)) reset();

        // add the trade
        _bar->add(trade, _bid, _ask);

        // trade has been added to the bar
        _processor->onAdded(*_bar, trade);
    }

    /**
     *  Process a bid
     *  @param  time
     *  @param  price
     *  @param  size
     */
    virtual void onBid(const Quote &bid) override
    {
        // simply remember
        _bid = bid;

        // don't do anything yet if the bid/ask is not valid
        if (!_bid.valid() || !_ask.valid()) return;

        // if there is no current bar, or it does not fit in the current bar,
        // emit the bar and reset the object (creates a new bar) 
        if (!_bar || !_processor->fitsBid(*_bar, bid)) reset();
    }

    /**
     *  Process an ask
     *  @param time
     *  @param price
     *  @param size
     */
    virtual void onAsk(const Quote &ask) override
    {
        // simply remember
        _ask = ask;

        // don't do anything yet if the bid/ask is not valid
        if (!_bid.valid() || !_ask.valid()) return;

        // if there is no current bar, or it does not fit in the current bar,
        // emit the bar and reset the object (creates a new bar) 
        if (!_bar || !_processor->fitsAsk(*_bar, ask)) reset();
    }

    /**
     *  Flush it
     */
    void flush()
    {
        // trade has been added to the bar
        reset();
    }
};
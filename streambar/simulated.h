
#pragma once

#include "eventprocessor.h"

class Simulated : public EventProcessor
{
private:
    /**
     *  Input stream
     */
    std::istream &_input;
    std::ostream &_output;

    /**
     *  Last bid/ask
     */
    Quote _ask;
    Quote _bid;

    size_t _next = 0;
    size_t _nextsize = 0;

    size_t _bought = 0;

    /**
     *  Helper method to process the next action
     */
    void processAction()
    {
        // process the current line
        _bought += _nextsize;

        std::string line;

        // parse the line
        std::getline(_input, line);

        // get the next and next size
        _next = static_cast<int>(atof(line.c_str()) * 1000);
        _nextsize = static_cast<int>(atoi(strchr(line.c_str(), ',') + 1));
    }

public:
    Simulated(std::istream &input, std::ostream &output) : _input(input), _output(output) 
    {
        // take one line (header)
        std::string line;
        std::getline(_input, line);

        // check the header
        if (line != "timestamp,size") throw std::runtime_error("incorrect header");

        // parse the line
        std::getline(_input, line);

        // get the next and next size
        _next = static_cast<int>(atof(line.c_str()) * 1000);
        _nextsize = static_cast<int>(atoi(strchr(line.c_str(), ',') + 1));
    }

    /**
     *  Process a trade
     *  @param  time
     *  @param  price
     *  @param  size
     */
    virtual void onTrade(const Quote &trade) override
    {
        // ignore for now, nothing to do here
    }
    
    /**
     *  Process a bid
     *  @param  time
     *  @param  price
     *  @param  size
     */
    virtual void onBid(const Quote &bid) override
    {
        // before updating, we may need to process some actions
        while (bid.time() > _next) processAction();

        // simply remember
        _bid = bid;
    }

    /**
     *  Process an ask
     *  @param time
     *  @param price
     *  @param size
     */
    virtual void onAsk(const Quote &ask) override
    {
        // before update, we may need to process actions
        while (ask.time() > _next) processAction();

        // simply remember
        _ask = ask;
    }
};
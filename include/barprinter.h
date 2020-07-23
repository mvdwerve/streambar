/**
 *  BarPrinter.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

#include <iostream>
#include <cmath>
#include <algorithm>
#include "bar.h"

class BarPrinter : public Bar::Handler
{
private:
    /**
     *  The output stream
     */
    std::ostream &_stream;

    /**
     *  Number of bars emitted
     */
    size_t _number = 0;

public:
    /**
     *  Constructor for the bar printer
     */
    BarPrinter(std::ostream &ostream) : _stream(ostream)
    {
        // print the bar beginning
        _stream << "open,high,low,close,bid_price,bid_size,ask_price,ask_size,first,last,volume,dollars,trades,vwap,std,mad,skewness,kurtosis,buys,sells,buy_volume,sell_volume" << std::endl;
    }

    /**
     *  Free function that calculates the bar open price
     *  @param  bar
     */
    float bar_open(const std::shared_ptr<Bar> &bar)
    {
        // first price
        return bar->trade(0).price();
    }

    /**
     *  Free function that calculates the bar high price
     *  @param bar
     */ 
    float bar_high(const std::shared_ptr<Bar> &bar)
    {
        // current max
        float max = 0.0;

        // iterate over all the trades
        for (const auto &info : bar->trades())
        {
            // always take the maximum
            max = std::max(info.trade().price(), max);
        }

        // return the maximum
        return max;
    }

    /**
     *  Free function that calculates the bar low price 
     *  @param  bar
     */
    float bar_low(const std::shared_ptr<Bar> &bar)
    {
        // current minimum (@todo fix flt_max)
        float min = 999999999.0;

        // iterate over all the trades
        for (const auto &info : bar->trades())
        {
            // always take the minimum
            min = std::min(info.trade().price(), min);
        }

        // return the minimum
        return min;
    }

    /**
     *  Free function that calculates the bar close price
     *  @param  bar
     */
    float bar_close(const std::shared_ptr<Bar> &bar)
    {
        // last price
        return bar->trade(bar->size() - 1).price();
    }

    /**
     *  Free function that calculates the bar low price 
     *  @param  bar
     */
    float bar_vwap(const std::shared_ptr<Bar> &bar)
    {
        // total volume
        size_t volume = 0;

        // total volume * price
        float total = 0.0;

        // iterate over all the trades
        for (const auto &info : bar->trades())
        {
            // always take the minimum
            volume += info.trade().size();

            // add to the total
            total += info.trade().size() * info.trade().price();
        }

        // divide by the total volume, and we get the average price
        return total / volume;
    }

    /**
     *  Free function that calculates the bar standard deviation
     *  @param  bar
     */
    float bar_vwap_std(const std::shared_ptr<Bar> &bar)
    {
        // total volume
        size_t volume = 0;

        // total volume * price
        float vwap = bar_vwap(bar);

        // the totla
        float total = 0;

        // iterate over all the trades
        for (const auto &info : bar->trades())
        {
            // always take the minimum
            volume += info.trade().size();

            // add to the total
            total += info.trade().size() * pow(info.trade().price() - vwap, 2);
        }

        // safety to prevert NaN
        if (total < 1e-6) return 0.0;

        // divide by the total volume, and we get the average price
        return sqrt(total / volume);
    }

    /**
     *  Free function that calculates the bar mean absolute deviation (MAD) 
     *  @param  bar
     */
    float bar_vwap_mad(const std::shared_ptr<Bar> &bar)
    {
        // total volume
        size_t volume = 0;

        // total volume * price
        float vwap = bar_vwap(bar);

        // the totla
        float total = 0;

        // iterate over all the trades
        for (const auto &info : bar->trades())
        {
            // always take the minimum
            volume += info.trade().size();

            // add to the total
            total += info.trade().size() * fabs(info.trade().price() - vwap);
        }

        // safety to prevert NaN
        if (total < 1e-6) return 0.0;

        // divide by the total volume, and we get the average price
        return total / volume;
    }

    /**
     *  Free function that calculates the bar skewness
     *  @param  bar
     */
    float bar_vwap_skewness(const std::shared_ptr<Bar> &bar)
    {
        // total volume
        size_t volume = 0;

        // total volume * price
        float vwap = bar_vwap(bar);

        // find the std
        float std = bar_vwap_std(bar);

        // if there is no standard deviation, leap out
        if (std == 0.0) return 0.0;

        // the totla
        float total = 0;

        // iterate over all the trades
        for (const auto &info : bar->trades())
        {
            // always take the minimum
            volume += info.trade().size();

            // add to the total
            total += info.trade().size() * pow((info.trade().price() - vwap) / std, 3);
        }

        // safety to prevert NaN
        if (total < 1e-6) return 0.0;

        // divide by the total volume, and we get the average price
        return total / volume;
    }

    /**
     *  Free function that calculates the bar kurtosis
     *  @param  bar
     */
    float bar_vwap_kurtosis(const std::shared_ptr<Bar> &bar)
    {
        // total volume
        size_t volume = 0;

        // total volume * price
        float vwap = bar_vwap(bar);

        // find the std
        float std = bar_vwap_std(bar);

        // if there is no standard deviation, leap out
        if (std == 0.0) return 0.0;

        // the totla
        float total = 0;

        // iterate over all the trades
        for (const auto &info : bar->trades())
        {
            // always take the minimum
            volume += info.trade().size();

            // add to the total
            total += info.trade().size() * pow((info.trade().price() - vwap) / std, 4);
        }

        // safety to prevert NaN
        if (total < 1e-6) return 0.0;

        // divide by the total volume, and we get the average price
        return total / volume;
    }

    /**
     *  Free function that calculates the first timestamp in the bar
     *  @param  bar
     */
    size_t bar_first(const std::shared_ptr<Bar> &bar)
    {
        // first price
        return bar->trade(0).time();
    }

    /**
     *  Free function that calculates the last timestamp in the bar
     *  @param  bar
     */
    size_t bar_last(const std::shared_ptr<Bar> &bar)
    {
        // last timestamp
        return bar->trade(bar->size() - 1).time();
    }

    /**
     *  Free function that calculates the bar volume
     *  @param  bar
     */
    size_t bar_volume(const std::shared_ptr<Bar> &bar)
    {
        // total volume
        size_t total = 0;

        // iterate over the bar
        for (const auto &info : bar->trades()) { total += info.trade().size(); }

        // total volume
        return total;
    }

    /**
     *  Free function that calculates the bar volume * price
     *  @param  bar
     */
    double bar_dollars(const std::shared_ptr<Bar> &bar)
    {
        // total volume * price
        double total = 0;

        // iterate over the bar
        for (const auto &info : bar->trades()) { total += info.trade().size() * info.trade().price(); }

        // total volume * price
        return total;
    }

    /**
     *  Free function that calculates the number of trades in a bar
     *  @param  bar
     */
    size_t bar_trades(const std::shared_ptr<Bar> &bar)
    {
        // already exposed
        return bar->size();
    }

    /**
     *  Free function that exposes the number of buys
     */
    size_t bar_buys(const std::shared_ptr<Bar> &bar)
    {
        // total volume
        size_t total = 0;

        // iterate over the bar
        for (const auto &info : bar->trades()) 
        { 
            // only positive ticks
            if (info.tick() > 0) ++total; 
        }

        // total volume
        return total;
    }

    /**
     *  Free function that exposes the number of sells
     */
    size_t bar_sells(const std::shared_ptr<Bar> &bar)
    {
        // total volume
        size_t total = 0;

        // iterate over the bar
        for (const auto &info : bar->trades()) 
        { 
            // only negative ticks
            if (info.tick() < 0) ++total; 
        }

        // total volume
        return total;  
    }

    /**
     *  Free function that exposes the number of buys
     */
    size_t bar_buy_volume(const std::shared_ptr<Bar> &bar)
    {
        // total volume
        size_t total = 0;

        // iterate over the bar
        for (const auto &info : bar->trades()) 
        { 
            // only positive ticks
            if (info.tick() > 0) total += info.trade().size(); 
        }

        // total volume
        return total;
    }

    /**
     *  Free function that exposes the number of sells
     */
    size_t bar_sell_volume(const std::shared_ptr<Bar> &bar)
    {
        // total volume
        size_t total = 0;

        // iterate over the bar
        for (const auto &info : bar->trades()) 
        { 
            // only negative ticks
            if (info.tick() < 0) total += info.trade().size(); 
        }

        // total volume
        return total;  
    }
    
    /**
     *  Called when a bar is fully done.
     *  @param  bar
     */
    virtual void onBar(const std::shared_ptr<Bar> &bar) override
    {
        // if the bar is not valid, leap out
        if (!bar || bar->size() == 0) return;

        // check if the last trade has a bid and an ask
        if (!bar->bid(0).valid() || !bar->ask(0).valid()) return;

        // print the bar
        _stream  
            << bar_open(bar)
            << "," << bar_high(bar)
            << "," << bar_low(bar)
            << "," << bar_close(bar)
            << "," << bar->bid(bar->size() - 1).price()
            << "," << bar->bid(bar->size() - 1).size()
            << "," << bar->ask(bar->size() - 1).price()
            << "," << bar->ask(bar->size() - 1).size()
            << "," << bar_first(bar)
            << "," << bar_last(bar)
            << "," << bar_volume(bar)
            << "," << bar_dollars(bar)
            << "," << bar_trades(bar)
            << "," << bar_vwap(bar)
            << "," << bar_vwap_std(bar)
            << "," << bar_vwap_mad(bar)
            << "," << bar_vwap_skewness(bar)
            << "," << bar_vwap_kurtosis(bar)
            << "," << bar_buys(bar)
            << "," << bar_sells(bar)
            << "," << bar_buy_volume(bar)
            << "," << bar_sell_volume(bar)
            << "\n";

        // one more bar
        _number++;
    }       

    size_t number() const { return _number; }
};
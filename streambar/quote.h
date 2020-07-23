/**
 *  Quote.h
 *  
 *  @author Michael van der Werve
 */

#pragma once

class Quote
{
private:
    /**
     *  Time of the quote
     */
    size_t _time = 0;
    
    /**
     *  Quoted price (@todo decimal)
     */
    float _price = 0.0;
    
    /**
     *  Size of the quote
     */
    size_t _size = 0;

public:
    /**
     *  Default constructor
     */
    Quote() = default;

    /**
     *  Constructor for a quote
     *  @param  time
     *  @param  price
     *  @param  size
     */
    Quote(size_t time, float price, size_t size) : _time(time), _price(price), _size(size) {}

    /**
     *  Whether or not it is valid
     *  @return bool
     */
    bool valid() const { return _size != 0; }

    /**
     *  Get the time 
     *  @return size_t
     */
    size_t time() const { return _time; }

    /**
     *  Get the price
     *  @return float
     */
    float price() const { return _price; }

    /**
     *  Get the size
     *  @return size_t
     */
    size_t size() const { return _size; }
};
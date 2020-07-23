#pragma once

#include <iostream>
#include "barmaker.h"
#include <fstream>
#include <memory>
#include <vector>
#include <limits.h>
#include <cstring>
#include <string>
#include "eventprocessor.h"

class Util
{
public:
    /**
    *  nth occurence of a character in a string
    *  @param  str
    *  @param  character
    *  @param  n
    */
    static const char *strchrn(const char *str, int character, int n)
    {
        // the result
        const char *res = str;

        // do the strchr step n times
        for (int i = 0; i < n; i++)
        {
            // find the string
            res = strchr(res, character);
            
            // if there was no result, we can't find the nth
            if (!res) return nullptr;

            // exclude the last
            else if (i != n) ++res;

            // this might have been the last character
            if (!*res) return nullptr;
        }
        
        // return the result
        return res;
    }

    /**
    *  Convert a timestamp to a relative offset in the day, it is the format
    *  09:37:33.713000 for example.
    *  @param  str
    */
    static size_t offset(const char *str)
    {
        // get the hours/minutes/seconds and microseconds
        size_t hours = atoi(str);
        size_t minutes = atoi(str + 3);
        size_t seconds = atoi(str + 6);
        size_t micros = atoi(str + 9);

        // convert to offset
        return (hours * 3600 + minutes * 60 + seconds) * 1000 + micros / 1000;
    }

    //static std::ostream &operator<<(std::ostream &ostream, const Quote &quote)
    // {
    //     // simply make of format '440 @ 1.20'
    //     return ostream << quote.size() << "@" << quote.price(); 
    // }

    /**
    *  Process function, also allows std::cin
    */
    static int process(EventProcessor &maker, std::istream &stream)
    {
        // the line we're currently reading
        std::string line;

        // skip the first line
        std::getline(stream, line);

        // amount skipped
        size_t skipped = 0;
        size_t rows = 0;

        // get a new line from the file
        while (std::getline(stream, line))
        {
            // safety for empty lines
            if (line.size() == 0) continue;
            
            // extra row!
            rows++;

            // find the record type
            uint8_t type = line[0] - '0';

            // find the exchange
            const char *exchange = strchr(line.c_str() + 2, ',');

            // find 5th comma
            const char *time = strchrn(line.c_str(), ',', 5);
            const char *price = strchr(time + 1, ',');
            const char *size = strchr(price + 1, ',');
            
            // only certain trade conditions
            const char *cond = strchr(size + 1, ',');

            // parse the condition number
            int numcond = atoi(cond + 1);

            // number of the exchange
            int numexc = atoi(exchange + 1);

            // parse the int, skip anything that is not 0, 95, or 115
            if (numcond != 0 && numcond != 95 && numcond != 115) { skipped++; continue; }

            // drop the dark pools
            if (numexc != 0 && (numexc == 57 || numexc == 58 || numexc == 59)) { skipped++; continue; }

            // construct the quote
            Quote quote(offset(time), static_cast<float>(atof(price + 1)), atoi(size + 1));

            // switch over the type
            switch (type) {
            case 1:     maker.onTrade(quote); break;
            case 2:     maker.onBid(quote); break;
            case 3:     maker.onAsk(quote); break;
            default: 
                // ignore, wrong type
                std::cerr << "error while processing line: unknown recordtype: \n -> " << line << std::endl;
            }
        }

        if (skipped > 0) std::cout << "skipped " << skipped << " out of " << rows << " events while processing." << std::endl; 

        // always 0 for now
        return 0;
    }

    /**
     *  Process function, also allows std::cin
     */
    static int processTape(EventProcessor &maker, std::istream &stream)
    {
        // the line we're currently reading
        std::string line;

        // skip the first line
        std::getline(stream, line);

        // get a new line from the file
        while (std::getline(stream, line))
        {
            // safety for empty lines
            if (line.size() == 0) continue;
            
            // find the record type
            uint8_t type = line[0] - '0';

            // time is first element
            size_t time = atoll(line.c_str() + 2);

            // element is the price, second comma, size is right after
            const char *price = strchr(line.c_str() + 2, ',');
            const char *size = strchr(price + 1, ',');

            // construct the quote
            Quote quote(time, static_cast<float>(atof(price + 1)), atof(size + 1));

            // switch over the type
            switch (type) {
            case 1:     maker.onTrade(quote); break;
            case 2:     maker.onBid(quote); break;
            case 3:     maker.onAsk(quote); break;
            default: 
                // ignore, wrong type
                std::cerr << "error while processing line: unknown recordtype: \n -> " << line << std::endl;
            }
        }

        // always 0 for now
        return 0;
    }
};
//
//  Stock.h
//  FRE6883Spring2018TeamProject
//
//  Created by Ji Yang on 4/25/18.
//  Copyright © 2018 Ji Yang. All rights reserved.
//

#ifndef Stock_h
#define Stock_h

#include <vector>
using namespace std;

class Stock
{
public:
    Stock() {}
    /*
     @param name_     The stock's name(ticker);
     @param startDate_ Every stock's start date;
     @param endDate_   Every stock's end date
     */
    Stock(string name_, string startDate_, string endDate_);
    virtual ~Stock() {}
    void setName(string name);
    void setStartDate(string startDate_);
    void setEndDate(string endDate_);
    string getName() const;
    string getStartDate() const;
    string getEndDate() const;
    virtual void setDates(vector<string> &d) {}
    virtual void setPrices(vector<double> &p) {}
protected:
    string name;
    string startDate;
    string endDate;
};

class SPYStock: public Stock
{
public:
    SPYStock() {}
    virtual ~SPYStock() {}
    virtual void setDates(vector<string> &d);
    virtual void setPrices(vector<double> &p);
    vector<string>* getDates();
    vector<double>* getPrices();
protected:
    vector<string> dates;
    vector<double> prices;
};

#endif /* Stock_h */

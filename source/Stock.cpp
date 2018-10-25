//
//  Stock.cpp
//  FRE6883Spring2018TeamProject
//
//  Created by Ji Yang on 4/25/18.
//  Copyright © 2018 Ji Yang. All rights reserved.
//

#include <string>
#include "Stock.h"

using namespace std;

Stock::Stock(string name_, string startDate_, string endDate_)
{
    name = name_;
    startDate = startDate_;
    endDate = endDate_;
}

void Stock::setName(string name_)
{
    name = name_;
}

void Stock::setStartDate(string startDate_)
{
    startDate = startDate_;
}

void Stock::setEndDate(string endDate_)
{
    endDate = endDate_;
}

string Stock::getName() const
{
    return name;
}

string Stock::getStartDate() const
{
    return startDate;
}

string Stock::getEndDate() const
{
    return endDate;
}

void SPYStock::setDates(vector<string> &d)
{
    dates = d;
}

void SPYStock::setPrices(vector<double> &p)
{
    prices = p;
}

vector<string>* SPYStock::getDates()
{
    vector<string> *p = &dates;
    return p;
}

vector<double>* SPYStock::getPrices()
{
    vector<double> *p = &prices;
    return p;
}

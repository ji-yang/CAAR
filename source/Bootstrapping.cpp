//
//  Bootstrapping.cpp
//  FRE6883Spring2018TeamProject
//
//  Created by Ji Yang on 4/27/18.
//  Copyright © 2018 Ji Yang. All rights reserved.
//


#include <algorithm>
#include <iterator>
#include <random>
#include <vector>
#include "Bootstrapping.h"

using namespace std;

vector<int> randIndex(const int range, const int N)
{
    vector<int> v;
    for (int i = 0; i <= range; i++)
        v.push_back(i);
    random_shuffle(v.begin(), v.end());
    vector<int> result(v.begin(), v.begin()+N);
    return result;
}

int myrandom (int i)
{
    return rand() % i;
}

StockVector::iterator findStock(StockVector &stocks, string stockname)
{
    StockVector::iterator itr = stocks.begin();
    for ( ; itr != stocks.end(); itr++)
        if ((*itr)->getName() == stockname)
            return itr;
    return stocks.end();
}

Vector AAR(StockVector &stocks, Map &prices, SPYStock &spy, vector<int> &rand_indices, const int num_of_sample)
{
    StringVector *spyDate = spy.getDates();
    Vector *spyPrice = spy.getPrices();
    Vector aar(90, 0.0);
    
    for (int i = 0; i < num_of_sample; i++)
    {
        Map::iterator mitr = prices.begin();
        advance(mitr, rand_indices[i]);
        string stockname = mitr->first;
        Vector *price = mitr->second;
        //calculate ARR_t
        StockVector::iterator sitr = findStock(stocks, stockname);
        const string startdate = (*sitr)->getStartDate();
        StringVector::iterator spyitr = find(spyDate->begin(), spyDate->end(), startdate);
        int offset =  int (spyitr - spyDate->begin());
        Vector price_m(spyPrice->begin()+offset, spyPrice->begin()+offset+91);
        for(int j = 0; j < aar.size(); j++)
            aar[j] = aar[j] + ((*price)[j+1] / (*price)[j] - price_m[j+1] / price_m[j]) / num_of_sample;
    }
    return aar;
}
Vector CAAR(const Vector &aar)
{
    Vector caar;
    caar.push_back(aar[0]);
    for (int i = 1; i < aar.size(); i++)
        caar.push_back(caar[i-1]+aar[i]);
    return caar;
}



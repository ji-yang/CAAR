//
//  Bootstrapping.h
//  FRE6883Spring2018TeamProject
//
//  Created by Ji Yang on 4/27/18.
//  Copyright © 2018 Ji Yang. All rights reserved.
//

#ifndef Bootstrapping_h
#define Bootstrapping_h

#include "Vector.h"
#include "Stock.h"

/* Return a rearranged vector in the range. */
vector<int> randIndex(const int range, const int N);
/* Return a random integer from 0 to i-1 (random generator function) */
int myrandom (int i);
/*
 Return a vector ARR_t (average daily abnormalreturns) for each group for all 90 reference days
 @param stocks        StockVector of one group
 @param prices        Prices of one group
 @param spy           SPY price
 @param rand_indices  The index number for randomly selecting stocks to consist one group
 @param N             The number of selected stocks to calculate AAR from each group
 */
Vector AAR(StockVector &stocks, Map &prices, SPYStock &spy, vector<int> &rand_indices, const int N);
/*
 Return a vector that sums ARR_t
 @param aar           The vector ARR_t
 */
Vector CAAR(const Vector &aar);
/*
 Return the address of a certain stock from the stock vector
 @param stocks         StockVector 
 @param stockname      Stock's name
 */
StockVector::iterator findStock(StockVector &stocks, string stockname);

#endif /* Bootstrapping_h */

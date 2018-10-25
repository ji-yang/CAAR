//
//  DataProcessing.h
//  FRE6883Spring2018TeamProject
//
//  Created by Ji Yang on 4/18/18.
//  Copyright © 2018 Ji Yang. All rights reserved.
//

#ifndef DataProcessing_h
#define DataProcessing_h

#include <vector>
#include <map>
#include "curl/curl.h"
#include "Stock.h"
#include "Vector.h"

using namespace std;

const int NUM_OF_BATCH = 9;
const int NUM_OF_STOCK = 498;
const string STOCK_FILENAME = "stocks.csv";
const string URL_A = "https://query1.finance.yahoo.com/v7/finance/download/";
const string URL_B = "?period1=";
const string URL_C = "&period2=";
const string URL_D = "&interval=1d&events=history&crumb=";

struct MemoryStruct {
    char *memory;
    size_t size;
};

/* Returns present system time */
string now(const char* format = "%c");
/* Memory allocation */
void *myrealloc(void *ptr, size_t size);
/* Write the downliad data into memory */
size_t write_data(void *ptr, size_t size, size_t nmemb, void *data);
/* Get time in seconds */
string getTimeinSeconds(string Time);
/*
Download the adjust close prices that download from Yahoo Finance. Eliminate the stocks that don't have 91-day price data
 @param stockList   One group of stocks
 @param stockMap    Price for all stocks
 @param spy         Stock pointer (actual reference to SPYStock)
 @param cookie      Cookie filename
 */
int curlDownload(StockVector &stockList, Map &stockMap, Stock *spy, string &cookie);
/*
 Read SPY and all stocks' name, start date, and end date, then store into group.
 @param group       An vector of StockVector
 @param spy         Stock pointer (actual reference to SPYStock)
 @param size        The number of batch for multithread
 @param filename    Stock filename (csv file that contains all stocks' names, start dates, end dates)
 */
void readInStock(vector<StockVector> &group, Stock *spy, int size, const string filename);
/*
 Download SPY and all stocks' prices. Call readInStock() to read SPY and all stocks' name, start date, and end date. Then use multithread to split all stocks into groups and download price data by calling curlDownload()
 @param stockList    StockVector pointer containing all stock groups
 @param stockMap     Map pointer containing all stock groups
 @param spy          Stock pointer (actual reference to SPYStock)
 @param numofgroup   The number of groups, (3 groups: beat, meet, miss)
 */
void downloadPrice(StockVector *stockList, Map *stockMap, Stock *spy, const int numofgroup);

#endif /* DataProcessing_h */

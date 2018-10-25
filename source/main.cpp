//
//  main.cpp
//  FRE6883Spring2018TeamProject
//
//  Created by Ji Yang on 4/18/18.
//  Copyright © 2018 Ji Yang. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include "DataProcessing.h"
#include "Stock.h"
#include "Vector.h"
#include "Bootstrapping.h"
#include "plot.h"

using namespace std;

int main(int argc, const char * argv[]) {
    
    const int NUM_OF_GROUP = 3;
    const int NUM_OF_RECORD = 90;
    const int NUM_OF_ITER = 30;
    
    /*
     Threshold
     EPS > 6.5: Beat
     1.4 < EPS < 6.5 Meet
     EPS < 1.4 Miss
     */
    StockVector stocks[NUM_OF_GROUP];
    Map prices[NUM_OF_GROUP];
    Vector ave_aar[NUM_OF_GROUP], ave_caar[NUM_OF_GROUP];
    SPYStock spy;
    Stock *spyptr = &spy;
    bool downloaded = false;
    
    for (int i = 0; i < NUM_OF_GROUP; i++)
        stocks[i].reserve(NUM_OF_STOCK / NUM_OF_GROUP);
    
    while(true)
    {
        cout << "\n------------------------------------------------\n"
        << "0: Exit.\n"
        << "1: Download price.\n"
        << "2: Plot AAR.\n"
        << "3: Plot CAAR.\n"
        << "4: Plot stock.\n"
        << "------------------------------------------------\n"
        << "Please enter your choice: ";
        int choice;
        cin >> choice;
        
        //exit
        if (choice == 0)
        {
            cout << "Program exit." << endl;
            break;
        }
        //download data
        if (choice == 1)
        {
            if (downloaded)
            {
                cout << "Data already downloaded." << endl;
                continue;
            }
            //Download stock prices
            downloadPrice(stocks, prices, spyptr, NUM_OF_GROUP);
            downloaded = true;
            cout << "Beat stocks: " << prices[0].size() << endl
            << "Meet stocks: " << prices[1].size() << endl
            << "Miss stocks: " << prices[2].size() << endl;
        }
        //plot AAR or CAAR
        else if (choice == 2 || choice == 3)
        {
            if (downloaded)
            {
                //reset AAR and CAAR vectors
                for (int i = 0; i < NUM_OF_GROUP; i++)
                {
                    ave_aar[i].clear();
                    ave_caar[i].clear();
                    ave_aar[i].resize(NUM_OF_RECORD, 0.0);
                    ave_caar[i].resize(NUM_OF_RECORD, 0.0);
                }
                //average on random samples
                for (int i = 0; i < NUM_OF_GROUP; i++)
                {
                    vector<int> indices;
                    for (int j = 0; j < prices[i].size(); j++)
                        indices.push_back(j);
                    for (int j = 0; j < NUM_OF_ITER; j++)
                    {
                        //random selection of indices
                        random_shuffle(indices.begin(), indices.end(), myrandom);
                        Vector aar = AAR(stocks[i], prices[i], spy, indices, 60);
                        Vector caar = CAAR(aar);
                        ave_aar[i] = ave_aar[i] + aar / NUM_OF_ITER;
                        ave_caar[i] = ave_caar[i] + caar / NUM_OF_ITER;
                    }
                }
                if (choice == 2)
                    plotTrend(ave_aar, NUM_OF_RECORD, "AAR");
                else
                    plotTrend(ave_caar, NUM_OF_RECORD, "CAAR");
            }
            
            else
                cout << "Please download data first." << endl;
        }
        //plot stock info
        else if (choice == 4)
        {
            if (downloaded)
            {
                string name;
                cout << "Please enter the stock tick: ";
                cin >> name;
                bool found = false;
                int i = 0;
                Map::iterator itr;
                for (; i < NUM_OF_GROUP; i++)
                {
                    itr = prices[i].find(name);
                    if (itr != prices[i].end())
                    {
                        found = true;
                        break;
                    }
                }
                //if ticker is valid, show stock group and plot its prices
                if (found)
                {
                    string group = i == 0? "Beat" : i == 1? "Meet" : "Miss";
                    cout << name + " is in Group " + group << endl;
                    StockVector::iterator sitr = findStock(stocks[i], name);
                    const string title = name + " prices: " + (*sitr)->getStartDate()
                    + " to " + (*sitr)->getEndDate();
                    plotStock(*itr->second, itr->second->size(), name, title);
                }
                else
                    cout << "Stock not found. Please try again." << endl;
            }
            else
                cout << "Please download data first." << endl;
        }
        else
            cout << "Input not recognized. Please try again." << endl;
    }
    
    //release dynamic allocated memory
    if(downloaded)
    {
        for (int i = 0; i < NUM_OF_GROUP; i++)
        {
            for (Map::iterator mitr = prices[i].begin(); mitr != prices[i].end(); mitr++)
                delete mitr->second;
            for (StockVector::iterator sitr = stocks[i].begin(); sitr != stocks[i].end(); sitr++)
                delete *sitr;
        }
    }
    
    return 0;
}

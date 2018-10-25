//
//  DataProcessing.cpp
//  FRE6883Spring2018TeamProject
//
//  Created by Ji Yang on 4/18/18.
//  Copyright © 2018 Ji Yang. All rights reserved.
//

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <locale>
#include <iomanip>
#include <thread>
#include "DataProcessing.h"
#include "curl/curl.h"

using namespace std;

string now(const char* format)
{
    time_t t = time(0);
    char cstr[128];
    strftime(cstr, sizeof(cstr), format, localtime(&t));
    return cstr;
}

void *myrealloc(void *ptr, size_t size)
{
    if (ptr)
        return realloc(ptr, size);
    else
        return malloc(size);
}
size_t write_data(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)data;
    
    mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory) {
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }
    return realsize;
}
string getTimeinSeconds(string Time)
{
    std::tm t = {0};
    std::istringstream ssTime(Time);
    char time[100];
    memset(time, 0, 100);
    if (ssTime >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
    {
        std::put_time(&t, "%c %Z");
        std::mktime(&t);
        sprintf (time, "%ld", mktime(&t));
        return string(time);
    }
    else
    {
        cout << "Parse failed\n";
        return "";
    }
}

int curlDownload(StockVector &stockList, Map &stockMap, Stock *spy, string &cookieFile)
{
    struct MemoryStruct data;
    data.memory = NULL;
    data.size = 0;
    
    StockVector::iterator itr = stockList.begin();
    string timeSuffix = "T16:00:00";
    string sCookies, sCrumb;
    
    //file pointer to create file that store the data
    FILE *fp;
    
    /* declaration of an object CURL */
    CURL *handle;
    
    /* result of the whole process */
    CURLcode result;
    
    /* set up the program environment that libcurl needs */
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* curl_easy_init() returns a CURL easy handle that you're gonna reuse in other easy functions*/
    handle = curl_easy_init();
    
    /* if everything's all right with the easy handle... */
    if (handle)
    {
        try {
            curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
            curl_easy_setopt(handle, CURLOPT_COOKIEJAR, cookieFile.c_str());
            curl_easy_setopt(handle, CURLOPT_COOKIEFILE, cookieFile.c_str());
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
            
            /* perform, then store the expected code in 'result'*/
            result = curl_easy_perform(handle);
            
            /* Check for errors */
            if (result != CURLE_OK)
            {
                /* if errors have occured, tell us what is wrong with 'result'*/
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }
            
            char cKey[] = "CrumbStore\":{\"crumb\":\"";
            char *ptr1 = strstr(data.memory, cKey);
            char *ptr2 = ptr1 + strlen(cKey);
            char *ptr3 = strstr(ptr2, "\"}");
            if ( ptr3 != NULL )
                *ptr3 = NULL;
            
            sCrumb = ptr2;
            fp = fopen(cookieFile.c_str(), "r");
            char cCookies[100];
            if (fp) {
                while (fscanf(fp, "%s", cCookies) != EOF);
                fclose(fp);
            }
            else
                cerr << "cookies file does not exists" << endl;
            
            sCookies = cCookies;
            free(data.memory);
            data.memory = NULL;
            data.size= 0;
        } catch (...) {
            cerr << "Download cookie error." << endl;
            return 1;
        }
        
        while (true)
        {
            if (itr == stockList.end())
                break;
            
            //download stock price
            string name = (*itr)->getName();
            string startTime = getTimeinSeconds((*itr)->getStartDate() + timeSuffix);
            string endTime = getTimeinSeconds((*itr)->getEndDate() + timeSuffix);
            string url = URL_A + name + URL_B + startTime + URL_C + endTime + URL_D + sCrumb;
            const char *cURL = url.c_str();
            const char *cookies = sCookies.c_str();
            curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);
            curl_easy_setopt(handle, CURLOPT_URL, cURL);
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
            result = curl_easy_perform(handle);
            
            /* Check for errors */
            if (result != CURLE_OK)
            {
                /* if errors have occurred, tell us what is wrong with 'result'*/
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }
            stringstream sData;
            sData.str(data.memory);
            string line;
            //cout << "Downloading Stock: " << name << ", ";
            getline(sData, line);
            
            double price = 100.0;
            //get the adjust close price of the period
            int cnt = 0;
            if (name == "SPY")
            {
                StringVector dates;
                Vector adjclose;
                while(getline(sData, line))
                {
                    istringstream ss(line);
                    string date;
                    string token;
                    getline(ss, date, ',');
                    for(int i = 0; i < 5; i++)
                        getline(ss, token, ',');
                    try {
                        price = stod(token);
                    } catch (...) {
                        cerr << "SPY: stod conversion exception." << endl;
                    }
                    dates.push_back(date);
                    adjclose.push_back(price);
                    cnt++;
                }
                spy->setPrices(adjclose);
                spy->setDates(dates);
            }
            else
            {
                Vector *adjclose = new Vector;
                while(getline(sData, line))
                {
                    istringstream ss(line);
                    string token;
                    for(int i = 0; i < 6; i++)
                        getline(ss, token, ',');
                    try {
                        price = stod(token);
                    } catch (...) {
                        cerr << name << ": stod conversion exception." << endl;
                    }
                    adjclose->push_back(price);
                    cnt++;
                }
                if (cnt >= 91)
                    stockMap[name] = adjclose;
            }
            //cout << "count = " << cnt << endl;
            free(data.memory);
            data.memory = NULL;
            data.size= 0;
            itr++;
        }
    }
    else
    {
        fprintf(stderr, "Curl init failed!\n");
        return 1;
    }
    
    /* cleanup since you've used curl_easy_init */
    curl_easy_cleanup(handle);
 
    /* this function releases resources acquired by curl_global_init() */
    curl_global_cleanup();
    return 0;
}

void readInStock(vector<StockVector> &group, Stock *spy, int size, const string filename)
{
    cout << "Collecting stocks info..." << endl;
    ifstream file(filename);
    string line, name, startDate, endDate;
    
    //title line
    getline(file, line);
    
    //SPY line
    getline(file, line);
    istringstream ss(line);
    getline(ss, name, ',');
    getline(ss, startDate, ',');
    getline(ss, endDate, ',');
    spy->setName(name);
    spy->setStartDate(startDate);
    spy->setEndDate(endDate);
    group[0].push_back(spy);
    
    //read stock params
    int batch_size = NUM_OF_STOCK / size;
    int loop_count = 0;
    while(getline(file, line))
    {
        istringstream ss(line);
        getline(ss, name, ',');
        getline(ss, startDate, ',');
        getline(ss, endDate, ',');
        Stock *stock_ptr = new Stock(name, startDate, endDate);
        group[loop_count/batch_size+1].push_back(stock_ptr);
        loop_count++;
    }
    file.close();
}

void downloadPrice(StockVector *stockList, Map *stockMap, Stock *spy, const int num_of_group)
{
    vector<StockVector> group;
    vector<Map> stockPrice;
    StringVector cookieFile;
    //split whole data into batches for multithreading
    for (int i = 0; i <= NUM_OF_BATCH; i++)
    {
        StockVector sv;
        sv.reserve(NUM_OF_STOCK / NUM_OF_BATCH);
        group.push_back(sv);
        Map m;
        stockPrice.push_back(m);
        string s = "cookies" + to_string(i) + ".txt";
        cookieFile.push_back(s);
    }
    
    readInStock(group, spy, NUM_OF_BATCH, STOCK_FILENAME);
    
    //download from Yahoo Finance
    cout << "Start Download: " << now() << endl;
    
    thread t[NUM_OF_BATCH + 1];
    for (int i = 0; i <= NUM_OF_BATCH; i++)
        t[i] = thread(curlDownload, ref(group[i]), ref(stockPrice[i]), ref(spy), ref(cookieFile[i]));
    for (int i = 0; i <= NUM_OF_BATCH; i++)
        t[i].join();
    
    cout << "Finish Download: " << now() << endl;
    
    //combine batches into 3 groups
    int denominator = NUM_OF_BATCH / num_of_group;
    for(int i = 1; i <= NUM_OF_BATCH; i++)
    {
        stockList[(i-1)/denominator].insert(stockList[(i-1)/denominator].end(), group[i].begin(), group[i].end());
        stockMap[(i-1)/denominator].insert(stockPrice[i].begin(), stockPrice[i].end());
    }
}


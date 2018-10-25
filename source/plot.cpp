//
//  plot.cpp
//  FRE6883Spring2018TeamProject
//
//  Created by Ji Yang on 4/28/18.
//  Copyright © 2018 Ji Yang. All rights reserved.
//

#include <vector>
#include <stdlib.h>
#include <math.h>
#include <chrono>
#include <thread>
#include "Vector.h"

using namespace std;

void plotTrend(const Vector* groupData, const int num_of_record, const string title)
{
    vector<int> TimeLine(num_of_record,0);
    for (int i = 0; i < num_of_record; i++)
        TimeLine[i] = i - 59;
    
    FILE *gnuplotPipe,*tempDataFile;
    const char *tempDataFileName;
    
    tempDataFileName = title.c_str();
    gnuplotPipe=popen("/opt/local/bin/gnuplot","w");    //Opens the file whose name is specified in the parameter filename and associates it with a stream that can be identified in future operations by the FILE pointer returned. "w"-write
    
    if(gnuplotPipe){
        const char* setTitle = ("set title \"" + title + "\"\n").c_str();
        fprintf(gnuplotPipe, "%s", setTitle);
        fprintf(gnuplotPipe,"set xlabel\"Time (Day)\"\n");
        fprintf(gnuplotPipe,"set ylabel\"Value\"\n");
        fprintf(gnuplotPipe,"set yrange[-0.05:0.05]\n");
        fprintf(gnuplotPipe,"plot \"%s\" using 1:2 title 'Beat' lw 2 with linespoints,\"%s\" using 1:3 title 'Meet' lw 2 with linespoints,\"%s\" using 1:4 title 'Miss' lw 2 with linespoints \n",tempDataFileName,tempDataFileName,tempDataFileName);
        fflush(gnuplotPipe); //If the given stream was open for writing, any unwritten data in its output buffer is written to the file.
        tempDataFile = fopen(tempDataFileName,"w");
        
        for (int i = 0; i < num_of_record; i++)
            fprintf(tempDataFile,"%int %f %f %f\n", TimeLine[i], groupData[0][i], groupData[1][i], groupData[2][i]);
        
        fclose(tempDataFile);
        //wait to complete plot
        this_thread::sleep_for(500ms);
        remove(tempDataFileName);
        fprintf(gnuplotPipe,"exit \n");
        
    }else{
        printf("gnuplot not found...");
    }
}

void plotStock(const Vector &price, const unsigned long num_of_record, string name, string title)
{
    vector<int> TimeLine(num_of_record,0);
    for (int i = 0; i < num_of_record; i++)
        TimeLine[i] = i;
    
    FILE *gnuplotPipe,*tempDataFile;
    const char *tempDataFileName;
    
    tempDataFileName = name.c_str();
    gnuplotPipe = popen("/opt/local/bin/gnuplot","w");    //Opens the file whose name is specified in the parameter filename and associates it with a stream that can be identified in future operations by the FILE pointer returned. "w"-write
    
    if(gnuplotPipe){
        const char* setTitle = ("set title \"" + title + "\"\n").c_str();
        fprintf(gnuplotPipe, "%s", setTitle);
        fprintf(gnuplotPipe,"set xlabel\"Time (Day)\"\n");
        fprintf(gnuplotPipe,"set ylabel\"Price\"\n");
        fprintf(gnuplotPipe,"plot \"%s\" lw 2 with linespoints \n", tempDataFileName);
        fflush(gnuplotPipe); //If the given stream was open for writing, any unwritten data in its output buffer is written to the file.
        
        tempDataFile = fopen(tempDataFileName,"w");
        
        for (int i = 0; i < num_of_record; i++)
            fprintf(tempDataFile,"%int %f\n", TimeLine[i], price[i]);
        
        fclose(tempDataFile);
        //wait to complete plot
        this_thread::sleep_for(500ms);
        remove(tempDataFileName);
        fprintf(gnuplotPipe,"exit \n");
        
    }else{
        printf("gnuplot not found...");
    }
}

//
//  plot.h
//  FRE6883Spring2018TeamProject
//
//  Created by Ji Yang on 4/28/18.
//  Copyright © 2018 Ji Yang. All rights reserved.
//

#ifndef plot_h
#define plot_h

/*
 Plot AAR or CAAR graph (time vs value)
 @param groupData        Vector pointer for 3 groups that we want to plot
 @param num_of_record    90 reference days
 @param title            Title name for the graph
 */
void plotTrend(const Vector* groupData, const int num_of_record, const string title);
/*
 Plot a selected stock from all stocks
 @param price           The vector of price for the selected stock
 @param num_of_record   90 reference days
 @param name            The name of the stock we want to plot
 @param title           Title name for the graph
 */
void plotStock(const Vector &price, const unsigned long num_of_record, string name, string title);
#endif /* plot_h */

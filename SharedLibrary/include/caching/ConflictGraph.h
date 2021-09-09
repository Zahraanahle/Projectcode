#ifndef CONFLICTGRAPH_H
#define CONFLICTGRAPH_H
#include <chrono>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <algorithm> 
#include <vector>
#include "randomHandler.h"
#include "CheckFunction.h"
#include "DataDefinition.h"

/* Include GSL header files. */
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>

namespace caching{

void _dealloc();

void computeNumberOfNodes();

void makeNodes();
void makeEdges();

cf_data conflictGraphGenerator(data_matrix data, int, int);
cf_data merge_cf(cf_data , cf_data);
void ColoringProcess(cf_data , int , int ,int *d_coloring,int d_n_col);
}//end namespace caching

#endif // CONFLICTGRAPH_H

/* -*- c++ -*- */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include "CheckFunction.h"
#include "EnvironmentSetup.h"
#include "DecodingInput.h"
#include "ConflictGraph.h"
#include "randomHandler.h"
#include "DataDefinition.h"
#include "grasp.h"
#include "FuncsFromMain.h"
//#include "TestTxRx.h"

using namespace caching;
using namespace std;


int main()
{
    int d_m_files = 20;
    int d_b_chunks = 100;
    int n_user1 = 3;
    int n_user2 = 2;
    time_t tstart1, tend1, tstart2, tend2,tstart3, tend3;
    data_matrix d_data1,d_data2;
    cf_data d_outForColoring1,d_outForColoring2,d_outForColoring3;
    bool d_GRASP=true;
    int d_n_col=0;
    int *d_coloring;
    bool DEBUG = true;


    cout << endl << "Data generation process" << endl << "-------------" << endl << endl;

    d_data1 = generateData(d_m_files, d_b_chunks);
    int nb_users = d_data1.n_utenti;

    cout << endl << "Conflict-Graph generator process" << endl << "-------------" << endl << endl;
    d_outForColoring1 = conflictGraphGenerator(d_data1, 0, n_user1);
    cout << endl << "Number of nodes of the graph : " << d_outForColoring1.n_nodi << endl << endl;
    d_outForColoring2 = conflictGraphGenerator(d_data1, n_user1, n_user1+n_user2);
    cout << endl << "Number of nodes of the graph: " << d_outForColoring2.n_nodi << endl << endl;
    d_outForColoring3 = merge_cf(d_outForColoring1,d_outForColoring2);

     
     if (d_outForColoring1.n_nodi > 0)
    {
        cout << endl << "Graph Coloring process of graph 1" << endl << "-------------" << endl << endl;

        if (d_GRASP)
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            int d_maxIter = 20;
            d_coloring = graspGraphColoring(d_maxIter, d_outForColoring1, &d_n_col);
            colorRienumeration(d_n_col, &d_coloring, d_outForColoring1.n_nodi);
            auto end_time = std::chrono::high_resolution_clock::now();
  
        auto time = end_time - start_time;
        std::cout << "took " << time/std::chrono::milliseconds(1) << "ms to run ";

        }
        cout << endl << "Coloring Succeeded !" << endl;
        cout << endl << "Number of used colors: " << d_n_col << endl;
        
    }/*end if (d_outputForColoring.n_nodi > 0)*/

   cout << "Coloring Process " << endl;
  
       auto start_time = std::chrono::high_resolution_clock::now();
       ColoringProcess(d_outForColoring3 ,d_outForColoring1.n_nodi,d_outForColoring2.n_nodi,d_coloring,d_n_col);
       auto end_time = std::chrono::high_resolution_clock::now();

    auto time = end_time - start_time;
    std::cout << "took " << time/std::chrono::milliseconds(1) << "ms to run ";

    } 












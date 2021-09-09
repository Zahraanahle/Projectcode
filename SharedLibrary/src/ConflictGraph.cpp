#include "ConflictGraph.h"

namespace caching{

/* Variables declaration */
    int n_utenti;
    int user1, user2;
    int m_files;
    int b_chuncks;
    int n_nodi;
    int m_archi;

    int ***Ind = NULL;
    int *Q = NULL;
    int **Q_chuncks = NULL;

    int **Matrix_Adj = NULL;
    nodo *nodes = NULL;

/******************************************************************************************************************************************/
    void computeNumberOfNodes(){
        int i, k, id_file;
        n_nodi = 0;

    /*For each user ...*/
        for (i=user1; i<user2; i++){
        /*For each chunk ...*/
            for (k=0; k<b_chuncks; k++){
            /*The file that the user 'i' request*/
                id_file = Q[i];
            /*Checvk if user 'i' have in cache the chunk 'k' related to files 'id_file'*/
                if (Ind[i][id_file][k] == 1){
                    Q_chuncks[i][k] = 0;
                }else{
                    Q_chuncks[i][k] = 1;
                    n_nodi++;
                }
            }
        }
    }

/*This is a function that provide to make nodes of the conflict information graph*/
    void makeNodes(){
        int i, j, k, id;

        nodes = (nodo *) malloc (n_nodi * sizeof(nodo));
        if (!nodes){
            printf("\nError: Allocation Nodes.\n");
            exit(0);
        }

        Matrix_Adj = (int **) malloc (n_nodi * sizeof(int *));
        check_memory_allocation_2D(Matrix_Adj, "Allocation 2D Adj Matrix.");

        for (i=0; i<n_nodi; i++){
            Matrix_Adj[i] = (int *) malloc (n_nodi * sizeof(int));
            check_memory_allocation_1D(Matrix_Adj[i], "Allocation 1D Adj Matrix.");
        }

        for (i=0; i<n_nodi; i++){
            Matrix_Adj[i][i] = 0;
        }

        id =0;
    /*For each user and for each chunk related to requested file that the user not have in cache will be created a node*/
        for (i=user1; i<user2; i++){
            j = Q[i];
            for (k=0; k<b_chuncks; k++){
                if (Q_chuncks[i][k] == 1){
                    nodes[id].id = id;
                    nodes[id].degree = 0;
                    nodes[id].id_utente = i;
                    nodes[id].id_chunck = k;
                    nodes[id].id_file = j;
                    id++;
                }
            }
        }

        if (id != n_nodi){
            printf("\nError: Create Nodes Number Dismatch With The Aspected Nodes Number.\n");
            exit(0);
        }
    }

/*This is a function that provide to make a edges of the conflict information graph*/
    void makeEdges(){
        int i_1, j_1, k_1, i_2, j_2, k_2, id1, id2;

        m_archi = 0;

        cout << "Edges process starts ----" << endl;

        cout<< n_nodi << endl;


    /*For each pair of nodes of the conflict information graph ...*/
        for (id1=0; id1<n_nodi-1; id1++){
          //  cout << id1 << ',';


            for (id2=(id1+1); id2<n_nodi; id2++){
                i_1 = nodes[id1].id_utente;
                j_1 = nodes[id1].id_file;
                k_1 = nodes[id1].id_chunck;

                i_2 = nodes[id2].id_utente;
                j_2 = nodes[id2].id_file;
                k_2 = nodes[id2].id_chunck;
            /*If the files requests by user 'i_1' and user 'i_2' are different or, the chunk requests by user 'i_1' and user 'i_2' are different and
            the user 'i_2' not have in cache the chunk requests by user 'i_1' or the user 'i_1' not have in cache the chunk requests by user 'i_2' then will be created an edges between the two nodes
            of the conflict information graph*/
                if ( ( ( k_1 != k_2 ) || ( j_1 != j_2) ) && ( ( Ind[i_2][j_1][k_1] == 0 ) || ( Ind[i_1][j_2][k_2] == 0 ) ) ){
                   Matrix_Adj[id1][id2] = 1;
                   Matrix_Adj[id2][id1] = 1;
                   nodes[id1].degree++;
                   nodes[id2].degree++;            

                   m_archi += 2;
               }else{
                Matrix_Adj[id1][id2] = 0;
                Matrix_Adj[id2][id1] = 0;
            }
        }
    }
}

/******************************************************************************************************************************************/
void _dealloc(){
    int i;

    free(Q);
    Q = NULL;

    for (i=0; i<n_utenti; i++){
        free(Q_chuncks[i]);
        Q_chuncks[i] = NULL;
    }

    free(Q_chuncks);
    Q_chuncks = NULL;
}

/******************************************************************************************************************************************/

/*This is a main function*/
cf_data conflictGraphGenerator(data_matrix data, int u1, int u2){
    m_files = data.m_files;
    b_chuncks = data.b_chunks;
    n_utenti = data.n_utenti;
    user1 = u1;
    user2 = u2;
    Ind = data.Ind;
    Q = data.Q;
    Q_chuncks = data.Q_chuncks;

    cf_data output;

    if (Ind != NULL && Q != NULL && Q_chuncks != NULL){
        /*Bulding a conflict information graph*/
        computeNumberOfNodes();

        if (n_nodi > 0){
            makeNodes();
            makeEdges();
        }else{
            m_archi = 0;
        }

        output.Matrix_Adj = Matrix_Adj;
        output.n_nodi = n_nodi;
        output.nodes = nodes;
        output.Ind = Ind;

        //_dealloc();
    }else{
        output.n_nodi = 0;
    }

    return output;
}

cf_data merge_cf(cf_data d1,cf_data d2) { 

    cf_data d3;
    int total_nb_of_nodes = d1.n_nodi + d2.n_nodi;
    nodo* merged_nodes = new nodo[total_nb_of_nodes];
    cout << "nb nodes: " << total_nb_of_nodes << endl;

    for (int i = 0; i < d1.n_nodi; i++) 
    {
        merged_nodes[i] = d1.nodes[i];
    }
    cout << "graph 1 added" << endl;

    for (int j = 0; j < d2.n_nodi; j++) {
      merged_nodes[j+d1.n_nodi] = d2.nodes[j];
  }

  cout << "graph 2 added" << endl;

  Matrix_Adj = (int **) malloc (total_nb_of_nodes * sizeof(int *));


  for (int i=0; i<total_nb_of_nodes; i++){
    Matrix_Adj[i] = (int *) malloc (total_nb_of_nodes * sizeof(int));
}
cout << "adjacent matrix created" << endl;

for (int i=0; i<total_nb_of_nodes; i++){
    Matrix_Adj[i][i] = 0;
}

    //Matrix_Adj=Matrix_Adj;
nodes=merged_nodes;
n_nodi= total_nb_of_nodes;


makeEdges();

d3.Matrix_Adj = Matrix_Adj;
d3.n_nodi = n_nodi;
d3.nodes = nodes;
d3.Ind = Ind;


return d3;
}


 //d_coloring is an array that contains the colors of all the nodes in conflict graph1

void ColoringProcess(cf_data d3, int n_nodi1, int n_nodi2,int *d_coloring,int d_n_col){

    vector <unsigned int> adjacent_color;   //the list of colors a non-colored node cannot take
    vector <unsigned int> node_color;  // the list of colors of each node
    vector <unsigned int> possible_colors;  //list of possible colors ,an uncolored node can take
    vector <unsigned int> available_colors;// list storing all the available colors from 1 to d_n_col(=746 initially)
    vector <unsigned int> assigned_color;// list storing the colors assigned to each uncolored node 
    vector <unsigned int> adjacent_color_unique; //list of unique adjacent colors 

    int nb_colored,nb_uncolored,nb_total,total_colored,nb_colors,color_possible;
     
    //Initializing 
    nb_colored=n_nodi1;  //number of colored nodes
    nb_uncolored=n_nodi2; //number of uncolored nodes
    nb_total=nb_colored+nb_uncolored; //total number of nodes

    total_colored=nb_colored;

    nb_colors=d_n_col; //total number of available colors

    for(int y = 0; y < d_n_col; y++)
        available_colors.push_back(y+1);
 
    for (int k=0;k<total_colored;k++)
        node_color.push_back(d_coloring[k]);


    for(int i=0;i<nb_uncolored;i++)
    {
      adjacent_color.clear();
      possible_colors.clear();
      adjacent_color_unique.clear();
      int number,count=0;

        for(int j=0;j<total_colored; j++) 
        {
            if (d3.Matrix_Adj[i+nb_colored][j]==1)
            {              
                count++;
                adjacent_color.push_back(node_color[j]);

            } 
        }
        number=total_colored-count;
        
      //  cout<<"The uncolored node doesn't have edges with "<<number<<" colored node" << endl;

        sort(adjacent_color.begin(),adjacent_color.end());

        unique_copy(adjacent_color.begin(),adjacent_color.end(),back_inserter(adjacent_color_unique));

        set_difference(available_colors.begin(), available_colors.end(), adjacent_color_unique.begin(), adjacent_color_unique.end(),
        back_inserter(possible_colors));

        
        if(adjacent_color_unique.size()==nb_colors){
              nb_colors++;
              cout << "new color " << nb_colors << endl;
              available_colors.push_back(nb_colors);  
              assigned_color.push_back(nb_colors);
              node_color.push_back(nb_colors);
        }else{
            assigned_color.push_back(possible_colors[0]);
            node_color.push_back(possible_colors[0]);
        }

        total_colored++;
    }

     cout << "we now have " << total_colored << "  colored node" << endl;
     cout<< "The set of assigned colors for the uncolored nodes is: "<<endl;

     for(int f=0;f<assigned_color.size();f++)
       cout<< assigned_color[f] << ",";

  }
}




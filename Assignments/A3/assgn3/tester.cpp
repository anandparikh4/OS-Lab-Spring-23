// uncomment following code for testing whether the shared memory and graph data structure implementation work correctly
// // TESTING
//     ofstream F;
//     F.open("test1.txt");        // graph from local memory
//     vector<vector<int>> a(4039 , vector<int>(0));
//     for(int i=0;i<left.size();i++){
//         a[left[i]].push_back(right[i]);
//         a[right[i]].push_back(left[i]);
//     }
//     for(int i=0;i<a.size();i++){
//         F << i << ": ";
//         for(int j=a[i].size()-1;j>=0;j--) F << a[i][j] << " " ;
//         F << endl;
//     }
//     F.close();

//     F.open("test2.txt");        // graph from shared memory
//     vector<vector<int>> b(4039 , vector<int>(0));
//     for(int i=0;i<*(G->n);i++){
//         F << i << ": ";
//         int curr = (G->heads[i]).next;
//         while(curr != -1){
//             F << G->nodes[curr].id << " ";
//             curr = G->nodes[curr].next;
//         }
//         F << endl;
//     }
//     F.close();
//     // run following command in terminal to check equality: 
//     // diff test1.txt test2.txt 
// // TESTING
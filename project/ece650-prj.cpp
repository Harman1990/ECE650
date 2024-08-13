// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <unordered_map>
#include <queue>
#include <list>
#include <memory>
#include "minisat/core/Solver.h"
#include "minisat/core/SolverTypes.h"
#include <algorithm>
#include <pthread.h>
#include <time.h>


using namespace Minisat;

struct ThreadArgs {
    int v;
    std::vector<std::pair<int, int>> pairVector;
};

void* findVectorCoverWrapper(void* arg);
void* findVectorCover_1Wrapper(void* arg);
void* findVectorCover_2Wrapper(void* arg);



std::vector<int> processBFS(const std::unordered_map<int, std::list<int>> adjacenyList, const std::set<int> setVertices, int source, int destination);
void printShortestPath( std::vector<int> answer);

void findVectorCover(int v,std::vector<std::pair<int, int>> pairVector);
void findVectorCover_1(int v,std::vector<std::pair<int, int>> pairVector);
void findVectorCover_2(int v,std::vector<std::pair<int, int>> pairVector);


std::vector<int> finalResult_Vector_1;
std::vector<int> finalResult_Vector_2;
std::vector<int> finalResult_Vector_3;

void print_Vector1_result();
void print_Vector2_result();
void print_Vector3_result();

double duration_1;
double duration_2;
double duration_3;



int main(int argc, char** argv) {

    // Declare a vector of pairs of integers to store edges
    std::vector<std::pair<int, int>> pairVector;

    // Variable to store input of Number of Vertices
    int vertices=0;

    // Unique set of vertices from entered edges.
    std::set<int> setVertices;

    // Adjaceny List of Graph
    std::unordered_map<int, std::list<int>> adjacenyList;

    bool noEdge=false;
      
    while (!std::cin.eof()) {

        std::string line;

        std::getline(std::cin, line);

        if(!(line[0]=='s')){
            //std::cout <<line<<"\n";
        }
        

        if (line.empty() || line.find_first_not_of(" ")){
            continue;
        }

        std::istringstream input(line);

        char command;
        input >> command;

        if (command == 'V'){

            bool valid_input=true;

            while(!input.eof()){

                input >> vertices;
                if (input.fail()) {
                valid_input=false;
                break;
                }

            }

            if (valid_input == false){
                std::cerr << "Error: Invalid input for vertices \n";
                continue;
            }

            if (vertices <= 1){
                vertices=0;
                //std::cerr << "Error: Invalid input for vertices \n";
                continue;
            }

            pairVector.clear();
            noEdge=false;

        }

        else if(command == 'E'){

            if(noEdge==true){
                vertices=0;
            }
            if (vertices == 0){
                std::cout << "Error: Missing vertices Input \n";
                continue;
            }

            if (pairVector.size() != 0){
                std::cerr << "Error: Missing vertices Input \n";
                vertices=0;
                continue;
            }

            
            char edge_start;
            input >> edge_start;

            bool error=false;

            bool sameEdge=false;

            //bool noEdge=false;

            while(!input.eof()){

                char pair_bracket_start;
                input >> pair_bracket_start;

                //Checking no edge condition
                if(pair_bracket_start=='}'){
                    noEdge=true;
                    //vertices=0;
                    break;
                }


            
                int vertice_1;
                input >> vertice_1;

                char comma;
                input >> comma;

                int vertice_2;
                input >> vertice_2;

                char pair_bracket_end;
                input >> pair_bracket_end;
                       
                if (!(vertice_1 >=1 && vertice_1 <=vertices)){
                    error=true;
                    break;
                }

                if (!(vertice_2 >=1 && vertice_2 <=vertices)){
                    error=true;
                    break;
                }

                if (vertice_1 == vertice_2){
                    error=true;
                    break;
                }
            
                if(pairVector.size()>0){
                    for (const auto& pair : pairVector) {
                        if(pair.first== vertice_1 && pair.second==vertice_2){
                            sameEdge=true;
                        }
                   
                    }
                
                }


                pairVector.emplace_back(vertice_1,vertice_2);
                

                char comma_or_bracket;
                input >> comma_or_bracket;

                if (comma_or_bracket == '}'){
                    break;
                }
                

            }

            if (error == true){
                pairVector.clear();
                vertices=0;
                std::cerr << "Error: Invalid Edges \n";
                continue;

            }

            
            //Testing for similar edges.
            if(pairVector.size() >1){
                for (const auto& pair : pairVector) {

                    for(const auto& pair2 : pairVector){
                        if(pair.first == pair2.second && pair.second == pair2.first){
                            sameEdge=true;
                        }

                    }
                }

            }


            if (sameEdge == true){
                pairVector.clear();
                vertices=0;
                //std::cout << "Error: Invalid Edges \n";
                std::cerr << "Error: Invalid Edges\n";
                continue;

            }

            // Create Adjaceny List
            adjacenyList.clear();

            for (const auto& pair : pairVector) {
            
                int vertex_1=pair.first;
                //std::cerr << "Edge: "<<pair.first;
                int vertex_2=pair.second;
                //std::cerr << "Edge: "<<pair.second<<"\n";

                adjacenyList[vertex_1].push_back(vertex_2);

                adjacenyList[vertex_2].push_back(vertex_1);

            }


            ThreadArgs args1 = {vertices, pairVector};
            ThreadArgs args2 = {vertices, pairVector};
            ThreadArgs args3 = {vertices, pairVector};

            pthread_t thread1, thread2, thread3;
            pthread_create(&thread1, nullptr, findVectorCoverWrapper, &args1);
            pthread_create(&thread2, nullptr, findVectorCover_1Wrapper, &args2);
            pthread_create(&thread3, nullptr, findVectorCover_2Wrapper, &args3);

            pthread_join(thread1, nullptr);
            pthread_join(thread2, nullptr);
            pthread_join(thread3, nullptr);

            print_Vector1_result();
            print_Vector2_result();
            print_Vector3_result();


        }

        else if (command == 's'){

            bool noEdgeCounter=noEdge;

            if (vertices == 0 ){
                std::cerr << "Error: Missing vertices information \n";
                noEdge=false;
                continue;
            }

            if (pairVector.size() == 0 && noEdge==false){

                vertices=0;
                std::cerr << "Error: Missing Edges information  \n";
                continue;
            }
            

            int source;
            input >> source;
            if (input.fail()) {
                std::cerr << "Error: Invalid format of command \n";
                noEdge=false;
                continue;
            }

            int destination;
            input >> destination;
            if (input.fail()) {
                std::cerr << "Error: Invalid format of command \n";
                noEdge=false;
                continue;
            }


            setVertices.clear();

            // Making unique set of vertices from entered edges.
            for (const auto& pair : pairVector) {
            
                setVertices.insert(pair.first);
                setVertices.insert(pair.second);
        
            }


            if (!(source >=1 && source <=vertices)){
                std::cerr << "Error: Vertex input invalid for command s \n";
                //noEdgeCounter=false;
                continue;
            }

            if (!(destination >=1 && destination <=vertices)){
                std::cerr << "Error: Vertex input invalid for command s \n";
                //noEdgeCounter=false;
                continue;
            }

            if (source == destination){
                std::cerr << "Error: Vertex input invalid for command s \n";
                //noEdgeCounter=false;
                continue;
            }

            if(noEdgeCounter==true){
                std::cerr << "Error: Path does not exist \n";
                //noEdgeCounter=false;
                vertices=0;
                continue;
            }


            bool is_Source_Valid=false;
            bool is_Destination_Valid=false;

            for (const auto& vertex : setVertices) {
                if (source == vertex){
                    is_Source_Valid=true;
                }

                if (destination == vertex){
                    is_Destination_Valid=true;
                }

            }

            if (is_Source_Valid == false || is_Destination_Valid == false){
                std::cerr << "Error: Path does not exist \n";
                continue;
            }

            
            std::vector<int> answer;

            answer=processBFS(adjacenyList,setVertices,source,destination);

            printShortestPath(answer);


            std::cout << "\n";

            answer.clear();
        
            setVertices.clear();

            
     
        }

        else {
            std::cerr << "Error: Invalid Command \n";

        }

    
         
    }

    return 0;
   
}


std::vector<int> processBFS( std::unordered_map<int, std::list<int>> adjacenyList,  std::set<int> setVertices, int source, int destination) {
    
            // Declare an unordered_map with int keys and string values -- to maintain Color/visited
            std::unordered_map<int, std::string> color;

            // Declare an unordered_map with int keys and string values -- to maintain distance
            std::unordered_map<int, int> distance;

            // Declare an unordered_map with int keys and string values -- to maintain parent
            std::unordered_map<int, int> parent;

            // Set Color, distance and parent of all vertices
            for (const auto& vertex : setVertices) {
                color[vertex]="WHITE";

                distance[vertex]=0;

                parent[vertex]=-1;
                
            }

            color[source]="GRAY";
            distance[source]=0;
            parent[source]=-1;

            std::queue<int> myQueue;

            myQueue.push(source);

            int current_vertex;
            while(!myQueue.empty()){

                current_vertex = myQueue.front();
                myQueue.pop();

                for (const auto& vertex : adjacenyList[current_vertex]) {

                    if (color[vertex]=="WHITE"){
                        
                        color[vertex] ="GRAY";
                        distance[vertex]=distance[current_vertex] + 1;
                        parent[vertex] = current_vertex;
                        myQueue.push(vertex);

                    }

                    color[current_vertex]="BLACK";
                }


            }

            // Shortes Path
            std::vector<int> answer;

            int current_Node=destination;


            if(parent[destination]== -1){
                return answer;
            }
            answer.push_back(current_Node);

            
            
            while(current_Node != source){
                current_Node= parent[current_Node];
                answer.push_back(current_Node);

            }


    return answer;
}

void printShortestPath( std::vector<int> answer){

    if(answer.size() == 0 ){
        std::cerr << "Error: Path does not exist";
    }
    else {
            //Print Answer
        for(int i=answer.size()-1; i >=0; --i){
            std::cout << answer[i];

            if (i > 0){
                std::cout << "-";
            }
        }


    }
}




void findVectorCover(int v,std::vector<std::pair<int, int>> pairVector){

    // Unique set of vertices from entered edges.
    std::set<int> setVertices;

    // Making unique set of vertices from entered edges.
    for (const auto& pair : pairVector) {
            
        setVertices.insert(pair.first);
        setVertices.insert(pair.second);
        
    }


    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());

    for(int k=1;k<=v;++k){

        std::vector<std::vector<Lit>> literals_2d(v + 1, std::vector<Lit>(k + 1));

        
        for(int i = 1; i <= v; ++i) {
            for(int j = 1; j <= k; ++j) {
                Lit lit = mkLit(solver->newVar()); 
                literals_2d[i][j] = lit;
            }
        }


        //First group of clauses
        for(int vertex=1;vertex<=k;++vertex){

            vec<Lit> clause;
            for( int n=1;n<=v;++n){

                clause.push(literals_2d[n][vertex]);

            }
            
            solver->addClause(clause);
            clause.clear();

        }

        //Second group of clauses
        for(int n=1;n<=v;++n){

            if(k>1){

                for(int vertex1=1;vertex1<=k;++vertex1){

                vec<Lit> clause; 
                for(int vertex2=vertex1+1;vertex2<=k;++vertex2){


                    clause.push(~literals_2d[n][vertex1]);
                    clause.push(~literals_2d[n][vertex2]);

                    solver->addClause(clause);
                    clause.clear();

                }


                }

            }   


            
        }

        //Third group of clauses
        for(int vertex=1;vertex<=k;++vertex){

            for(int n1=1;n1<=v;++n1){

                vec<Lit> clause; 

                for(int n2=n1+1;n2<=v;++n2){

                    clause.push(~literals_2d[n1][vertex]);
                    clause.push(~literals_2d[n2][vertex]);
                    solver->addClause(clause);
                    clause.clear();
                }

            }

        }


        //Fourth Group of Clauses
        for (const auto& pair : pairVector) {

            int edge1=pair.first;
            int edge2=pair.second;
            vec<Lit> clause; 

            for(int vertex=1;vertex<=k;++vertex){

                clause.push(literals_2d[edge1][vertex]);

            }

            for(int vertex=1;vertex<=k;++vertex){

                clause.push(literals_2d[edge2][vertex]);

            }


            solver->addClause(clause);
            clause.clear();



                
        }

    bool res=solver->solve();


    if(res==1){


        for(int i=1;i<=v;++i){

            for(int j=1;j<=k;++j){

                if(solver->modelValue(literals_2d[i][j]) == l_True){

                    finalResult_Vector_1.emplace_back(i);

                }

                               
            }


        }


        break;

    }

    literals_2d.clear();
    solver.reset (new Minisat::Solver());  

    }


}


void findVectorCover_1(int v,std::vector<std::pair<int, int>> pairVector){

    // Adjaceny List of Graph
    std::unordered_map<int, std::list<int>> adjacenyList;

    for (const auto& pair : pairVector) {
            
                adjacenyList[pair.first].push_back(pair.second);
                adjacenyList[pair.second].push_back(pair.first);

    }

    while(adjacenyList.size()!=0){


        int vertex_max=0;
        int count_edges=0;


        for (const auto& pair : adjacenyList) {
            int vertex = pair.first;
            const std::list<int>& neighbors = pair.second;

            if (neighbors.size() > static_cast<size_t>(count_edges)){

                vertex_max=vertex;
                count_edges=neighbors.size();
                

            }

        }

        finalResult_Vector_2.emplace_back(vertex_max);


        std::list<int>& neighbors=adjacenyList[vertex_max];

        for (int value : neighbors) {

            if(adjacenyList[value].size()==1){

                auto it=adjacenyList.find(value);

                if (it != adjacenyList.end()) {
                    adjacenyList.erase(it);
                    
                }

            }
            else{
                adjacenyList[value].remove(vertex_max);
            }
                        
        }

        
        auto it=adjacenyList.find(vertex_max);

        if (it != adjacenyList.end()) {
                adjacenyList.erase(it);
                
        }


    }


}

void findVectorCover_2(int v,std::vector<std::pair<int, int>> pairVector){

    while(pairVector.size()!=0){

        int first_vector=pairVector[0].first;
        int second_vector=pairVector[0].second;


        finalResult_Vector_3.emplace_back(first_vector);
        finalResult_Vector_3.emplace_back(second_vector);


         for (auto it = pairVector.begin(); it != pairVector.end(); ) {
                if (it->first == first_vector || it->second == first_vector) {
                    it = pairVector.erase(it);
                } else {
                    ++it;
                }
        }

        for (auto it = pairVector.begin(); it != pairVector.end(); ) {
                if (it->first == second_vector || it->second == second_vector) {
                    it = pairVector.erase(it);
                } else {
                    ++it;
                }
        }

        
    
    }


}



void* findVectorCoverWrapper(void* args) {

    ThreadArgs* threadArgs = reinterpret_cast<ThreadArgs*>(args);

    clock_t start, finish;

	start = clock();
	    
    findVectorCover(threadArgs->v, threadArgs->pairVector);

    finish = clock();

	duration_1 = (double)(finish - start) / CLOCKS_PER_SEC;

    return nullptr;


}

void* findVectorCover_1Wrapper(void* args) {

    ThreadArgs* threadArgs = reinterpret_cast<ThreadArgs*>(args);

    clock_t start, finish;

	start = clock();

    findVectorCover_1(threadArgs->v, threadArgs->pairVector);

    finish = clock();

    duration_2 = (double)(finish - start) / CLOCKS_PER_SEC;

    return nullptr;
}

void* findVectorCover_2Wrapper(void* args) {

    ThreadArgs* threadArgs = reinterpret_cast<ThreadArgs*>(args);

    clock_t start, finish;
	start = clock();

    findVectorCover_2(threadArgs->v, threadArgs->pairVector);

    finish = clock();

    duration_3 = (double)(finish - start) / CLOCKS_PER_SEC;

    return nullptr;
}



void print_Vector1_result(){

    std::cout<<"CNF-SAT-VC: ";

    std::sort(finalResult_Vector_1.begin(), finalResult_Vector_1.end());
    
    // Printing the sorted vector
    for (int num : finalResult_Vector_1) {
        std::cout << num << " ";
    }

    std::cout <<"\n";

    // std::cout << "the duration time for algo 1 is: " << duration_1 << "\n";

    finalResult_Vector_1.clear();
}


void print_Vector2_result(){

    std::cout<<"APPROX-VC-1: ";

    std::sort(finalResult_Vector_2.begin(), finalResult_Vector_2.end());
    
    // Printing the sorted vector
    for (int num : finalResult_Vector_2) {
        std::cout << num << " ";
    }

    std::cout <<"\n";

    // std::cout << "the duration time for algo 2 is: " << duration_2 << "\n";

    finalResult_Vector_2.clear();

}

void print_Vector3_result(){

    std::cout<<"APPROX-VC-2: ";

    std::sort(finalResult_Vector_3.begin(), finalResult_Vector_3.end());
    
    // Printing the sorted vector
    for (int num : finalResult_Vector_3) {
        std::cout << num << " ";
    }

    std::cout <<"\n";

    // std::cout << "the duration time for algo 3 is: " << duration_3 << "\n";

    finalResult_Vector_3.clear();

}
// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <unordered_map>
#include <queue>
#include <list>

std::vector<int> processBFS(const std::unordered_map<int, std::list<int>> adjacenyList, const std::set<int> setVertices, int source, int destination);
void printShortestPath( std::vector<int> answer);


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
                std::cerr << "Error: Invalid input for vertices \n";
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
                std::cout << "Error: Missing vertices Input \n";
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
                std::cout << "Error: Invalid Edges \n";
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
                std::cout << "Error: Invalid Edges \n";
                continue;

            }

            // Create Adjaceny List
            adjacenyList.clear();

            for (const auto& pair : pairVector) {
            
                int vertex_1=pair.first;

                int vertex_2=pair.second;

                adjacenyList[vertex_1].push_back(vertex_2);
                adjacenyList[vertex_2].push_back(vertex_1);

            }

        }

        else if (command == 's'){

            bool noEdgeCounter=noEdge;

            if (vertices == 0 ){
                std::cout << "Error: Missing vertices information \n";
                noEdge=false;
                continue;
            }

            if (pairVector.size() == 0 && noEdge==false){

                vertices=0;
                std::cout << "Error: Missing Edges information  \n";
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
                std::cout << "Error: Vertex input invalid for command s \n";
                //noEdgeCounter=false;
                continue;
            }

            if (!(destination >=1 && destination <=vertices)){
                std::cout << "Error: Vertex input invalid for command s \n";
                //noEdgeCounter=false;
                continue;
            }

            if (source == destination){
                std::cout << "Error: Vertex input invalid for command s \n";
                //noEdgeCounter=false;
                continue;
            }

            if(noEdgeCounter==true){
                std::cout << "Error: Path does not exist \n";
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
                std::cout << "Error: Path does not exist \n";
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
            std::cout << "Error: Invalid Command \n";

        }

         
    }
   
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
        std::cout << "Error: Path does not exist";
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




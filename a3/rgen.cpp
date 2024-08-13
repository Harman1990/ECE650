// an example of reading random numbers from /dev/urandom
// https://stackoverflow.com/questions/35726331/c-extracting-random-numbers-from-dev-urandom
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <sstream>
#include <unistd.h>

int counter_Print=0;

std::vector<std::string> street_Names;

struct Point {
    int x, y;
};

struct LineSegment {
    Point start;
    Point end;
};

struct Street {
    std::string name;
    std::vector<LineSegment> lineSegments;
};


std::vector<Street> generateStreets(int argument_street_number, int argument_line_segments, int argument_coordinate);

bool checkStreets(std::vector<Street> streets_vector);
void printStreets(std::vector<Street> streets_vector);

void createStreets(int argument_street_number, int argument_line_segments, int argument_coordinate);




bool isPointOnSegment(const Point& point, const LineSegment& segment) {
    // Check if the point lies on the line defined by the two endpoints
    if ((point.x >= std::min(segment.start.x, segment.end.x) && point.x <= std::max(segment.start.x, segment.end.x)) &&
        (point.y >= std::min(segment.start.y, segment.end.y) && point.y <= std::max(segment.start.y, segment.end.y)) &&
        (point.x - segment.start.x) * (segment.end.y - segment.start.y) == (point.y - segment.start.y) * (segment.end.x - segment.start.x)) {
        return true;
    }
    return false;
}



int main(int argc, char *argv[]) {

    

    //Default Values
    int argument_street_number=10;
    int argument_line_segments=5;
    int argument_wait_time=5;
    int argument_coordinate=20;


    for (int i = 1; i < argc; ++i) {

        if (strcmp(argv[i], "-s") == 0) {

            //std::cout << "Found command -s" << std::endl;
            argument_street_number = std::stoi(argv[i+1]);
            if(argument_street_number<2){
                std::cerr << "Error: Wrong Command line input\n";
                exit(0);
            }
            //std::cout << argument_street_number << std::endl;
        }

        if (strcmp(argv[i], "-n") == 0) {

            //std::cout << "Found command -n" << std::endl;
            argument_line_segments = std::stoi(argv[i+1]);
            if(argument_line_segments<1){
                std::cerr << "Error: Wrong Command line input\n";
                exit(0);
            }
            //std::cout << argument_line_segments << std::endl;
        }

        if (strcmp(argv[i], "-l") == 0) {

            //std::cout << "Found command -l" << std::endl;
            argument_wait_time = std::stoi(argv[i+1]);
            if(argument_wait_time<5){
                std::cerr << "Error: Wrong Command line input\n";
                exit(0);
            }
            //std::cout << argument_wait_time << std::endl;
        }

        if (strcmp(argv[i], "-c") == 0) {

            //std::cout << "Found command -c" << std::endl;
            argument_coordinate = std::stoi(argv[i+1]);
            if(argument_coordinate<1){
                std::cerr << "Error: Wrong Command line input\n";
                exit(0);
            }
            //std::cout << argument_coordinate << std::endl;
        }


    
    }

    

    // open /dev/urandom to read
    std::ifstream urandom("/dev/urandom");

    // check that it did not fail
    if (urandom.fail()) {
      std::cerr << "Error: cannot open /dev/urandom\n";
        
    }

    unsigned int wait_time = 42;
    urandom.read((char *)&wait_time, sizeof(int));
    wait_time = (wait_time % (argument_wait_time - 4)) + 5;

     

    while(true){

        
        createStreets(argument_street_number, argument_line_segments, argument_coordinate);
        sleep(wait_time);
        
    }

    

    return 0;
}


void createStreets(int argument_street_number, int argument_line_segments, int argument_coordinate){

    int counter_Check=0;

    bool is_Streets_valid = false;

    std::vector<Street> streets_vector;  

    while(is_Streets_valid==false && counter_Check<25){

    
        streets_vector=generateStreets(argument_street_number, argument_line_segments, argument_coordinate);
        // printStreets(streets_vector);
        is_Streets_valid= checkStreets(streets_vector);
        ++counter_Check;
        
        
    }

    

    if(is_Streets_valid == true){
        printStreets(streets_vector);
        

    }
    else{
        
        std::cerr << "Error: failed to generate valid input for 25 simultaneous attempts \n";
        exit(0);
    }

    


}


std::vector<Street> generateStreets(int argument_street_number, int argument_line_segments, int argument_coordinate){

    

    std::vector<LineSegment> lines_vector;

    std::vector<Street> streets_vector;


    char alphabets[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    char street_name[10];

    // open /dev/urandom to read
    std::ifstream urandom("/dev/urandom");

    // check that it did not fail
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        
    }

  

    // Logic for generating random street number
    unsigned int street_Number = 42;
    urandom.read((char *)&street_Number, sizeof(int));
    street_Number=(street_Number % (argument_street_number-1)) +2;
    //std::cout << "Street Numbers: " << street_Number << "\n";

    

    for(size_t i=1;i<=street_Number;i++){

        unsigned int line_segments = 42;
        urandom.read((char *)&line_segments, sizeof(int));
        line_segments=(line_segments % (argument_line_segments)) +1;
        
        // long unsigned int
        for(size_t j=1;j<=line_segments;j++){

            if(lines_vector.size()==0){

                unsigned int x_coordinate = 42;
                urandom.read((char *)&x_coordinate, sizeof(int));
                x_coordinate=(x_coordinate % (2*argument_coordinate+1)) - argument_coordinate;
                //std::cout << "(" <<x_coordinate<<",";

                unsigned int y_coordinate = 42;
                urandom.read((char *)&y_coordinate, sizeof(int));
                y_coordinate=(y_coordinate % (2*argument_coordinate+1)) - argument_coordinate;
                //std::cout <<y_coordinate << ")";

                unsigned int x_coordinate_2 = 42;
                urandom.read((char *)&x_coordinate_2, sizeof(int));
                x_coordinate_2=(x_coordinate_2 % (2*argument_coordinate+1)) - argument_coordinate;
                //std::cout << "(" <<x_coordinate<<",";

                unsigned int y_coordinate_2 = 42;
                urandom.read((char *)&y_coordinate_2, sizeof(int));
                y_coordinate_2=(y_coordinate_2 % (2*argument_coordinate+1)) - argument_coordinate;
                //std::cout <<y_coordinate << ")";

                LineSegment ls;
                ls.start.x=x_coordinate;
                ls.start.y=y_coordinate;

                ls.end.x=x_coordinate_2;
                ls.end.y=y_coordinate_2;

                lines_vector.push_back(ls);

            }
            else{

                unsigned int x_coordinate = 42;
                urandom.read((char *)&x_coordinate, sizeof(int));
                x_coordinate=(x_coordinate % (2*argument_coordinate+1)) - argument_coordinate;
                //std::cout << "(" <<x_coordinate<<",";

                unsigned int y_coordinate = 42;
                urandom.read((char *)&y_coordinate, sizeof(int));
                y_coordinate=(y_coordinate % (2*argument_coordinate+1)) - argument_coordinate;
                //std::cout <<y_coordinate << ")";

                LineSegment ls;

                LineSegment last_ls=lines_vector.back();

                ls.start=last_ls.end;
        
                ls.end.x=x_coordinate;
                ls.end.y=y_coordinate;

                lines_vector.push_back(ls);
      

            }

        }


        // Logic for generating random street name
        int j=0;

        while(j<10){

            unsigned int alphabet = 42;
            urandom.read((char *)&alphabet, sizeof(int));
            alphabet=alphabet % 26;
            street_name[j] = alphabets[alphabet];

            j++;

        }

        

        std::string final_name ="";

        for(int i=0; i<10;i++){
            
            final_name=final_name+street_name[i];

        }

        std::string street_name = final_name;
    
        Street street;
        street.name=street_name;

        street.lineSegments=lines_vector;

        streets_vector.push_back(street);

   
        
        lines_vector.clear();

       

        

    }

    urandom.close();
    return streets_vector;


}


bool checkStreets(std::vector<Street> streets_vector){


    for (size_t i = 0; i < streets_vector.size(); ++i) {
        
        Street s1=streets_vector[i];
        //std::cout<< "Checking for street: "<<s1.name<<"\n";

        for(size_t k = 0; k < s1.lineSegments.size(); ++k){

          if((s1.lineSegments[k].start.x == s1.lineSegments[k].end.x) && (s1.lineSegments[k].start.y == s1.lineSegments[k].end.y)){
                //std::cerr<< "Zero Length Line segment for street:" << s1.name<<"\n";
                return false;
          }

        }


        for(size_t k = 0; k < s1.lineSegments.size(); ++k){


          for(size_t l = k+1; l < s1.lineSegments.size(); ++l)
          {


            if (isPointOnSegment(s1.lineSegments[l].start,s1.lineSegments[k])==true && isPointOnSegment(s1.lineSegments[l].end,s1.lineSegments[k])==true){
                //std::cerr<< "Line segment of street overlaps completely another line segment:" << s1.name<<" For line segments "<< s1.lineSegments[k].start.x<<","<<s1.lineSegments[k].start.y << "->"<<s1.lineSegments[k].end.x<<","<<s1.lineSegments[k].end.y<<"   ";
                //std::cerr<< s1.lineSegments[l].start.x<<","<<s1.lineSegments[l].start.y << "->"<<s1.lineSegments[l].end.x<<","<<s1.lineSegments[l].end.y<<"\n";
                return false;
            }    

            if (isPointOnSegment(s1.lineSegments[k].start,s1.lineSegments[l])==true && isPointOnSegment(s1.lineSegments[k].end,s1.lineSegments[l])==true){
                //std::cerr<< "Line segment of street overlaps completely another line segment2:" << s1.name<<" For line segments "<< s1.lineSegments[k].start.x<<","<<s1.lineSegments[k].start.y << "->"<<s1.lineSegments[k].end.x<<","<<s1.lineSegments[k].end.y<<"   ";
                //std::cerr<< s1.lineSegments[l].start.x<<","<<s1.lineSegments[l].start.y << "->"<<s1.lineSegments[l].end.x<<","<<s1.lineSegments[l].end.y<<"\n";
                return false;
            } 

            if((s1.lineSegments[k].start.x!=s1.lineSegments[l].start.x) && (s1.lineSegments[k].start.x!=s1.lineSegments[l].end.x) && (s1.lineSegments[k].end.x!=s1.lineSegments[l].start.x) && (s1.lineSegments[k].end.x!=s1.lineSegments[l].end.x)){

                if ((isPointOnSegment(s1.lineSegments[l].start,s1.lineSegments[k])==true || isPointOnSegment(s1.lineSegments[l].end,s1.lineSegments[k])==true) && (isPointOnSegment(s1.lineSegments[k].start,s1.lineSegments[l])==true || isPointOnSegment(s1.lineSegments[k].end,s1.lineSegments[l])==true)){
                
                //std::cerr<< "Line segment of street overlaps partially another line segment:" << s1.name<<" For line segments "<< s1.lineSegments[k].start.x<<","<<s1.lineSegments[k].start.y << "->"<<s1.lineSegments[k].end.x<<","<<s1.lineSegments[k].end.y<<"   ";
                //std::cerr<< s1.lineSegments[l].start.x<<","<<s1.lineSegments[l].start.y << "->"<<s1.lineSegments[l].end.x<<","<<s1.lineSegments[l].end.y<<"\n";
                return false;
                }

            }
             


            if((s1.lineSegments[k].start.x==s1.lineSegments[l].start.x && s1.lineSegments[k].start.y==s1.lineSegments[l].start.y) && (s1.lineSegments[k].end.x==s1.lineSegments[l].end.x && s1.lineSegments[k].end.y==s1.lineSegments[l].end.y)){
                //std::cerr<< "Common segment between same street:" << s1.name<<"\n";
                return false;
            }

            if((s1.lineSegments[k].start.x==s1.lineSegments[l].end.x && s1.lineSegments[k].start.y==s1.lineSegments[l].end.y) && (s1.lineSegments[k].end.x==s1.lineSegments[l].start.x && s1.lineSegments[k].end.y==s1.lineSegments[l].start.y)){

                //std::cerr<< "Common segment between same street 2:"<<s1.name<<"\n";
                return false;
                
            }



          }

        }

       
        
        for(size_t j = i+1; j < streets_vector.size(); ++j){
          Street s2=streets_vector[j];

          for (size_t k = 0; k < s1.lineSegments.size(); ++k) {

            for (size_t l = 0; l < s2.lineSegments.size(); ++l) {


                if (isPointOnSegment(s2.lineSegments[l].start,s1.lineSegments[k])==true && isPointOnSegment(s2.lineSegments[l].end,s1.lineSegments[k])==true){
                    //std::cerr<< "Line segment of street overlaps completely another line segment:" << s1.name<<" For line segments "<< s1.lineSegments[k].start.x<<","<<s1.lineSegments[k].start.y << "->"<<s1.lineSegments[k].end.x<<","<<s1.lineSegments[k].end.y<<"   ";
                    //std::cerr<< s2.name <<s2.lineSegments[l].start.x<<","<<s2.lineSegments[l].start.y << "->"<<s2.lineSegments[l].end.x<<","<<s2.lineSegments[l].end.y<<"\n";
                    return false;
                }  

                if (isPointOnSegment(s1.lineSegments[k].start,s2.lineSegments[l])==true && isPointOnSegment(s1.lineSegments[k].end,s2.lineSegments[l])==true){
                    //std::cerr<< "Line segment of street overlaps completely another line segment2:" << s1.name<<" For line segments "<< s1.lineSegments[k].start.x<<","<<s1.lineSegments[k].start.y << "->"<<s1.lineSegments[k].end.x<<","<<s1.lineSegments[k].end.y<<"   ";
                    //std::cerr<< s2.name << s2.lineSegments[l].start.x<<","<<s2.lineSegments[l].start.y << "->"<<s2.lineSegments[l].end.x<<","<<s2.lineSegments[l].end.y<<"\n";
                    return false;
                }


                if((s1.lineSegments[k].start.x!=s2.lineSegments[l].start.x) && (s1.lineSegments[k].start.x!=s2.lineSegments[l].end.x) && (s1.lineSegments[k].end.x!=s2.lineSegments[l].start.x) && (s1.lineSegments[k].end.x!=s2.lineSegments[l].end.x)){

                    if ((isPointOnSegment(s2.lineSegments[l].start,s1.lineSegments[k])==true || isPointOnSegment(s2.lineSegments[l].end,s1.lineSegments[k])==true) && (isPointOnSegment(s1.lineSegments[k].start,s2.lineSegments[l])==true || isPointOnSegment(s1.lineSegments[k].end,s2.lineSegments[l])==true)){
                
                    //std::cerr<< "Line segment of street overlaps partially another line segment:" << s1.name<<" For line segments "<< s1.lineSegments[k].start.x<<","<<s1.lineSegments[k].start.y << "->"<<s1.lineSegments[k].end.x<<","<<s1.lineSegments[k].end.y<<"   ";
                    //std::cerr<< s2.name << s2.lineSegments[l].start.x<<","<<s2.lineSegments[l].start.y << "->"<<s2.lineSegments[l].end.x<<","<<s2.lineSegments[l].end.y<<"\n";
                    return false;
                    }

                }
            

              if((s1.lineSegments[k].start.x==s2.lineSegments[l].start.x && s1.lineSegments[k].start.y==s2.lineSegments[l].start.y) && (s1.lineSegments[k].end.x==s2.lineSegments[l].end.x && s1.lineSegments[k].end.y==s2.lineSegments[l].end.y)){

                //std::cerr<< "Common line segment between streets: "<<s1.name<<" and " <<s2.name<<"\n";
                return false;
                
              }

              if((s1.lineSegments[k].start.x==s2.lineSegments[l].end.x && s1.lineSegments[k].start.y==s2.lineSegments[l].end.y) && (s1.lineSegments[k].end.x==s2.lineSegments[l].start.x && s1.lineSegments[k].end.y==s2.lineSegments[l].start.y)){

                //std::cerr<< "Common line segment 2  between streets:"<<s1.name<<" and " <<s2.name<<"\n";
                return false;
                
              }

          
            }

          }


        }


  }  


  return true;


}


void printStreets(std::vector<Street> streets_vector){


    std::ostringstream output;

    if(counter_Print>0){

       
        for(const auto &street_name : street_Names){

            output << "rm \""<<street_name<<"\"\n";

        }


    }

    street_Names.clear();


    for (const auto &street : streets_vector) {

        
        //output <<"rm \n"
        output << "add \"" << street.name << "\" ";

        for (size_t i = 0; i < street.lineSegments.size(); ++i) {
            LineSegment ls = street.lineSegments[i];
            output << "(" << ls.start.x << "," << ls.start.y << ") ";

            if (i == street.lineSegments.size() - 1) {
                output << "(" << ls.end.x << "," << ls.end.y << ")\n";
            }
        }


        street_Names.push_back(street.name);



    }

    // Print the entire output string
    std::cout << output.str() << "gg\n";
    std::cout.flush();

    ++counter_Print;

    
}


 



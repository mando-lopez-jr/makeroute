/*
THIS MAY CRASH YOUR COMPUTER IF YOU RUN IT

This program is not ready!
In it's current form, it has a bug that causes it to
endlessly write points to a file.

DO NOT RUN IT
*/

#include <iostream> //included to recieve input from user, will be removed once file input is implemented
#include <string> //included for strings
#include <fstream> //included for for file access
#include <cmath> //included for sqrt fun

/*
I've done this program as a single file to make things simpler.
This being said, I acknowledge it may have been cleaner to implement
in multiple files.
*/

/*
IT CONTAINS NO ERROR HANDLING

This program contains no error handling, nor do I plan to implement any.
When It's ultimately usable, I plan to include a sample file to show how
it's input should look, but as of now, it will simply crash,
or worse. (See above)
*/

class Coor{ //Implements a Coordinate pair
private:
    double x,y; 
public:
    Coor(){x=0;y=0;} //Default constructor with Coordinates at origin

    Coor(double i, double j){x=i;y=j;} //Creates a Coor Pair with points (i,j)
    
    Coor(std::string in, int line = 0){ //For creating Coor Pairs with GPS Coors formatted as x.xxx, y.yyy
        
        //Finds the separation between pairs
        int comma = in.find(", "); 

        //These two lines split in into lat and lon
        std::string lat = in.substr(0, comma);
        std::string lon = in.substr(1+comma, 100); //100 is an arbitraty number that should not reasonably be reached.

        //Converts lat and lon to C strings and converts those to doubles
        x = ::atof(lat.c_str());
        y = ::atof(lon.c_str());
    }

    virtual ~Coor(){}; //Deconstructor

    void operator= (Coor const &obj){ //Overloads the  = assignment operator
        x=obj.x;
        y=obj.y;
    }

    void operator+= (Coor const &obj){ //Overloads the += assignment operator
        x+=obj.x;
        y+=obj.y;
    }

    std::string tostr(){ //Converts the Coor to a string
        std::string i = std::to_string(x);
        std::string j = std::to_string(y);

        return i + ", " + j + "\n";
    }

    /*
    I BELIEVE THAT THE FOLLOWING TWO METHODS ARE THE SOURCE OF MY BUG.
    IT IS MY HOPE THAT NO ONE WILL EVER NEED TO READ THIS
    */

    Coor makeStep(Coor co2){ //Takes a second Coor and makes a step vector out of them
        
        //Creates a psuedo-vector out of the two Coors
        double vi = co2.x-x;
        double vj = co2.y-y;

        //Finds the magnitude of this "vector"
        double magnitude = sqrt((vi*vi)+(vj*vj));

        //Creates a "step" vector by making a unit vector and
        //dividing it by a, currently hard coded, step value
        vi = vi/magnitude/1000;
        vj = vj/magnitude/1000;

        //Returns the step vector as a Coor
        return Coor(vi, vj);
    }

    bool compCoor(Coor co2){ //Checks if two Coors are Equal. Will likely be replaced by an overloaded operator
        
        //This is a little janky, but since doubles can't be accurately compared
        //I've decided to check if they're sufficiently similar, i.e. their difference
        //is sufficiently small
        if ((x-co2.x <0.00005 && x-co2.x>-0.00005 ) && (y-co2.y < 0.00005 && y-co2.y > -0.00005)){
            return true;
        }
        else{
            return false;
        }
    }
};

int main(){ //As Anakin said, "This is where the fun begins"
    
    //Creates output file
    std::ofstream outf;
    outf.open("route.txt");

    //Creates input string and reads a starting value
    std::string input;
    std::cin>>input;

    //Instantiates the main Coors that this function will need
    //close is used to close the route
    //p1 and p2 are the primary points used
    //ps is a step point
    //pt is a temporary point
    Coor close, p1, p2, ps, pt;

    //Gives value to p1 and saves that value in a close variable
    p1 = Coor(input);
    close = p1;
    
    //Creates a bool to see if the while loop has run once yet
    bool firstpass = true;

    while (true){
        
        if (input == "close"){ //Breaks the while loop if close is entered
            break;
        }
        else if (firstpass){ //Reads a second input if this is the first loop
            std::cin>>input;
            firstpass = false;
            p2 = Coor(input);
        }
        else{ //Assigns input into p2
            p2 = Coor(input);
        }

        //Sets ps to a step vector        
        ps = p1.makeStep(p2);

        //Sets pt to p1
        pt = p1;

        //Prints the original pt to the file
        outf << pt.tostr();

        //Until pt is equal to p2, it increments it by ps and writes it to file
        while(!pt.compCoor(p2)){
            pt += ps;
            outf << pt.tostr();
        }

        //Sets p1 equal to p2 for the next pass
        p1 = p2;


        //Recieves an input for the next pass
        std::cin>>input;

    }

    //Closes the file and ends the program
    outf.close();
    return 0;
}
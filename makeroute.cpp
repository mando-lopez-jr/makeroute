/*
Multiline Comment

Psuedo-code

Select mode: degrees or decimal

input1 = f.readLine()

copy = input1

firstPass = true

while true

    if input1 == quit, break
    else if input1 == close, input2 = copy

    input2 = f.readline()

    vector1 = Vector.convert(input 1)

    vector2 = Vector.convert(input 2)

    vectorUnit = vector1.makeUnit(vector2)

    vectorCPY = vector1

    if firstpass, write vector1 to -o

    while !vector1.compare(vector2)

        write vector1.addVector(vectorUnit)
        vector1 = vector1.addVector(vectorUnit)

    vector1 = vector2


    
Vector
    

*/

#include <iostream>
#include <fstream>
#include <cmath>

class Coor{
private:
    double x,y;
public:
    Coor(){x=0;y=0;}

    Coor(double i, double j){x=i;y=j;}
    
    Coor(std::string in, int line = 0){
        
        int comma = in.find(", ");
        std::string lat = in.substr(0, comma);
        std::string lon = in.substr(1+comma, 100); //100 is an arbitraty number that should not reasonably be reached.

        x = ::atof(lat.c_str());
        y = ::atof(lon.c_str());
    }

    virtual ~Coor(){};

    void operator= (Coor const &obj){
        x=obj.x;
        y=obj.y;
    }

    void operator+= (Coor const &obj){
        x+=obj.x;
        y+=obj.y;
    }

    std::string tostr(){
        std::string i = std::to_string(x);
        std::string j = std::to_string(y);

        return i + ", " + j + "\n";
    }

/*    double parse(std::string in, int line){
        double out = ::atof(in.c_str());
        
        if(out > 1.0 && out < 1.0){
            float x = ::atof(in.substr(0,2).c_str());
            float y = ::atof(in.substr(3,2).c_str());
            float z = ::atof(in.substr(6,2).c_str());
            int sign = 0;
            if (in[9] == 'W' || in[9] == 'S' || in[9] == 'w' || in[9] == 's'){
                sign = -1;
            }
            else{
                sign = 1;
            }

            out = x+(y+(z/60));
        }

        return out;
        
    }*/

    Coor makeUnit(Coor co2){
        double vi = co2.x-x;
        double vj = co2.y-y;

        double magnitude = sqrt((vi*vi)+(vj*vj));


        vi = vi/magnitude/1000;
        vj = vj/magnitude/1000;

        return Coor(vi, vj);
    }

    bool compCoor(Coor co2){
        if ((x-co2.x <0.00005 && x-co2.x>-0.00005 ) && (y-co2.y < 0.00005 && y-co2.y > -0.00005)){
            return true;
        }
        else{
            return false;
        }
    }
};

int main(){
    std::ofstream outf;
    
    outf.open("route.txt");

    std::string input;
    
    std::cin>>input;

    Coor close, p1, p2, pu, pc;

    p1 = Coor(input);
    close = p1;
    bool firstpass = true;

    while (input != "quit"){
        if (input == "close"){
            p2 = close;
        }
        else if (firstpass){
            std::cin>>input;
            firstpass = false;
            p2 = Coor(input);
        }
        else{
            p2 = Coor(input);
        }

        pu = p1.makeUnit(p2);

        pc = p1;

        outf << pc.tostr();

        while(!pc.compCoor(p2)){
            pc += pu;
            outf << pc.tostr();
        }
        
        p1 = p2;

        std::cin>>input;

    }

    outf.close();

    return 0;
}
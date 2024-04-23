#include <iostream>
#include <string>

int main(int argc, char const *argv[])
{
    std::string gcode_bloque;
    
    if(argc<2){
        std::cout<<"G-code interprete"<<std::endl;

        while (true)
        {
            std::cout<<">>>";
            std::getline(std::cin,gcode_bloque);
            std::cout<<gcode_bloque<<std::endl;   
        }
        
    }
    return 0;
}

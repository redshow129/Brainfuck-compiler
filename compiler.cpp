#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// Коментарии компилятора
void Log(std::string type,std::string s){
    cout<<"["+type+"] "+ s<<endl;
}

// Вывод
void Print(char ch){
    cout<<ch;

}
// Ввод
char Input(){
    char out;
    cin>>out;
    return out;
}


int main(int argc, char* argv[]){
    std::vector<std::string> out = {
        "#include <iostream>",
        "#include <cstdint>",

        "using namespace std;",

        "void Print(uint8_t ch){cout.put(static_cast<char>(ch));cout.flush();}",
        "char Input(){int c = cin.get(); if(c == EOF) return 0; return static_cast<char>(c);}",

        "int main(){",
        "char ar[30000] = {};"
    };

    Log("INFO","Starting app");
    // argc[1] - путь к файлу
    try{
        ifstream file;
        string path = (argc > 1) ? argv[1] : "main.bf";

        file.open(path); // argc[1]
        if (file.is_open()){
            Log("INFO","Opening file "+path);

            ofstream file2;
            Log("INFO","Opening cache file "+path);
            file2.open("c.cpp");
            if (!file2.is_open()){
                Log("ERROR","Didnt open file");
                return -1;
            }
            
            int i = 0; // указатель
            int plus = 0; // сколько прибавить / убавить

            std::string line;
            int line_number = 1;
            while(getline(file, line)){
                for (int j = 0; j < line.length(); j++)
                switch (line[j])
                {
                    case '>': {
                        //out.push_back("i++;");
                        i++;
                        break;
                    }
                    case '<': {
                        //out.push_back("i--;");
                        i--;
                        break;
                    }
                    case '+': {
                        //out.push_back("ar["+to_string(i)+"]++;");
                        plus += 1;
                        if (line[j+1] != '+' && line[j+1] != '-'){
                            out.push_back("ar["+to_string(i)+"]+="+to_string(plus)+";");
                            plus = 0;
                        }
                            
                        break;
                    }
                    case '-': {
                        //out.push_back("ar["+to_string(i)+"]--;");
                        plus -= 1;
                        if (line[j+1] != '+' && line[j+1] != '-'){
                            out.push_back("ar["+to_string(i)+"]+="+to_string(plus)+";");
                            plus = 0;
                        }
                        break;
                    }
                    case '.': {
                        out.push_back("Print(ar["+to_string(i)+"]);");
                        break;
                    }
                    case ',': {
                        out.push_back("ar["+to_string(i)+"] = Input();");
                        break;
                    }
                    case '[': {
                        out.push_back("while(ar["+to_string(i)+"]){");
                        break;
                    }
                    case ']': {
                        out.push_back("}");
                        break;
                    }
                    // любой другой синвол эгнорируется
                }
                
            }
            out.push_back("return 0;}");
            for(int i=0; i<out.size(); i++)
                file2 << out[i]+"\n";
            file2.close();
            Log("INFO","Code successfulled formated to C++ to c.cpp!");

            int status = std::system("g++ -O2 c.cpp -o out");

            if (status == 0)
                Log("OK","Program successfull compiled to out!");
            else
                Log("ERROR","Program didnt compiled, error code: "+to_string(status));
            

            //std::error_code ec; // Для безопасной обработки ошибок без исключений

            /*
            if (fs::remove("c.cpp", ec))
                Log("INFO","Deleted c.cpp file!");
            else if (ec)
                Log("ERROR","Didnt deleted c.cpp file: "+ec.message());
            else 
                Log("ERROR","where is file?");
            */
        }else{
            Log("ERROR","Didnt opened file "+path);
            return -1;
        }


    }catch(const exception& e){
        Log("ERROR",e.what());
    }
    Log("INFO","Done!");
    cout<<"\n";


    int status = std::system("./out");
    cout<<"\n\n";
    Log("INFO","Program exited with code: "+to_string(status));

    return 0;
}
#include <fstream>
#include <iostream>
#include "formulae.h"
#include "errors.h"
#include "boolexpr.h"

int main(int argc, char* argv[])
{
    char buf[1024];
    if (argc == 4)
    {
        std::ifstream input;
        std::ofstream output;
        input.open(std::string(argv[2]));
        output.open(std::string(argv[3]));
        if (!input.is_open())
        {
            std::cout << "ERROR: CANNOT OPEN AN INPUT FILE!";
            return -1;
        }
        if (!output.is_open())
        {
            std::cout << "ERROR: CANNOT OPEN AN OUTPUT FILE!";
            return -1;
        }

        if (std::string(argv[1]) == "-calc")
        {
            while (!input.eof())
            {
                input.getline(buf, 1024);
                if (strlen(buf) == 0) break;
                Formula F(buf);
                std::cout << F.str() << ":\n";
                int result = F.calc();
                std::cout << "\n";
                output << result << "\n";
                global::Workspace.clear();
            }
        }
        else if (std::string(argv[1]) == "-cnf")
        {
            while (!input.eof())
            {
                input.getline(buf, 1024);
                if (strlen(buf) == 0) break;
                Formula F(buf);
                std::string ans = F.saveCnf();
                output << ans << "\n";
                global::Workspace.clear();
            }
        }
        else if (std::string(argv[1]) == "-dnf")
        {
            while (!input.eof())
            {
                input.getline(buf, 1024);
                if (strlen(buf) == 0) break;
                Formula F(buf);
                std::string ans = F.saveDnf();
                output << ans << "\n";
                global::Workspace.clear();
            }
        }
        else if (std::string(argv[1]) == "-zh")
        {
            while (!input.eof())
            {
                input.getline(buf, 1024);
                if (strlen(buf) == 0) break;
                Formula F(buf);
                std::string ans = F.saveZh();
                output << ans << "\n";
                global::Workspace.clear();
            }
        }
        else if (std::string(argv[1]) == "-isfull")
        {
            std::vector<std::vector<bool>> funcs;
            while (!input.eof())
            {
                input.getline(buf, 1024);
                if (strlen(buf) == 0) break;
                Formula F(buf);
                funcs.push_back(F.getCompleteness());
                global::Workspace.clear();
            }
            bool isFull = true;
            for (int i = 0; i < 5; ++i)
            {
                bool isAll = true;
                for (auto vec : funcs)
                {
                    isAll &= vec[i];
                }
                if (isAll)
                    isFull = false;
            }
            if (isFull)
                output << "yes";
            else
                output << "no";
        }
    }
    else if (argc == 3)
    {
        std::ifstream input;
        input.open(std::string(argv[2]));
        if (!input.is_open())
        {
            std::cout << "ERROR: CANNOT OPEN AN INPUT FILE!";
            return -1;
        }
        if (std::string(argv[1]) == "-table")
        {
            while (!input.eof())
            {
                input.getline(buf, 1024);
                if (strlen(buf) == 0) break;
                Formula F(buf);
                std::cout << F.str() << ":\n";
                F.printTable();
                std::cout << "\n";
                global::Workspace.clear();
            }
        }
    }
    else if (argc == 2)
    {
        if (std::string(argv[1]) == "?" || std::string(argv[1]) == "-h")
        {
            std::cout << "[-cnf][-dnf][-zh] [inputFile] [outputFile]: Saves different forms of functions in [inputFile] to [outputFile]\n";
            std::cout << "[-isfull] [inputFile] [outputFile]: Determines whether functions in [inputFile] are complete and saves answer to [outputFile]\n";
            std::cout << "[-table] [inputFile]: Prints truth table of functions in [inputFile] to console\n";
            std::cout << "[?][-h]: Prints help information to console\n";
        }
    }
    
}
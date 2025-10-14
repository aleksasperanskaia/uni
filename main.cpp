#include <fstream>
int main(int argc, char* argv[]) {
    std::ifstream in(argv[1]);
    int variables;
    in >> variables;
    in >> variables;
    char temp;
    in.get(temp);
    int variants = 1 << variables;
    bool *function = new bool[variants];
    int zeros = 0; int one = 0;
    for(int i = 0; i < variants; i++)
    {
        in.get(temp);
        function[i] = temp-48;
        if(function[i] == true) {
            one++;
        } else {
            zeros++;
        }
    }
    in.close();
    std::ofstream out(argv[2]);
    if(one == 0 || zeros == 0) {
        out << '1';
        delete[] function;
        out.close();
        return 0;
    } else if (one != zeros) {
        out << '0';
        delete[] function;
        out.close();
        return 0;
    }
    int temp2 = 1;
    for(int temp2 = 1; temp2 != variants; temp2=temp2<<1)
    {
        for(int i = temp2; i < variants; i+=temp2<<1)
        {
            for(int w = 0; w < temp2; w++)
            {
                function[i+w] = (function[i-temp2+w] != function[i+w]);
            }
        }
    }
    zeros = 1;
    for(one = 1; one < variants; one++)
    {
        if(one == zeros && variants != zeros){
            zeros = zeros << 1;
        }
        else if(function[one])
        {
            out << '0';
            delete[] function;
            out.close();
            return 0;
        }
    }
    out << '1';
    delete[] function;
    out.close();
    return 0;
}

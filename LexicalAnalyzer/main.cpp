#include <fstream>
#include "analyzer.h"

#define dbg(n) cout<<"Debug "<<n<<endl
#define debg(s,n) cout<<s<<" : "<<n<<endl
using namespace std;

int main()
{
    analyzer x("configuration.in");
    x.start();

    ofstream out("out.txt");

    while(true){
        token t = x.getToken();
        out<<t.getType()<<endl;
        if(t.getType() == "END_OF_FILE")break ;
    }
}



#include <fstream>
#include "analyzer.h"
#include "symbol_table.h"

#define dbg(n) cout<<"Debug "<<n<<endl
#define debg(s,n) cout<<s<<" : "<<n<<endl
using namespace std;

int main()
{
    analyzer x("configuration.in");
    symbol_table table ;
    x.start();

    ofstream out("out.txt");

    if( x.hasError() ){
        out<<"FILE ERRORS";
        return 0 ;
    }

    while(true){
        token t = x.getToken();
        if( !table.exists(t.getValue()) ){
            table.add(t.getValue(),entry(t.getValue(),t.getType()));
        }

        out<<t.getType()<<endl;
        if(t.getType() == "END_OF_FILE")break ;
    }
}



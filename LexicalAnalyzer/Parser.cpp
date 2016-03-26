#include "Parser.h"
#include <iostream>


//Initializes the singelton of the parse
Parser* Parser::instance = NULL;

Parser* Parser::getInstance(){
	if (!instance)
		instance = new Parser();

	return instance;
}

Parser::Parser(){

}

Automata* Parser::getNFA(string filename){

	def.clear();
	keywords.clear();
	start_states.clear();
	regexs.clear();
	delim.clear();

	p = 1;
	file.open(filename, ifstream::in);
	read_file();
	create_keywords_states();
	evaluate_regex();
	make_one_start();
	return new Automata(start, ENFA);
}

void Parser::read_file()
{
    string line;
    while(getline(file,line))
        process_line(line);
}
void Parser::process_line(string line)
{
    int tol = -1;/* type of line */
    trim(line);
    tol = type_of_line(line);
    if(tol == -1)
        return;
    if(tol == PUNC)
        handle_punc(line);
    else if(tol == KEYWORD)
        handle_keywords(line);
    else if(tol == DEF)
        handle_def(line);
    else
        handle_reg(line);

}

void Parser::trim(string &line)
{
    int i =0,j = line.size()-1;
    while(i < line.size() && ( line[i] ==' '|| line[i] =='\t') )
        i++;
    while(j>=0 && (line[j] == ' ' || line [j] =='\t') )
        j--;
    line = line.substr(i,j-i+1);
    return ;
}
int Parser::type_of_line(string line)
{
    if(line[0] == '{')
        return KEYWORD;
    if(line[0] == '[')
        return PUNC;
    for(int i = 0 ;i  < line.size();i++)
    {
        if(line[i] == '=')
            return DEF;
        if(line[i] == ':')
            return REGX;
    }
    return -1;
}
void Parser::handle_punc(string line)
{
    for(int i = 1 ; i < line.size()-1;i++)
    {
        if(line[i] == ' ' || line[i] == '\\'|| line[i]=='\t')
            continue;
        delim.push_back(line[i]);
    }
}
void Parser::handle_keywords(string line)
{
    int start =-1;
    for(int i = 1 ; i < line.size();i++)
    {
        if(line[i] == ' ' || line[i] == '\\' || line[i]=='\t'|| line[i]=='}')
        {
            if(start !=-1)
                keywords.push_back(line.substr(start,i-start));
            start =-1;
            continue;
        }
        if(start == -1)
            start =i;
    }
}
void Parser::handle_def(string line)
{
    int i = get_ind(line ,'=');
    string name = get_name(line,i) ;
    i++;
    string temp =decode(line,i);
    def[name] = temp;
}
void Parser::handle_reg(string line)
{
    int i = get_ind(line ,':');
    string name = get_name(line,i) ;
    i++;
    string temp =decode(line,i);
    regexs.push_back(make_pair(name,temp));
}
int Parser:: get_ind(string line ,char del)
{
    int i = 0 ;
    while(i<line.size() && line[i]!=del)
        i++;
    return i;
}
string Parser:: get_name(string line,int ind)
{
    int j = ind - 1;
    while(j>=0 && (line[j] == ' ' || line [j] =='\t'))
        j--;
    return line.substr(0,j+1);
}
string Parser:: get_enclosed(char c)
{
    string temp="";
    if (is_reserved_symbol(c))
        temp+='\\';
    temp+=c;
    return temp;
}

string Parser::decode_or(int from, int to)
{
    string temp ="";
    from= from+256;
    to = to+256;
    from%=256;
    to%=256;
    temp+="("+get_enclosed(from);
    for(int x =from+1 ; x <= to ; x++)
        temp+="|" +get_enclosed((char)x);
    temp+=")";
    return temp;
}
string Parser::decode(string line ,int ind)
{
    int last1=237,last2=237;
    string temp ="";
    for( int i = ind ; i <line.size() ;i++)
    {
        if( line[i] == ' '|| line[i] == '\t')
            continue;
        if(is_reserved_symbol(line[i])&& line[i]!='-' &&(i ==0 || line[i-1] != '\\'))
        {
            if(last1 != 237)
                temp+=get_enclosed(last1);
            temp+=line[i],last1=237,last2=237;
            continue;
        }
        if(line[i] == '\\' &&(i ==0 || line[i-1] != '\\'))
            continue;
        if(line [i] == '-' && (i ==0 || line[i-1] != '\\') )
            last2 = last1,last1 = line[i];
        else
        {
            if(i>0 && line[i-1]=='\\' && line[i]=='L' )
                line[i]=238;
            else
            {
                int j = i+1;
                while(j<line.size() && (!is_reserved_symbol(line[j]) && line[j]!=' '&& line[j]!='\t'&& line[j]!='\\'))
                    j++;
                j--;
                if(i != j)
                {
                    if(last1 != 237)
                        temp+=get_enclosed(last1);
                    string x = def[line.substr(i,j-i+1)];
                    if(x=="")
                        x = line.substr(i,j-i+1);
                    temp+=x,last1=237,last2=237;
                    i=j;
                    continue;
                }
            }
            if(last1 == '-'&& last2!= 237)
                temp+=decode_or(last2,line[i]),last1=237,last2=237;
            else
            {
                if(last1 != 237)
                    temp+=get_enclosed(last1);
                last1=line[i],last2=237;
            }
        }
    }
    if(last1 != 237)
        temp+=get_enclosed(last1);
    temp = "("+temp+ ")";
    return temp;
}
bool Parser::is_reserved_symbol(char c)
{
    return (c == '|' ||c == '+' || c == '(' || c==')' || c == '*'|| c=='-');
}
void Parser::create_keywords_states()
{
    for(string s:keywords)
    {
        State *start = new State(s,0,p),*temp,*next = new State(s,0,p);
        temp = start;
        for(char c:s)
        {
            temp->addTransition(c,next);
            temp = next;
            next = new State(s,0,p);
        }
        temp->setAcceptanceState(s);
        start_states.push_back(start);
        p++;
    }
}

void Parser::evaluate_regex()
{
    for(int i = 0 ; i <regexs.size();i++)
    {
        string name = regexs[i].first;
        string reg = regexs[i].second;
        stack<char> op;
        stack<pair <State*,State*> > st;
        char last_op='|';
        for(int i = 0 ; i <reg.size();i++)
        {
            if(reg[i]=='\\'&&(i==0 || reg[i-1] !='\\'))
                continue;
            if((is_reserved_symbol(reg[i]) && i>0 && reg[i-1] =='\\')|| !is_reserved_symbol(reg[i])||reg[i]=='(')
            {
                if(last_op !='(' && last_op !='|')
                {
                    while(!op.empty())
                    {
                        char c = op.top();
                        if(c =='(' || c =='|')
                            break;
                        op.pop();
                        solve(st,c);
                    }
                    op.push('c');
                }
                if(reg[i]=='(')
                    op.push('(');
                else
                    create_state(st,name,reg[i]);
                if(!is_reserved_symbol(reg[i]))
                    last_op = reg[i];
                else
                    last_op = (char)237;
            }
            else
            {
                if(reg[i]!='*'&& reg[i]!='+')
                {
                    while(!op.empty())
                    {
                        char c = op.top();
                        if(c =='(')
                        {
                            if(reg[i]==')')
                                op.pop();
                            break;
                        }
                        if(reg[i]=='c' && c == '|')
                            break;
                        op.pop();
                        solve(st,c);
                    }
                }
                if(reg[i]!=')')
                    op.push(reg[i]);
            }
            last_op = reg[i];

        }
        while(!op.empty())
        {
            char c = op.top();
            op.pop();
            solve(st,c);
        }
        st.top().second->setAcceptanceState(name);
        start_states.push_back(st.top().first);
        p++;
    }
}
void  Parser::create_state(stack<pair<State* , State*> > &st,string name,char c)
{
    State *start = new State(name,0,p);
    State *en = new State(name,0,p);
    start->addTransition(c,en);
    st.push(make_pair(start,en));
}
void Parser::concatenate(stack<pair<State* , State*> > &st)
{
    State *s1 = st.top().first;
    State *e1 = st.top().second;
    st.pop();
    State *s2 = st.top().first;
    State *e2 = st.top().second;
    st.pop();
    e2->addTransition(Eps,s1);
    st.push(make_pair(s2,e1));
}

void Parser::zero_or_more(stack<pair<State* , State*> > &st)
{
    one_or_more(st);
    State *s1 = st.top().first;
    State *e1 = st.top().second;
    st.pop();
    State *s2 = new State(s1->name,0,p);
    State *e2 = new State(s1->name,0,p);
    s2->addTransition(Eps,s1);
    e1->addTransition(Eps,e2);
    s2->addTransition(Eps,e2);
    st.push(make_pair(s2,e2));
}
void Parser::one_or_more(stack<pair<State* , State*> > &st)
{
    State *s1 = st.top().first;
    State *e1 = st.top().second;
    e1->addTransition(Eps,s1);
}
void Parser::or_op(stack<pair<State* , State*> > &st)
{
    State *s1 = st.top().first;
    State *e1 = st.top().second;
    st.pop();
    State *s2 = st.top().first;
    State *e2 = st.top().second;
    st.pop();
    if(s2->isDummy)
    {
        s2->addTransition(Eps,s1);
        e1->addTransition(Eps,e2);
        st.push(make_pair(s2,e2));
        return;
    }
    State *s3 = new State(s1->name,0,p,true);
    State *e3 = new State(s1->name,0,p,true);
    s3->addTransition(Eps,s1);
    s3->addTransition(Eps,s2);
    e1->addTransition(Eps,e3);
    e2->addTransition(Eps,e3);
    st.push(make_pair(s3,e3));
}
void Parser::solve(stack<pair<State* , State*> > &st , char c)
{

    if(c == 'c')
        concatenate(st);
    else if(c =='+')
        one_or_more(st);
    else if(c == '*')
        zero_or_more(st);
    else
        or_op(st);
}
void Parser::make_one_start()
{
    if(start_states.size()==0)
        return;
    if(start_states.size() ==1)
    {
        start =start_states[0];
        return;
    }
    start = new State();
    for(State* s:start_states)
        start->addTransition(Eps,s);
}

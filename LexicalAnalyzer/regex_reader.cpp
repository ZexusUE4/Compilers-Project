#include "regex_reader.h"
#include <iostream>


//Initializes the singelton of the parse
regex_reader* regex_reader::instance = NULL;

regex_reader* regex_reader::getInstance(){
	if (!instance)
		instance = new regex_reader();

	return instance;
}

regex_reader::regex_reader(){

}

automata* regex_reader::get_nfa(string filename){

	def.clear();
	keywords.clear();
	start_states.clear();
	regexs.clear();
	delim.clear();

	p = 1;
	file.open(filename, ifstream::in);
	read_file();
	create_keywords_states();
    create_delim_states();
	evaluate_regex();
	make_one_start();
	return new automata(start, ENFA);
}

void regex_reader::read_file()
{
    string line;
    while(getline(file,line))
        process_line(line);
}
void regex_reader::process_line(string line)
{
    if(line.size()==0)
        return;

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

void regex_reader::trim(string &line)
{
    int i =0,j = line.size()-1;
    while(i < (int)line.size() && ( line[i] ==' '|| line[i] =='\t') )
        i++;
    while(j>=0 && (line[j] == ' ' || line [j] =='\t') )
        j--;
    line = line.substr(i,j-i+1);
    return ;
}
int regex_reader::type_of_line(string line)
{
    if(line[0] == '{')
        return KEYWORD;
    if(line[0] == '[')
        return PUNC;
    for(int i = 0 ;i  < (int)line.size();i++)
    {
        if(line[i] == '=')
            return DEF;
        if(line[i] == ':')
            return REGX;
    }
    return -1;
}
void regex_reader::handle_punc(string line)
{
    for(int i = 1 ; i < (int)line.size()-1;i++)
    {
        if(line[i] == ' ' || line[i] == '\\'|| line[i]=='\t')
            continue;
        delim.push_back(line[i]);
    }
}
void regex_reader::handle_keywords(string line)
{
    int start =-1;
    for(int i = 1 ; i < (int)line.size();i++)
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
void regex_reader::handle_def(string line)
{
    int i = get_ind(line ,'=');
    string name = get_name(line,i) ;
    i++;
    string temp =decode(line,i);
    def[name] = temp;
}
void regex_reader::handle_reg(string line)
{
    int i = get_ind(line ,':');
    string name = get_name(line,i) ;
    i++;
    string temp =decode(line,i);
    regexs.push_back(make_pair(name,temp));
}
int regex_reader:: get_ind(string line ,char del)
{
    int i = 0 ;
    while(i< (int)line.size() && line[i]!=del)
        i++;
    return i;
}
string regex_reader:: get_name(string line,int ind)
{
    int j = ind - 1;
    while(j>=0 && (line[j] == ' ' || line [j] =='\t'))
        j--;
    return line.substr(0,j+1);
}
string regex_reader:: get_enclosed(char c)
{
    string temp="";
    if (is_reserved_symbol(c))
        temp+='\\';
    temp+=c;
    return temp;
}

void regex_reader::decode_or(stack<pair<state* , state*> > &st,int from , int to)
{
    from= from+256;
    to = to+256;
    from%=256;
    to%=256;
    state *s1 = st.top().first;
    state *e1 = st.top().second;
    for(int i = from+1 ; i<=to;i++)
        s1->add_transiton((char)i,e1);

}
string regex_reader::enclose_or(int from, int to)
{
    string temp ="";
    from= from+256;
    to = to+256;
    from%=256;
    to%=256;
    temp+="("+get_enclosed((char)from)+"-"+get_enclosed((char)to)+")";
    return temp;
}
string regex_reader::decode(string line ,int ind)
{
    int last1=237,last2=237;
    string temp ="";
    for( int i = ind ; i < (int)line.size() ;i++)
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
                while(j < (int)line.size() && (!is_reserved_symbol(line[j]) && line[j]!=' '&& line[j]!='\t'&& line[j]!='\\'))
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
                temp+=enclose_or(last2,line[i]),last1=237,last2=237;
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
bool regex_reader::is_reserved_symbol(char c)
{
    return (c == '|' ||c == '+' || c == '(' || c==')' || c == '*'|| c=='-');
}
void regex_reader::create_keywords_states()
{
    for(string s:keywords)
    {
        state *start = new state(s,0,p),*temp,*next;
        temp = start;
        for(char c:s)
        {
            next = new state(s,0,p);
            temp->add_transiton(c,next);
            temp = next;
        }
        temp->set_acceptance_state(s);
        start_states.push_back(start);
        p++;
    }
}

void regex_reader::create_delim_states()
{
	for (char c : delim)
	{
		string str = "";
		str += c;
		state *s = new state(str, 0, p), *en = new state(str, 1, p);
		s->add_transiton(c, en);
		start_states.push_back(s);
		p++;
	}
}

void regex_reader::evaluate_regex()
{
    for(int i = 0 ; i <(int) regexs.size();i++)
    {
        string name = regexs[i].first;
        string reg = regexs[i].second;
        stack<char> op;
        stack<pair <state*,state*> > st;
        char last_op='|',from = 0,to = 0;
        for(int i = 0 ; i < (int) reg.size();i++)
        {
            if(reg[i]=='\\'&&(i==0 || reg[i-1] !='\\'))
                continue;
            if((is_reserved_symbol(reg[i]) && i>0 && reg[i-1] =='\\')|| !is_reserved_symbol(reg[i])||reg[i]=='(')
            {
                if(last_op == '-')
                {
                    last_op =(char)237;
                    to =reg[i];
                    continue;
                }
                if(last_op !='(' && last_op !='|')
                {
                    while(!op.empty())
                    {
                        char c = op.top();
                        if(c =='(' || c =='|')
                            break;
                        op.pop();
                        solve(st,c,from,to);
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
                        solve(st,c,from ,to);
                    }
                    if(reg[i]=='-')
                        from = last_op;
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
            solve(st,c,from,to);
        }
        st.top().second->set_acceptance_state(name);
        start_states.push_back(st.top().first);
        p++;
    }
}
void  regex_reader::create_state(stack<pair<state* , state*> > &st,string name,char c)
{
    state *start = new state(name,0,p);
    state *en = new state(name,0,p);
    start->add_transiton(c,en);
    st.push(make_pair(start,en));
}
void regex_reader::concatenate(stack<pair<state* , state*> > &st)
{
    state *s1 = st.top().first;
    state *e1 = st.top().second;
    st.pop();
    state *s2 = st.top().first;
    state *e2 = st.top().second;
    st.pop();
    e2->add_transiton(Eps,s1);
    st.push(make_pair(s2,e1));
}

void regex_reader::zero_or_more(stack<pair<state* , state*> > &st)
{
    one_or_more(st);
    state *s1 = st.top().first;
    state *e1 = st.top().second;
    st.pop();
    state *s2 = new state(s1->name,0,p);
    state *e2 = new state(s1->name,0,p);
    s2->add_transiton(Eps,s1);
    e1->add_transiton(Eps,e2);
    s2->add_transiton(Eps,e2);
    st.push(make_pair(s2,e2));
}
void regex_reader::one_or_more(stack<pair<state* , state*> > &st)
{
    state *s1 = st.top().first;
    state *e1 = st.top().second;
    e1->add_transiton(Eps,s1);
}
void regex_reader::or_op(stack<pair<state* , state*> > &st)
{
    state *s1 = st.top().first;
    state *e1 = st.top().second;
    st.pop();
    state *s2 = st.top().first;
    state *e2 = st.top().second;
    st.pop();
    if(s2->is_dummy)
    {
        s2->add_transiton(Eps,s1);
        e1->add_transiton(Eps,e2);
        st.push(make_pair(s2,e2));
        return;
    }
    state *s3 = new state(s1->name,0,p,true);
    state *e3 = new state(s1->name,0,p,true);
    s3->add_transiton(Eps,s1);
    s3->add_transiton(Eps,s2);
    e1->add_transiton(Eps,e3);
    e2->add_transiton(Eps,e3);
    st.push(make_pair(s3,e3));
}
void regex_reader::solve(stack<pair<state* , state*> > &st , char c,char from , char to)
{

    if(c == 'c')
        concatenate(st);
    else if(c =='+')
        one_or_more(st);
    else if(c == '*')
        zero_or_more(st);
    else if(c =='|')
        or_op(st);
    else
        decode_or(st,from,to);
}

void regex_reader::make_one_start()
{
    if(start_states.size()==0)
        return;
    if(start_states.size() ==1)
    {
        start =start_states[0];
        return;
    }
    start = new state();
    for(state* s:start_states)
        start->add_transiton(Eps,s);
}

#include "code_generator.h"
#include <iostream>

code_generator* code_generator::_instance = NULL;

code_generator::code_generator()
{
    err_cnt = 0;
    out1.open("compiled_code.j",ios_base::binary);
    out2.open("jasmin.j",ios_base::binary);
}

code_generator* code_generator::instance()
{
    if(_instance == NULL)
    {
        _instance = new code_generator();
    }
    return _instance;
}

void code_generator::register_call_back(int stack_size, psymbol* node){
    call_back_map[stack_size].push(node);
}

void code_generator::check_for_call_backs(int cur_stack_size)
{
    if(call_back_map.find(cur_stack_size) != call_back_map.end()){
        while(call_back_map[cur_stack_size].size()){
            psymbol* node = call_back_map[cur_stack_size].top();
            node->call_back(node);
            call_back_map[cur_stack_size].pop();
        }
    }
};

void code_generator::throw_error(string err_msg, string action)
{
    cout << "Error no.: " << (++err_cnt) << "\t";
    cout << "Msg: " + err_msg << "\t";
    cout << "Action: " + action << endl;
}

 void code_generator::binary_arth( string type_1 , string type_2 , op_type op )
{
	if( type_1 == type_2 ){
		if( type_1 == "int" ){
			add_instruction(arth_op(op,"int"));
		}
		else if( type_1 == "float" ){
			add_instruction(arth_op(op,"float"));
		}
	}
	else{
		solve_casting(type_1,type_2);
		add_instruction(arth_op(op,"float"));
	}
}

void code_generator::add_instruction(string inst)
{
    lines.push_back(inst);
}

void code_generator::back_patch(vector<int>* keys, int line_no)
{
    vector<int> ks = *keys;
    for(int k: ks)
    {
        back_patch(k,line_no);
    }
}

void code_generator::back_patch(int key, int line_no)
{
    lines[key] += "L" + int_to_string(line_no);
}

void code_generator::commit_code()
{
    print_start_template(out1);
    print_start_template(out2);
    string store= "store";
    for(int i = 0 ; i < lines.size() ; ++i){
        if( lines[i].size() )
        {
            out1 << "\t\tL" << i << ":\n\t\t\t" << lines[i] <<endl;
            out2 << "\t\tL" << i << ":\n\t\t\t" << lines[i] <<endl;
            int pos= lines[i].find(store);
            if(pos != string::npos)
            {
                string type= (lines[i][0] == 'i') ? "int" : "float";
                int index = string_to_int(lines[i], 7);
                printing_template(out2, index, type);
            }
        }
    }
    out1 << "\t\tL" << lines.size() << ":" << endl; // Last label must be printed
    out2 << "\t\tL" << lines.size() << ":" << endl; // Last label must be printed
    print_end_template(out1);
    print_end_template(out2);
}

int code_generator::get_program_counter()
{
    return lines.size();
}

string code_generator::int_to_string( int num )
{
   char x[100];
   sprintf(x,"%d",num);
   return string(x);
}

string code_generator::float_to_string( float num )
{
   char x[100];
   sprintf(x,"%f",num);
   return string(x);
}

string code_generator::get_comp_inst(string relop,string data_type)
{
    relop_type t = get_relop_type(relop);

	if(data_type == "int"){
		switch(t)
		{
			case relop_type::GT:
			return "if_icmple ";
			case relop_type::LT:
			return "if_icmpge ";
			case relop_type::GE:
			return "if_icmplt ";
			case relop_type::LE:
			return "if_icmpgt ";
			case relop_type::EQ:
			return "if_icmpne ";
			case relop_type::NQ:
			return "if_icmpeq ";
			return "Invalid_type";
		}
	}
	else{
		switch(t)
		{
			case relop_type::GT:
				return "ifle ";
			case relop_type::LT:
				return "ifge ";
			case relop_type::GE:
				return "iflt ";
			case relop_type::LE:
				return "iflgt ";
			case relop_type::EQ:
				return "ifne ";
			case relop_type::NQ:
				return "ifeq ";
		}
	}
}
relop_type code_generator::get_relop_type(string relop_val)
{
    if(relop_val == ">")
        return relop_type::GT;
    if(relop_val == "<")
        return relop_type::LT;
    if(relop_val == ">=")
        return relop_type::GE;
    if(relop_val == "<=")
        return relop_type::LE;
    if(relop_val == "==")
        return relop_type::EQ;
    if(relop_val == "!=")
        return relop_type::NQ;
}

void code_generator::neg( string type )
{
	if( type == "int" ){
		add_inst_optimized("ineg");
	}
	else if( type == "float" ){
		add_inst_optimized("fneg");
	}
}

void code_generator::add_inst_optimized( string line )
{
    if( line == "fneg" || line == "ineg" ){
        if( lines[lines.size()-1] == line ){
            lines.pop_back();
        }
        else{
            add_instruction(line);
        }
    }
    else{
        add_instruction(line);
    }
}

string code_generator::arth_op( op_type op , string type )
{
	string ret = "f";

	if( type == "int" )
		ret = "i" ;

	if( op == op_type::ADD )
		ret += "add" ;
	else if( op == op_type::SUB )
		ret += "sub" ;
	else if( op == op_type::MUL )
		ret += "mul" ;
	else if( op == op_type::DIV )
		ret += "div";

	return ret ;
}

void code_generator::insert_id_to_stack( string id_lexeme )
{
	entry* e = sym_table.get_entry(id_lexeme);

	if( e->get_type() == "int" ){
		add_instruction("iload "+int_to_string(e->get_symbol_pointer()));
	}
	else if( e->get_type() == "float" ){
		add_instruction("fload "+int_to_string(e->get_symbol_pointer()));
	}
}

void code_generator::insert_int_const_to_stack( int val )
{
	add_instruction("ldc "+int_to_string(val));
}

void code_generator::insert_float_const_to_stack( float val )
{
     add_instruction("ldc "+float_to_string(val));
}

void code_generator::assign_symbol( string id , string type_of_expression )
{
    entry* e = sym_table.get_entry(id);
	int pointer = e->get_symbol_pointer();
	string type = e->get_type();

	if( type != type_of_expression ){
		if( type == "int" ){
			add_instruction("f2i");
		}
		else if( type == "float" ){
			add_instruction("i2f");
		}
	}

	if(type == "float")
        add_instruction("fstore "+int_to_string(pointer));
	else if(type == "int")
        add_instruction("istore "+int_to_string(pointer));
}

void code_generator::declare_symbol( int table_pointer , string type )
{
   if( type == "int" ){ /* set value 0 by default */
     add_instruction("ldc 0");
	 add_instruction("istore "+int_to_string(table_pointer));
   }
   else if( type == "float" ){
     add_instruction("ldc 0.0");
	 add_instruction("fstore "+int_to_string(table_pointer));
   }
}

string code_generator::max_type(string type1, string type2)
{
  if(type1 == type2)
    return type1;
  else if(type1 == "float" || type2 == "float")
    return "float";
  return "int";
}

void code_generator::print_start_template(ofstream& output)
    {
      output << ".class public bytecodeOutput\n"
        << "\t.super java/lang/Object\n"
        << "\t; standard initializer\n"
        << "\t.method public <init>()V\n"
        << "\t\taload_0\n"
        << "\t\tinvokenonvirtual java/lang/Object/<init>()V\n"
        << "\t\treturn\n"
        << "\t.end method\n"
        << "\n"
        << "\t.method public static main([Ljava/lang/String;)V\n"
        << "\t\t; set limits used by this method\n"
        << "\t\t.limit locals 50\n"
        << "\t\t.limit stack 50" << endl;
    }

void code_generator::print_end_template(ofstream& output)
{
    output << "\t\treturn\n"
        << "\t.end method" << endl;
}

void code_generator::printing_template(ofstream& output, int index, string type)
{
    output << "\t\t\tgetstatic java/lang/System/out Ljava/io/PrintStream;\n"
        << "\t\t\tldc " << "\"" << index << ": \"\n"
        << "\t\t\tinvokevirtual java/io/PrintStream/print(Ljava/lang/String;)V\n";

    output << "\t\t\tgetstatic java/lang/System/out Ljava/io/PrintStream;\n";
    if(type == "int")
    {
        output << "\t\t\tiload " << index << "\n" <<
                "\t\t\tinvokevirtual java/io/PrintStream/println(I)V" << endl;
    }
    if(type == "float")
    {
        output << "\t\t\tfload " << index << "\n" <<
                "\t\t\tinvokevirtual java/io/PrintStream/println(F)V" << endl;
    }
}

int code_generator::string_to_int(string str, int index)
{
    int ret= 0;
    for(int i= index; i< str.size(); i++)
    {
        ret*= 10;
        ret+= (str[i]-'0');
    }
    return ret;
}

string code_generator::solve_casting( string type_1 , string type_2 )
{
	if( type_1 == type_2 )return type_1 ;
	if( type_1 == "int" ){
		/* Some kind of hard problem */
		add_instruction("fstore "+int_to_string(sym_table.get_table_counter()+1));
		add_instruction("i2f");
		add_instruction("fload "+int_to_string(sym_table.get_table_counter()+1));
	}
	else if( type_1 == "float" ){
		add_instruction("i2f");
	}
	return "float" ;/* even though it might seems ridiculous , but needed for code expansion */
}

//mips simulator in c++
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <cmath>
#include <map>
#include <vector>
#include "func.h"
using namespace std;
#define maxn 32 * 1024 * 1024
#define ln 4
#define pn 16

class num_of_var_error {};

int GP, correct(0), total(0);
char store[maxn];
map<string, int> Label;
map<string, int> Token;
map<string, int> reg;
map<int, int> branch;

struct branch_inst
{
	unsigned int history = 10;
	unsigned int predictLog[pn] = {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10 };
};

vector<branch_inst> repo;

int Register[35];
int &gp = Register[28];
int &sp = Register[29];
int &fp = Register[30];
int &ra = Register[31];
int &hi = Register[32];
int &lo = Register[33];
int &PC = Register[34];

bool interrupt = false;
bool return_value = false;
int return_value_is = 0;

int reliance[5][5];   //represent the reliance relationship between instructions, 0 -> nonerelative, 1 -> relative
int status[5];         //represent the status of each functional part, 0 -> empty, 1 -> full, -1 -> halt
int rR[3];
int wR[2];

class instruction_base
{
public:
	//order of operation
	int op;
	//other data
	int imme, shamt;
	//using lo and hi register
	char num;
	//register
	char R[3];

	instruction_base() :op(0), imme(0), shamt(0), num(0) 
	{
		R[0] = 0;
		R[1] = 0;
		R[2] = 0;
	}

};

class instruction_id :public instruction_base
{
public:
	int NPC;

	instruction_id() :NPC(0) {}
};

class instruction_ex :public instruction_id
{
public:
	int R1, S2;

	instruction_ex() :R1(0), S2(0) {}
};

class instruction_mem :public instruction_ex
{
public:
	int res, res_b, Addr;
	
	instruction_mem() :res(0), res_b(0), Addr(0) {}
};

class instruction_wb :public instruction_mem
{
public:
	int rep;

	instruction_wb():rep(0){}
};

instruction_base null_inst_base;
instruction_id null_inst_id;
instruction_ex null_inst_ex;
instruction_mem null_inst_mem;
instruction_wb null_inst_wb;

instruction_base tmp_inst_base;
instruction_id tmp_inst_id;
instruction_ex tmp_inst_ex;
instruction_mem tmp_inst_mem;
instruction_wb tmp_inst_wb;

instruction_id cc_btoi(instruction_base Base, int npc)
{
	instruction_id tmp;
	
	tmp.NPC = npc;
	tmp.imme = Base.imme;
	tmp.num = Base.num;
	tmp.op = Base.op;
	tmp.shamt = Base.shamt;
	for (int i = 0; i < 3; ++i)
		tmp.R[i] = Base.R[i];
	
	return tmp;
}

instruction_ex cc_itoe(instruction_id Base, int r1, int s2)
{
	instruction_ex tmp;

	tmp.NPC = Base.NPC;
	tmp.imme = Base.imme;
	tmp.num = Base.num;
	tmp.op = Base.op;
	tmp.shamt = Base.shamt;
	tmp.R1 = r1;
	tmp.S2 = s2;
	for (int i = 0; i < 3; ++i)
		tmp.R[i] = Base.R[i];
	
	return tmp;
}

instruction_mem cc_etom(instruction_ex Base, int res, int res_back, int addr)
{
	instruction_mem tmp;
	tmp.res = res;
	tmp.res_b = res_back;
	tmp.NPC = Base.NPC;
	tmp.imme = Base.imme;
	tmp.num = Base.num;
	tmp.op = Base.op;
	tmp.shamt = Base.shamt;
	tmp.R1 = Base.R1;
	tmp.S2 = Base.S2;
	tmp.Addr = addr;
	for (int i = 0; i < 3; ++i)
		tmp.R[i] = Base.R[i];
	
	return tmp;
}

instruction_wb cc_mtow(instruction_mem Base, int rep)
{
	instruction_wb tmp;
	tmp.NPC = Base.NPC;
	tmp.imme = Base.imme;
	tmp.num = Base.num;
	tmp.op = Base.op;
	tmp.shamt = Base.shamt;
	tmp.R1 = Base.R1;
	tmp.S2 = Base.S2;
	tmp.Addr = Base.Addr;
	tmp.res = Base.res;
	tmp.res_b = Base.res_b;
	tmp.rep = rep;
	for (int i = 0; i < 3; ++i)
		tmp.R[i] = Base.R[i];

	return tmp;
}
/*
void instruction_store(int &curGP, const instruction_base &inst)
{
	string immestr = c_itow(inst.imme);
	string opstr = c_itow(inst.op);
	string shamtstr = c_itow(inst.shamt);
}
*/
//convert function
int c_wtoi(const string &str)         
{
	int c = (1 << 8) - 1;
	int t3 = ((int)str[0] & c) << 24;
	int t2 = ((int)str[1] & c) << 16;
	int t1 = ((int)str[2] & c) << 8;
	int t0 = ((int)str[3] & c);
	return t3 + t2 + t1 + t0;
}

int c_htoi(const string &str)				 
{
	int c = (1 << 8) - 1;
	int t1 = ((int)str[0] & c) << 8;
	int t0 = ((int)str[1] & c);
	return t1 + t0;
}

int c_btoi(const char &c)         
{
	int t = (1 << 8) - 1;
	int t0 = ((int)c & t);
	return t0;
}

string c_itow(int i)				  
{
	char tmp[4];
	string str = "";
	tmp[0] = i >> 24;
	tmp[1] = i >> 16;
	tmp[2] = i >> 8;
	tmp[3] = i;
	for (int i = 0; i < 4; ++i)
		str += tmp[i];
	return str;
}

string c_itoh(int i)
{
	char tmp[2];
	string str = "";
	tmp[0] = i >> 8;
	tmp[1] = i;
	for (int i = 0; i < 2; ++i)
		str += tmp[i];
	return str;
}

char c_itob(int i)
{
	char tmp = i;
	return tmp;
}

void varibleini()
{
	GP = 160000;
	ra = 0;
	sp = fp = maxn - 1;
}


void prepare(const string &f)
{
	int type = 0;				 //record the type of current process: 0 - undecided, 1 - data, 2 - text


	fstream fop;
	fop.open(f.c_str());

	string buffer = "";			 //store the current line
	string label_name = "";		 //store the label
	string instruction = "";	 //store the instruction

	while (getline(fop, buffer, '\n'))
	{
		int i = 0;
		//cout << buffer << endl;
		//deal with label
		if (buffer[0] == '_')
		{
			++i;
			//cout << "Label" << endl;
			while (buffer[i] != ':' && buffer[i] != '\0')
				label_name += buffer[i++];
			if (buffer[i] == ':')
			{
				if (type == 1)
				{
					 Label.insert(pair<string, int>(label_name, GP));
					 //cout << label_name << ":" << GP << endl;
				}
				else {
					Label.insert(pair<string, int>(label_name, ra));
					//cout << label_name << ":" << ra << endl;
				}
			}
		}
		else if (buffer[0] == 'm')
		{
			//cout << "Main" << endl;
			while (buffer[i] != ':' && buffer[i] != '\0')
				label_name += buffer[i++];
			Label.insert(pair<string, int>(label_name, ra));
			PC = ra;
		}
		else
		{
			if (buffer[i] == '\t')
				++i;
			if (buffer[i] == '\0') continue;
			if (buffer[i] == '.')
			{
				++i;
				int ope = 0;
				while (buffer[i] != ' ' && buffer[i] != '\0')
					ope += buffer[i++] - '`';
				switch (ope)
				{
				case(26): { type = 1; /*cout << "Data" << endl;*/ break; }		//case .data
				case(69): { type = 2; /*cout << "Text" << endl;*/ break; }		//case .text
				case(27): {
					stringstream half;
					int ans;
					string tmp;
					while (buffer[i] != '\0' && buffer[i] != '#')
					{
						if (buffer[i] == ' ') { ++i; continue; }
						tmp += buffer[i++];
					}
					half << tmp;
					half >> ans;
					
					tmp = c_itoh(ans);
					store[GP++] = tmp[0];
					store[GP++] = tmp[1];
					tmp = "";
					//cout << "Half" << endl;
					break;
				}
				case(60): {
					stringstream word;
					int ans;
					string tmp;
					while (buffer[i] != '\0' && buffer[i] != '#')
					{
						if (buffer[i] == ' ') { ++i; continue; }
						tmp += buffer[i++];
					}
					//cout << "Word" << endl;
					word << tmp;
					word >> ans;
					tmp = c_itow(ans);
					for (int j = 0; j < 4; ++j)
						store[GP++] = tmp[j];
					tmp = "";
					break;
				}
				case(52): {
					char c;
					{
						if (buffer[i] == ' ') { ++i; continue; }
						c = buffer[i++];
					}
					//cout << "Byte" << endl;
					store[GP++] = c;
					break;
				}
				case(44): {
					stringstream space;
					string tmp;
					int ans;
					while (buffer[i] != '\0' && buffer[i] != '#')
					{
						if (buffer[i] == ' ') { ++i; continue; }
						tmp += buffer[i++];
					}
					space << tmp;
					space >> ans;
					GP += ans;
					//cout << "Space" << endl;
					break;
				}
				case(41): case(67): {
					string tmp = "";
					i += 2;
					while (buffer[i] != '"' && buffer[i] != '#' && buffer[i] != '\0')
					{
						if (buffer[i] == '\\')
						{
							switch (buffer[i + 1] - 'a')
							{
							case(0): { tmp += '\a'; break; }
							case(1): { tmp += '\b'; break; }
							case(5): { tmp += '\f'; break; }
							case(13): { tmp += '\n'; break; }
							case(17): { tmp += '\r'; break; }
							case(19): { tmp += '\t'; break; }
							case(21): { tmp += '\v'; break; }
							default: {
								tmp += buffer[i + 1];
							}
							}
							i += 2;
						}
						else tmp += buffer[i++];
					}
					//memcpy(&store[GP], &tmp, sizeof(char) * tmp.size());
					for (int i = 0; i < (int)tmp.length(); ++i) {
						store[GP + i] = tmp[i];
					}
					GP += tmp.length();
					if (ope == 67)
					{
						store[GP] = '\0';
						GP += 1;
						//cout << "Asciiz" << endl;
					}
					//else cout << "Ascii" << endl;
					break;
				}
				case(43): {
					int n;
					string tmp;
					stringstream align;
					while (buffer[i] != '\0' && buffer[i] != '#')
					{
						if (buffer[i] == ' ') { ++i; continue; }
						tmp += buffer[i++];
					}
					align << tmp;
					align >> n;
					int s = (int)pow(2, n);
					if (GP % s) GP += s - GP % s;
					//cout << "Align" << endl;
					break;
				}
				}
			}
			else if (buffer[i] == '_')
			{
				++i;
				//cout << "Label" << endl;
				while (buffer[i] != ':' && buffer[i] != '\0')
					label_name += buffer[i++];
				if (buffer[i] == ':')
				{
					if (type == 1) {
						Label.insert(pair<string, int>(label_name, GP));
						//cout << label_name << ":" << GP << endl;
					}
					else {
						Label.insert(pair<string, int>(label_name, ra));
						//cout << label_name << ":" << ra << endl;
					}
				}
			}
			else ra += 16;
		}
		buffer = "";
		label_name = "";
		instruction = "";
	}
	fop.close();
}
void preprocess(const string &f)
{
	prepare(f);
	ra = 0;


	fstream fop;
	fop.open(f.c_str());

	string buffer = "";			 //store the current line
	string label_name = "";		 //store the label
	string instruction = "";	 //store the instruction

	while (getline(fop, buffer, '\n'))
	{
		int i = 0;
		//cout << buffer << endl;
		//deal with label
		if (buffer[0] == '_') continue;
		else if (buffer[0] == 'm') continue;
		else
		{
			if (buffer[i] == '\t') ++i;
			if (buffer[i] == '\0') continue;
			if (buffer[i] == '_') continue;
			if (buffer[i] == '.') continue;
			else
			{
				string tmp = "";
				while (buffer[i] != ' ' && buffer[i] != '\0')
					tmp += buffer[i++];
				instruction_base tmpc;
				tmpc.op = Token[tmp];
				int j = 0;

				while (buffer[i] != '\0' && buffer[i] != '#')
				{
					++i;
					tmp = "";
					while (buffer[i] == ' ')
						++i;
					if (buffer[i] == '\0')
						break;
					if (buffer[i] == '$')
					{
						++i;
						while (buffer[i] != ' ' && buffer[i] != ',' && buffer[i] != '\0' && buffer[i] != '#')
							tmp += buffer[i++];
						tmpc.R[j++] = c_itob(reg[tmp]);
						++tmpc.num;
					}
					else if ((buffer[i] - '0' <= 9 && buffer[i] - '0' >= 0) || (buffer[i] == '-'))
					{
						tmp += buffer[i++];
						while (buffer[i] - '0' <= 9 && buffer[i] - '0' >= 0)
							tmp += buffer[i++];
						stringstream tmpss;
						tmpss << tmp;
						if (buffer[i] == '(')
						{
							tmpss >> tmpc.shamt;
							tmp = "";

							i += 2;
							while (buffer[i] != ')')
								tmp += buffer[i++];
							tmpc.R[j++] = c_itob(reg[tmp]);
							++tmpc.num;
						}
						else
						{
							tmpss >> tmpc.imme;
							++tmpc.num;
						}
					}
					else if (buffer[i] == '_')
					{
						++i;
						tmp = "";
						while (buffer[i] != '\0' && buffer[i] != '#' && buffer[i] != ' ')
							tmp += buffer[i++];
						tmpc.shamt = Label[tmp];
					}
				}
				/*if (tmpc.op == 7 || tmpc.op == 8)
				{
					for (int k = 1; k < j; ++k)
						if (tmpc.R[k] == 0)  tmpc.shamt += 1;
				}*/
				if (tmpc.op <= 36 && tmpc.op >= 25)
				{
					repo.push_back(branch_inst());
					branch.insert(pair<int, int>(ra + 16, repo.size()));
				}
				memcpy(&store[ra], &tmpc, 16);
				//cout << tmpc.op << endl;
				ra += 16;
			}
		}

		buffer = "";
		label_name = "";
		instruction = "";

	}
	fop.close();
}

//From HERE begin branch predictation
//lastest editted on 7/5

//
class Log
{
public:
	int pre; //0 - no prediction, -1 - predicted false, 1 - predicted true
	int tar_PC;
	int raw_PC;

	Log() {}
	Log(int p, int t, int r) :pre(p), tar_PC(t), raw_PC(r) {}
};
Log prelog;

//function
int tmp;
int temp;
bool branch_predict(const int op, const int i)
{
	if (op >= 25 && op <= 36)
	{
		temp = branch[i];
		if (temp != 0) temp -= 1;
		else cout << "not found" << endl;
		tmp = (repo[temp].predictLog[(repo[temp].history % pn)] >> 3) % 2;
		if (tmp) return true;
		else return false;
	}
	else return true;
}
//To HERE end branch predictation

instruction_id IF()
{
	if (interrupt) return null_inst_id;

	instruction_base inst;
	int NPC;
	if (prelog.pre != 1)
	{
		memcpy(&inst, &store[PC], 16);
		PC += 16;
		NPC = PC;
	}
	else
	{
		memcpy(&inst, &store[prelog.tar_PC], 16);
		prelog.tar_PC += 16;
		NPC = prelog.tar_PC;
	}
	//cout << "now processing PC=" << PC << ' ' << "GP=" << GP << '\n';
	//cout << "inst.op=" << inst.op << endl;
	

	return cc_btoi(inst, NPC);
	//ID(inst, NPC);
}

instruction_ex ID(instruction_id inst)
{
	if(interrupt) return null_inst_ex;
	if (inst.op == 0) return null_inst_ex;
	
	int rsrc1(0), src2(0);

	//for (int i = 0; i < 35; ++i)
	//	cout << "#" << i << ' ' << Register[i] << ' ';
	//cout << endl;
	switch (inst.op)
	{
	case(1): case(2): case(3): case(4): case(5): case(6): 
	case(11): case(12): case(15): case(16): case(18): case(19):
	case(20): case(21): case(22): case(23):
	case(48): {
		rsrc1 = Register[c_btoi(inst.R[1])];
		src2 = (inst.imme ? inst.imme : Register[c_btoi(inst.R[2])]);
		break;
	}
	case(7): case(8): {
		if (inst.num == 3)
		{
			if (inst.imme)
			{
				rsrc1 = Register[c_btoi(inst.R[1])];
				src2 = inst.imme;
			}
			else
			{
				rsrc1 = Register[c_btoi(inst.R[1])];
				src2 = Register[c_btoi(inst.R[2])];
			}
		}
		else if (inst.num == 2)
		{
			if (inst.imme)
			{
				rsrc1 = Register[c_btoi(inst.R[0])];
				src2 = inst.imme;
			}
			else
			{
				rsrc1 = Register[c_btoi(inst.R[0])];
				src2 = Register[c_btoi(inst.R[1])];
			}
		}
		else throw num_of_var_error();
		break;
	}
	case(9): case(10): {
		if (inst.num == 3)
		{
			if (inst.imme)
			{
				rsrc1 = Register[c_btoi(inst.R[1])];
				src2 = inst.imme;
			}
			else
			{
				rsrc1 = Register[c_btoi(inst.R[1])];
				src2 = Register[c_btoi(inst.R[2])];
			}
		}
		else if (inst.num == 2)
		{
			if (inst.imme)
			{
				rsrc1 = Register[c_btoi(inst.R[0])];
				src2 = inst.imme;
			}
			else
			{
				rsrc1 = Register[c_btoi(inst.R[0])];
				src2 = Register[c_btoi(inst.R[1])];
			}
		}
		else throw num_of_var_error();
		break;
	}
	case(13): case(14): {
		rsrc1 = Register[c_btoi(inst.R[1])];
		break;
	}
	case(25): case(26): case(27): case(28): case(29): case(30): {
		rsrc1 = Register[c_btoi(inst.R[0])];
		if (inst.imme) src2 = inst.imme;
		else src2 = Register[c_btoi(inst.R[1])];
		break;
	}
	case(31): case(32): case(33): case(35): case(36):
	case(37): case(38): case(39): case(40):
	case(41): case(42): case(43): case(44): case(45): case(46): case(47): {
		rsrc1 = Register[c_btoi(inst.R[0])];
		src2 = Register[c_btoi(inst.R[1])];
		break;
	}
	case(49): {
		rsrc1 = hi;
		break;
	}
	case(50): {
		rsrc1 = lo;
		break;
	}
	case(52): {
		switch (inst.imme = Register[2])
		{
		case(1): case(4): case(8): case(9): case(17): {
			rsrc1 = Register[4];
			src2 = Register[5];
			break;
		}
		case(10): {
			interrupt = true;
			return_value = false;
			break;
		}
		case(11): {
			interrupt = true;
			return_value = true;
			return_value_is = Register[4];
			break;
		}
		default: {break; }
		}
	}
	default: {break; }
	}
	
	return cc_itoe(inst, rsrc1, src2);
	//EX(inst, NPC, rsrc1, src2);
}

instruction_mem EX(instruction_ex inst)
{
	if (interrupt) return null_inst_mem;
	if (inst.op == 0) return null_inst_mem;

	const int R1 = inst.R1;
	const int S2 = inst.S2;

	int result = 0;
	int result_backup = 0;
	int WBaddr = 0;

	switch (inst.op)
	{
	case(1): case(2): case(3):{
		result = R1 + S2;
		WBaddr = c_btoi(inst.R[0]);
		break;
	}
	case(4):case(5):case(6): {
		result = R1 - S2;
		WBaddr = c_btoi(inst.R[0]);
		break;
	}
	case(7): case(8): {
		if (inst.num == 3) 
		{
			result = R1 * S2;
			WBaddr = c_btoi(inst.R[0]);
		}
		else 
		{
			long long r = R1 * S2;
			int c = (1 << 8) - 1;
			result = r & c;
			result = r >> 32;
			WBaddr = -1;
		}
		break;
	}
	case(9): case(10): {
		result = R1 / S2;
		result_backup = R1 % S2;
		if (inst.num == 3) WBaddr = c_btoi(inst.R[0]);
		else WBaddr = -1;
		break;
	}
	case(11): case(12): {
		result = (int)(R1 ^ S2);
		WBaddr = c_btoi(inst.R[0]);
		break;
	}
	case(13):case(14): {
		result = -R1;
		WBaddr = c_btoi(inst.R[0]);
		break;
	}
	case(15):case(16): {
		result = R1 % S2;
		WBaddr = c_btoi(inst.R[0]);
		break;
	}
	case(17): {
		result = inst.imme;
		WBaddr = c_btoi(inst.R[0]);
		break;
	}
	case(18): case(25): case(31): {
		result = (R1 == S2 ? 1 : 0);
		WBaddr = c_btoi(inst.R[0]);
		break;
	}
	case(19): case(27): case(33): {
		result = (R1 >= S2 ? 1 : 0);
		WBaddr = c_btoi(inst.R[0]);
		break;
	}
	case(20): case(29): case(35): {
		result = (R1 > S2 ? 1 : 0);
		WBaddr = c_btoi(inst.R[0]);
		break;
	}
	case(21): case(28): case(34): {
		result = (R1 <= S2 ? 1 : 0);
		WBaddr = c_btoi(inst.R[0]);
		break;
	}
	case(22): case(30): case(36): {
		result = (R1 < S2 ? 1 : 0);
		WBaddr = c_btoi(inst.R[0]);
		break;
	}
	case(23): case(26): case(32): {
		result = (R1 != S2 ? 1 : 0);
		WBaddr = c_btoi(inst.R[0]);
		break;
	}
	case(24): case(37): case(39): 
	case(38): case(40): {
		result = R1;
		WBaddr = -1;
		break;
	}
	case(41): case(42): case(43): case(44): {
		result = S2 + inst.shamt;
		WBaddr = c_btoi(inst.R[0]);
		break;
	}
	case(45): case(46): case(47): {
		WBaddr = S2 + inst.shamt;
		result = R1;
		break;
		
	}
	case(48): {
		result = R1;
		WBaddr = c_btoi(inst.R[0]);
		break;
	}
	case (49): {
		result = R1;
		WBaddr = c_btoi(inst.R[0]);
		break;
	}
	case (50): {
		result = R1;
		WBaddr = c_btoi(inst.R[0]);
		break;
	}
	case(51): {
		break;
	}
	case(52): {
		switch (inst.imme)
		{
		case(1): {
			cout << R1;
			break;
		}
		case(4): {
			char *c = &store[R1];
			while ((*c) != '\0')
			{
				cout << *c;
				++c;
			}
			break;
		}
		case(5): {
			//cout << "************************************need type in" << endl;
			cin >> result;
			break;
		}
		case(8):{
			string s;
			//cout << "************************************need type in" << endl;
			cin >> s;
			int k = 0;
			while (s[k] != '\0')
			{
				store[GP + k] = s[k];
				++k;
			}
			inst.shamt = GP;
			result = R1;
			result_backup = S2 - 1;
			GP += S2 - 1;
			break;
		}
		case(9): {
			result = GP;
			GP += R1;
			break;
		}
		case(10): {
			interrupt = true;
			return_value = false;
			break;
		}
		case(17): {
			interrupt = true;
			return_value = true;
			return_value_is = R1;
			break;
		}
		}	
	}
	default: {break; }
	}
	return cc_etom(inst, result, result_backup, WBaddr);
	//MEM(inst, NPC, result, result_backup, WBaddr);
}

instruction_wb MEM(instruction_mem inst)
{
	if (interrupt) return null_inst_wb;
	if (inst.op == 0) return null_inst_wb;

	int vres = 0;
	const int res = inst.res;
	const int res_b = inst.res_b;
	const int Addr = inst.Addr;
	string tmp = "";

	switch (inst.op)
	{
	case(41): {          //---->la
		vres = res;
		break;
	}
	case(42): {
		vres = c_btoi(store[res]);
		break;
	}
	case(43): {
		for (int i = 0; i < 2; ++i)
			tmp += store[res + i];
		vres = c_htoi(tmp);
		break;
	}
	case(44): {
		for (int i = 0; i < 4; ++i)
			tmp += store[res + i];
		vres = c_wtoi(tmp);
		break;
	}
	case(45): {
		store[Addr] = c_itob(res);
		break;
	}
	case(46): {
		tmp = c_itoh(res);
		for (int i = 0; i < 2; ++i)
			store[Addr + i] = tmp[i];
		break;
	}
	case(47): {
		tmp = c_itow(res);
		for (int i = 0; i < 4; ++i)
			store[Addr + i] = tmp[i];
		break;
	}
	case(52): {
		if (inst.imme == 8)
			for(int k = 0; k < res_b; ++k)
				store[res + k] = store[inst.shamt + k];
		break;
	}
	default: {break; }
	}
	return cc_mtow(inst, vres);
	//WB(inst, NPC, res, res_b, Addr, vres);
}

bool WB(instruction_wb inst)
{
	if (inst.op == 0) return false;

	const int Addr = inst.Addr;
	const int res = inst.res;
	const int res_b = inst.res_b;
	const int rep = inst.rep;
	//const int NPC = inst.NPC;
	//cout << "operation:" << inst.op << '\n';
	//cout << "PC: " << NPC - 16 << endl;
	switch (inst.op)
	{
	case(1): case(2): case(3):
	case(4): case(5): case(6):
	case(11): case(12):
	case(13): case(14):
	case(15): case(16):
	case(17):
	case(18): case(19): case(20): case(21): case(22): case(23): {
		Register[Addr] = res;
		break;
	}
	case(7): case(8): case(9): case(10):
	{
		if (inst.num == 3) Register[Addr] = res;
		else lo = res, hi = res_b;
		break;
	}
	case(25): case(26): case(27): case(28):
	case(29): case(30): case(31): case(32):
	case(33): case(34): case(35): case(36): {
		break;
	}
	case(24): case(37): {
		break;
	}
	case(39): {
		break;
	}
	case(38): {
		break;
	}
	case(40): {
		break;
	}
	case(41): {
		Register[Addr] = rep;
		break;
	}
	case(42): {
		Register[Addr] = rep;
		break;
	}
	case(43): {
		Register[Addr] = rep;
		break;
	}
	case(44): {
		Register[Addr] = rep;
		break;
	}
	case(48): case(49): case(50): {
		Register[Addr] = res;
		break;
	}
	case(52): {
		switch (inst.imme)
		{
		case(5): case(9): {
			Register[2] = res;
			break;
		}
		default: {break; }
		}
	}
	}
	//for (int i = 0; i < 34; ++i)
		//cout << "#" << i << ' ' << Register[i] << ' ';
	//cout << endl;
	return true;
}

void Token_ini()
{
	Token.insert(pair<string, int>("add",1));
	Token.insert(pair<string, int>("addu", 2));
	Token.insert(pair<string, int>("addiu", 3));
	Token.insert(pair<string, int>("sub", 4));
	Token.insert(pair<string, int>("subu", 5));
	Token.insert(pair<string, int>("subiu", 6));
	Token.insert(pair<string, int>("mul", 7));
	Token.insert(pair<string, int>("mulu", 8));
	Token.insert(pair<string, int>("div", 9));
	Token.insert(pair<string, int>("divu", 10));
	Token.insert(pair<string, int>("xor", 11));
	Token.insert(pair<string, int>("xoru", 12));
	Token.insert(pair<string, int>("neg", 13));
	Token.insert(pair<string, int>("negu", 14));
	Token.insert(pair<string, int>("rem", 15));
	Token.insert(pair<string, int>("remu", 16));
	Token.insert(pair<string, int>("li", 17));
	Token.insert(pair<string, int>("seq", 18));
	Token.insert(pair<string, int>("sge", 19));
	Token.insert(pair<string, int>("sgt", 20));
	Token.insert(pair<string, int>("sle", 21));
	Token.insert(pair<string, int>("slt", 22));
	Token.insert(pair<string, int>("sne", 23));
	Token.insert(pair<string, int>("b", 24));
	Token.insert(pair<string, int>("beq", 25));
	Token.insert(pair<string, int>("bne", 26));
	Token.insert(pair<string, int>("bge", 27));
	Token.insert(pair<string, int>("ble", 28));
	Token.insert(pair<string, int>("bgt", 29));
	Token.insert(pair<string, int>("blt", 30));
	Token.insert(pair<string, int>("beqz", 31));
	Token.insert(pair<string, int>("bnez", 32));
	Token.insert(pair<string, int>("bgez", 33));
	Token.insert(pair<string, int>("blez", 34));
	Token.insert(pair<string, int>("bgtz", 35));
	Token.insert(pair<string, int>("bltz", 36));
	Token.insert(pair<string, int>("j", 37));
	Token.insert(pair<string, int>("jr", 38));
	Token.insert(pair<string, int>("jal", 39));
	Token.insert(pair<string, int>("jalr", 40));
	Token.insert(pair<string, int>("la", 41));
	Token.insert(pair<string, int>("lb", 42));
	Token.insert(pair<string, int>("lh", 43));
	Token.insert(pair<string, int>("lw", 44));
	Token.insert(pair<string, int>("sb", 45));
	Token.insert(pair<string, int>("sh", 46));
	Token.insert(pair<string, int>("sw", 47));
	Token.insert(pair<string, int>("move", 48));
	Token.insert(pair<string, int>("mfhi", 49));
	Token.insert(pair<string, int>("mflo", 50));
	Token.insert(pair<string, int>("nop", 51));
	Token.insert(pair<string, int>("syscall", 52));
}

void reg_ini()
{
	reg.insert(pair<string, int>("zero", 0)); reg.insert(pair<string, int>("0", 0));
	reg.insert(pair<string, int>("at", 1)); reg.insert(pair<string, int>("1", 1));
	reg.insert(pair<string, int>("v0", 2)); reg.insert(pair<string, int>("2", 2));
	reg.insert(pair<string, int>("v1", 3)); reg.insert(pair<string, int>("3", 3));
	reg.insert(pair<string, int>("a0", 4)); reg.insert(pair<string, int>("4", 4));
	reg.insert(pair<string, int>("a1", 5)); reg.insert(pair<string, int>("5", 5));
	reg.insert(pair<string, int>("a2", 6)); reg.insert(pair<string, int>("6", 6));
	reg.insert(pair<string, int>("a3", 7)); reg.insert(pair<string, int>("7", 7));
	reg.insert(pair<string, int>("t0", 8)); reg.insert(pair<string, int>("8", 8));
	reg.insert(pair<string, int>("t1", 9)); reg.insert(pair<string, int>("9", 9));
	reg.insert(pair<string, int>("t2", 10)); reg.insert(pair<string, int>("10", 10));
	reg.insert(pair<string, int>("t3", 11)); reg.insert(pair<string, int>("11", 11));
	reg.insert(pair<string, int>("t4", 12)); reg.insert(pair<string, int>("12", 12));
	reg.insert(pair<string, int>("t5", 13)); reg.insert(pair<string, int>("13", 13));
	reg.insert(pair<string, int>("t6", 14)); reg.insert(pair<string, int>("14", 14));
	reg.insert(pair<string, int>("t7", 15)); reg.insert(pair<string, int>("15", 15));
	reg.insert(pair<string, int>("s0", 16)); reg.insert(pair<string, int>("16", 16));
	reg.insert(pair<string, int>("s1", 17)); reg.insert(pair<string, int>("17", 17));
	reg.insert(pair<string, int>("s2", 18)); reg.insert(pair<string, int>("18", 18));
	reg.insert(pair<string, int>("s3", 19)); reg.insert(pair<string, int>("19", 19));
	reg.insert(pair<string, int>("s4", 20)); reg.insert(pair<string, int>("20", 20));
	reg.insert(pair<string, int>("s5", 21)); reg.insert(pair<string, int>("21", 21));
	reg.insert(pair<string, int>("s6", 22)); reg.insert(pair<string, int>("22", 22));
	reg.insert(pair<string, int>("s7", 23)); reg.insert(pair<string, int>("23", 23));
	reg.insert(pair<string, int>("t8", 24)); reg.insert(pair<string, int>("24", 24));
	reg.insert(pair<string, int>("t9", 25)); reg.insert(pair<string, int>("25", 25));
	reg.insert(pair<string, int>("k0", 26)); reg.insert(pair<string, int>("26", 26));
	reg.insert(pair<string, int>("k1", 27)); reg.insert(pair<string, int>("27", 27));
	reg.insert(pair<string, int>("gp", 28)); reg.insert(pair<string, int>("28", 28));
	reg.insert(pair<string, int>("sp", 29)); reg.insert(pair<string, int>("29", 29));
	reg.insert(pair<string, int>("fp", 30)); reg.insert(pair<string, int>("30", 30));
	reg.insert(pair<string, int>("ra", 31)); reg.insert(pair<string, int>("31", 31));
	reg.insert(pair<string, int>("lo", 32)); reg.insert(pair<string, int>("32", 32));
	reg.insert(pair<string, int>("hi", 33)); reg.insert(pair<string, int>("33", 33));
	reg.insert(pair<string, int>("PC", 34)); reg.insert(pair<string, int>("34", 34));
}

void require_WB()
{
	for (int i = 0; i < 3; ++i)
		reliance[4][i] = 0;
	status[4] = 0;
}

void report_WB(int i)
{
	cout << "WB function failed with value:" << i << endl;
}

void Judge_WB()
{
	if (interrupt) return;

	if (status[4] == 1)
		WB(tmp_inst_wb) ? require_WB() : report_WB(1);
	else if (status[4] == 0) return;
	else report_WB(2);
}

void require_MEM()
{
	for (int i = 0; i < 5; ++i)
	{
		reliance[4][i] = reliance[3][i];
		reliance[3][i] = 0;
	}
	status[3] = 0;
	status[4] = 1;
}

void Judge_MEM()
{
	if (interrupt) return;

	if (status[3] != 0)
	{
		tmp_inst_wb = MEM(tmp_inst_mem);
		require_MEM();
	}
}

void require_EX()
{
	for (int i = 0; i < 5; ++i)
	{
		reliance[3][i] = reliance[2][i];
		reliance[2][i] = 0;
	}
	status[2] = 0;
	status[3] = 1;
}

void Judge_EX()
{
	if (interrupt) return;

	if (status[2] != 0)
	{
		tmp_inst_mem = EX(tmp_inst_ex);
		require_EX();
	}
}

void require_ID()
{
	for (int i = 0; i < 5; ++i)
	{
		reliance[2][i] = reliance[1][i];
		reliance[1][i] = 0;
	}
	status[1] = 0;
	status[2] = 1;
}

void Judge_ID()
{
	if (interrupt) return;

	if (status[1] == 1)
	{
		tmp_inst_ex = ID(tmp_inst_id);
		require_ID();
	}
	else if (status[1] == -1)
	{
		if (reliance[2][1] == 0 && reliance[3][1] == 0 && reliance[4][1] == 0)
		{
			tmp_inst_ex = ID(tmp_inst_id);
			require_ID();
		}
	}
}

void get_relative_register(instruction_base *raw_ptr)
{
	rR[0] = 0, rR[1] = 0, rR[2] = 0;
	int num = raw_ptr->num;
	char R[3];
	for (int i = 0; i < 3; ++i)
		R[i] = raw_ptr->R[i];
	int op = raw_ptr->op;
	
	switch (op)
	{
	case(7):case(8):case(9):case(10): {
		if (num == 3)
		{
			rR[0] = c_btoi(R[1]);
			rR[1] = c_btoi(R[2]);
		}
		else
		{
			rR[0] = c_btoi(R[0]);
			rR[1] = c_btoi(R[1]);
		}
		break;
	}
	case(25):case(26):case(27):case(28):case(29):case(30):
	case(31):case(32):case(33):case(34):case(35):case(36):
	case(38):case(40):case(45):case(46):case(47): {
		rR[0] = c_btoi(R[0]);
		rR[1] = c_btoi(R[1]);
		break;
	}
	case(49): {
		rR[0] = 33;
		break;
	}
	case(50): {
		rR[0] = 32;
		break;
	}
	case(52): {
		rR[0] = 2, rR[1] = 3; rR[2] = 4;
		break;
	}
	default: {
		rR[0] = c_btoi(R[1]);
		rR[1] = c_btoi(R[2]);
		break;
	}
	}
}

void get_wb_register(instruction_base *raw_ptr)
{
	wR[0] = 0, wR[1] = 0;
	int num = raw_ptr->num;
	char R[3];
	for (int i = 0; i < 3; ++i)
		R[i] = raw_ptr->R[i];
	int op = raw_ptr->op;

	switch (op)
	{
	case(7):case(8):case(9):case(10): {
		if (num == 2)
			wR[0] = 32, wR[1] = 33;
		else wR[0] = c_btoi(R[0]);
		break;
	}
	case(45):case(46):case(47): {
		break;
	}
	case(52): {
		wR[0] = 2;
		break;
	}
	default: {
		wR[0]= c_btoi(R[0]);
		break;
	} 
	}
}

bool is_data_hazard(instruction_base *instptr, instruction_base *raw_ptr)
{
	get_relative_register(raw_ptr);
	get_wb_register(instptr);

	for(int i = 0; i < 3; ++i)
		for (int j = 0; j < 2; ++j)
		{
			if (rR[i] == 0 || wR[j] == 0) continue;
			if (rR[i] == wR[j]) return true;
		}
	return false;
}

void deal_with_control_hazard()
{
	if (tmp_inst_ex.op <= 23 || tmp_inst_ex.op >= 41) return;
	if (status[2] != 0)
	{
		++total;
		//cout << "operation:" << tmp_inst_ex.op;
		prelog.pre = (branch_predict(tmp_inst_ex.op,PC) ? 1 : -1);
		//cout << "    predict result:" << prelog.pre << endl;
		prelog.raw_PC = PC;
		if(tmp_inst_ex.op == 38 || tmp_inst_ex.op == 40) prelog.tar_PC = tmp_inst_ex.R1;
		else prelog.tar_PC = tmp_inst_ex.shamt;
	}
	else prelog.pre = 0;
}

void analysis()
{
	//Data Hazard
	if (status[2] != 0 && is_data_hazard(&tmp_inst_ex, &tmp_inst_id))
	{
		status[1] = -1;
		reliance[2][1] = 1;
	}
	else if (status[3] != 0 && is_data_hazard(&tmp_inst_mem, &tmp_inst_id))
	{
		status[1] = -1;
		reliance[3][1] = 1;
	}
	else if (status[4] != 0 && is_data_hazard(&tmp_inst_wb, &tmp_inst_id))
	{
		status[1] = -1;
		reliance[4][1] = 1;
	}
}

void Judge_IF()
{
	if (interrupt) return;
	
	if (status[1] == 0)
	{
		tmp_inst_id = IF();
		status[1] = 1;
		analysis();
	}
}

void Rejudge()
{
	if (status[3] == 1)
	{
		const int op = tmp_inst_mem.op;
		switch (op)
		{
		case(25):case(26):case(27):case(28):case(29):case(30):
		case(31):case(32):case(33):case(34):case(35):case(36): {
			int ptr = branch[prelog.raw_PC] - 1;
			//if (ptr == 0)cout << "Not found" << endl;
			if (tmp_inst_mem.res)
			{
				if (prelog.pre == 1) {
					PC = prelog.tar_PC, correct = correct + 1;
					//cout << "->predict right" << endl;
				}
				else
				{
					status[1] = 0;
					PC = prelog.tar_PC;
					//cout << "->predict wrong" << endl;
				}
				repo[ptr].predictLog[repo[ptr].history % pn] <<= 1;
				repo[ptr].predictLog[repo[ptr].history % pn] += 1;
				repo[ptr].history <<= 1;
				repo[ptr].history += 1;
			}
			else
			{
				if (prelog.pre == 1)
				{
					status[1] = 0;
					//cout << "predict wrong" << endl;
				}
				else
				{
					correct = correct + 1;
					//cout << "predict right" << endl;
				}
				repo[ptr].predictLog[repo[ptr].history % pn] <<= 1;
				repo[ptr].history <<= 1;
			}
			break;
		}
		case(24):case(37):case(38): {
			correct = correct + 1;
			PC = prelog.tar_PC;
			break;
		}
		case(39):case(40): {
			correct = correct + 1;
			ra = prelog.raw_PC;
			PC = prelog.tar_PC;
			break;
		}
		default: {break; }
		}
		prelog.pre = 0;
	}
	else if (status[3] == 0) return;
	else cout << "Wrong Halt at stage mem" << endl;
}

void nxtCycle()
{
	Judge_WB();
	Judge_MEM();
	Judge_EX(); 
	Rejudge();
	Judge_ID();
	deal_with_control_hazard();
	Judge_IF();
}

int main(int argc, char *argv[])
{
	varibleini();
	Token_ini();
	reg_ini();
	//initialize step

	preprocess(argv[1]);
	ra = 0;
	while (!interrupt)
	{
		nxtCycle();
	}
	

	//cout << "Instruction.size =" << Instruction.size() << endl;
	//cout << "Static.size =" << Static.size() << endl;
	//cout << "\npredicition rate:" << (double)correct / total << endl;
	if (return_value)
		return return_value_is;
	else exit(0);
}

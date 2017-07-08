#pragma once
//func.h
#ifndef FUNC_H
#define FUNC_H

#include <string>

class Log;

class instruction_base;
class instruction_id;
class instruction_ex;
class instruction_mem;
class instruction_wb;
instruction_id cc_btoi(instruction_base Base, int npc);
instruction_ex cc_itoe(instruction_id Base, int r1, int s2);
instruction_mem cc_etom(instruction_ex Base, int res, int res_back, int addr);
instruction_wb cc_mtow(instruction_mem Base, int rep);

int c_wtoi(const std::string &str);
int c_htoi(const std::string &str);
int c_btoi(const char &c);
std::string c_itow(int i);
std::string c_itoh(int i);
char c_itob(int i);

instruction_id IF();
instruction_ex ID(instruction_id inst);
instruction_mem EX(instruction_ex inst);
instruction_wb MEM(instruction_mem inst);
bool WB(instruction_wb inst);

void require_WB();
void report_WB(int i);
void Judge_WB();
void require_MEM();
void Judge_MEM();
void require_EX();
void Judge_EX();
void require_ID();
void Judge_ID();

void get_relative_register(instruction_base *raw_ptr);
void get_wb_register(instruction_base *raw_ptr);
bool is_data_hazard(instruction_base *instptr, instruction_base *raw_ptr);

void deal_with_control_hazard();

void prepare(const std::string &f);
void preprocess(const std::string &f);

void varibleini();
void Token_ini();
void reg_ini();

#endif // !FUNC_H



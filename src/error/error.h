#pragma once


#define E			   "An error has ocured"
#define E_F			   "Could not read file"
#define E_EOF		   "Unexpected end of file"
#define E_PARSE_NODE   "Couldn't parse node"
#define E_INV_TOK	   E_PARSE_NODE ": invalid token"
#define E_UNK_TOK_NAME E_PARSE_NODE ": unknown token name"
#define E_INV_TOK_NAME E_PARSE_NODE ": invalid token name"
#define E_EPT_TOK_NAME E_PARSE_NODE ": empty token name"
#define E_DIRE_NIMPL   "Directive not implemented yet"
#define E_INV_TYPE	   "Invalid node type"
#define E_DEF_MAC	   "Macro already defined"
#define E_UDEF_MAC	   "Macro not defined yet"


void error(char error_message[]);
void error_s(char error_message[], char* arg);
void error_c(char error_message[], char arg);

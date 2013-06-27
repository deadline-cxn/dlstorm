/***************************************************************
 **      DL Code Storm Data Base Class                        **
 ***************************************************************/
#ifndef _DLCS_DATABASE
#define _DLCS_DATABASE

#include "dlstorm.h"
#include "c_gaf.h"

class CDBVar
{
public:
	CDBVar(void);
	~CDBVar();

	char name[1024];
	char value[1024];

	CDBVar *next_key;
	CDBVar *next_var;

};

class CDBTable
{
public:
	CDBTable(void);
	~CDBTable();

	int column_count;
	char name[1024];

	void AddColumn(char *columnname);
	void DelColumn(char *columnname);


	CDBVar *PosVar(char *var,char *val);
	//position on var that has val as value (return the key var)


	CDBVar *GetRowByColumnValue(char *var, char *val);

	CDBVar *GetKey(char *key);

	CDBVar *GetVar(CDBVar *key,char *var);
	CDBVar *GetVar(char *var);
	char   *GetVal(char *var);

	void InsertKey(char *key);

	void Update(char *var, char *val, char *key);

	std::vector <std::string> ColumnList(void);
	std::vector <std::string> DumpVars(void);

	CDBVar	*firstkey;

	CDBVar	*key;
	CDBTable *next;
};

class CDLDB
{
public:
    CDLDB(void);
    ~CDLDB();

	char filename[1024];

	void Load(char *file);
	void Save(char *file);


	char *TableList(void);
	void AddTable(char *tablen);
	void DropTable(char *tablen);
	void SelectTable(char *tablen);

	CDBTable *GetTable(char *tablen);
	CDBTable *GetParentTable(CDBVar *key);

	CDBTable *FirstTable(void);
	CDBTable *NextTable(void);

	CDBVar	*select(char *c);
	char	*parse(CDBVar *key,char *var);
	void	row_update(CDBVar *key, char *var, char *val);

	void	fill_rows_with_all_columns(CDBVar *key);


	CDBTable *firsttable;
	CDBTable *table;



};

#endif//_DLCS_DATABASE

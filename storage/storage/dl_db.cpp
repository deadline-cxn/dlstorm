/***************************************************************
 **   DL Storm Database class                                 **
 ***************************************************************/
#include "dl_db.h"

extern "C"
    void    Log(char *fmt,...);

CDBVar::CDBVar()
{
	next_key=NULL;
	next_var=NULL;
}
CDBVar::~CDBVar()
{
	DEL(next_key);
	DEL(next_var);
}


CDBTable::CDBTable()
{
	firstkey=NULL;
	key=NULL;
	next=NULL;
}

CDBTable::~CDBTable()
{
	DEL(firstkey);
	DEL(next);
}

CDBVar *CDBTable::GetRowByColumnValue(char *var, char *val)
{
	CDBVar *t=firstkey;
	CDBVar *r;
	while(t)
	{
		r=GetVar(t,var);
		if(dlcs_strcasecmp(r->value,val)) return t;
		t=t->next_key;
	}

	return t;
}

////////////////////////////////////////
CDBVar *CDBTable::GetVar(CDBVar *key,char *var)
{
	CDBVar *t;
	t=key;
	while(t)
	{
		if(dlcs_strcasecmp(t->name,var))
		{
			return t;
		}
		t=t->next_var;
	}
	return NULL;
}
////////////////////////////////////////
CDBVar *CDBTable::GetVar(char *var)
{
	CDBVar *t=firstkey;
	return(GetVar(t,var));
}
///////////////////////////////////

CDBVar *CDBTable::GetKey(char *key)
{
	CDBVar *t=firstkey;
	while(t)
	{
		if( dlcs_strcasecmp(t->value,key) )
		{
			return t;
		}
		t=t->next_key;
	}
	return NULL;
}

// //////////// //////// /// // / // 

void CDBTable::InsertKey(char *key)
{
	CDBVar *t=firstkey;
	int col=0;
	if(t!=NULL)
	{
		while(t->next_key!=NULL)
		{
			col++;
			t=t->next_key;
		}
		t->next_key=new CDBVar;
		col++;
		t=t->next_key;
	}
	else
	{
		t=new CDBVar;
		firstkey=t;
	}
	strcpy(t->name,firstkey->name);
	strcpy(t->value,key);
}

///////////////////////////////////////////////////////////

void CDBTable::Update(char *var, char *val, char *key)
{
	CDBVar *v=GetVar(var);
	CDBVar *k=GetKey(key);
	if(k==NULL)
	{
		InsertKey(key);
		k=GetKey(key);
	}
	CDBVar *x;
	x=k;
	int col;
	col=0;

	while(col<atoi(v->value))
	{
		col++;
		if(x->next_var)
			x=x->next_var;
		else
		{
			x->next_var=new CDBVar;
			x=x->next_var;
		}

	}
		
	if(x)
	{
		strcpy(x->name,var);
		strcpy(x->value,val);
		//Log(" (%s.%s->%s=%s)",name,k->name,v->name,val);
		//Log(" (%s.%s->%s=%s)",name,x->name,x->name,x->value);
	}
}

////////////////////////////////////////////////////

void CDBTable::AddColumn(char *columnname)
{
	CDBVar *t=firstkey;
	int col=0;
	if(t!=NULL)
	{
		while(t->next_var!=NULL)
		{
			col++;
			t=t->next_var;
		}
		t->next_var=new CDBVar;
		col++;
		t=t->next_var;
	}
	else
	{
		t=new CDBVar;
		firstkey=t;
	}

	strcpy(t->name,columnname);	
	strcpy(t->value,va("%d",col));
	column_count++;
}

////////////////////////////////////////////////////

void CDBTable::DelColumn(char *columnname)
{
	

}

////////////////////////////////////////////////////

std::vector <std::string> CDBTable::ColumnList(void)
{
	std::vector <std::string> rcl;
	key=firstkey;
	while(key)
	{
		rcl.push_back(key->name);
		key=key->next_var;	
	}
	return rcl;
}

////////////////////////////////////////////////////

std::vector <std::string> CDBTable::DumpVars(void)
{
	std::vector <std::string> rvl;
	CDBVar *v;

	key=firstkey;
	if(key) key=key->next_key;
	while(key)
	{
		v=key;
		while(v)
		{
			rvl.push_back(v->value);
			v=v->next_var;
			
		}	
		//rvl.push_back("\n");
		key=key->next_key;
	}
	return rvl;
}


////////////////////////////////////////////////////////
CDLDB::CDLDB()
{
	memset(filename,0,1024);
	firsttable=NULL;
	table=NULL;
}
////////////////////////////////////////////////////////
CDLDB::~CDLDB()
{
	DEL(firsttable);
}
////////////////////////////////////////////////////////
void CDLDB::Load(char *file)
{
	int i;
	std::vector <std::string> v,z;
    char in[1024]; memset(in,0,1024);
    char *Entry;
	char var[1024]; memset(var,0,1024);
	char val[1024]; memset(val,0,1024);
	char key[1024]; memset(key,0,1024);
	char cols[1024]; memset(cols,0,1024);

	int mode;

	mode=0;  // 0=TABLE, 1=COLUMNS, 2=DATA

	FILE *fp=fopen(file,"rt");

	while(1)
	{
		if(!fgets(in,1024,fp)) break;

		Entry=strtok(in,"\n\r");

		if(Entry)
		{
			
			if(dlcs_strcasecmp(Entry,"[TABLE]"))
			{
				mode=0;
				continue;
			}
			else
			if(dlcs_strcasecmp(Entry,"[COLUMNS]"))
			{
				mode=1;
				continue;
			}
			else
			if(dlcs_strcasecmp(Entry,"[DATA]"))
			{
				mode=2;
				continue;
			}
			else
			if(dlcs_strcasecmp(Entry,"[ROW]"))
			{
				mode=3;
				continue;
			}

			switch(mode)
			{
				
				case 0:
					AddTable(Entry);
					table=GetTable(Entry);
					break;
				
				case 1:
					
					strcpy(cols,Entry);
					v=explode(" ",Entry);
					for(i=0;i<v.size();i++)
					{
						if(strlen((char *)v[i].c_str()) )
						{
							table->AddColumn((char *)v[i].c_str());
						}
					}

					break;
				case 2:

					z=explode(" ",cols);
					v=explode(",",Entry);

					for(i=0;i<z.size();i++)
					{
						if(strlen((char *)v[i].c_str()) )
						{					
							table->Update(
								(char *)z[i].c_str(),
								(char *)v[i].c_str(),
								(char *)v[0].c_str() );
						}
					}

					break;
				case 3:

					break;
			}
		}

	}
	fclose(fp);
	

}
////////////////////////////////////////////////////////
void CDLDB::Save(char *file)
{
	int i;
	int j;
	
	std::vector <std::string> cl;
	std::vector <std::string> vl;
	FILE *fp=fopen(file,"wt");
	table=firsttable;
	while(table)
	{
		fputs("[TABLE]",fp);
		fputs("\n",fp);
		fputs(table->name,fp);
		fputs("\n",fp);
		
		cl=table->ColumnList();

		if(cl.size())
		{			
			fputs("[COLUMNS]",fp);
			fputs("\n",fp);
			for(i=0;i<cl.size();i++)
			{
				fputs(cl[i].c_str(),fp);
				fputs(" ",fp);
			}
			fputs("\n",fp);
		}

		vl=table->DumpVars();

		Log("%d",cl.size());

		j=0;
		if(vl.size())
		{
			fputs("[DATA]",fp);
			fputs("\n",fp);
			
			for(i=0;i<vl.size();i++)
			{
				fputs(vl[i].c_str(),fp);
				j++;
				if(j==cl.size()) 
				{
					fputs("\n",fp);
					j=0;
				}
				else
				{
					fputs(",",fp);
				}

			}
		}

		table=table->next;
	}

	fclose(fp);
}
////////////////////////////////////////////////////////
CDBTable * CDLDB::FirstTable()
{
	table=firsttable;
	return firsttable;
}
////////////////////////////////////////////////////////
CDBTable * CDLDB::NextTable()
{
	table=table->next;
	return table;
}
////////////////////////////////////////////////////////
CDBTable * CDLDB::GetTable(char *tablen)
{
	CDBTable *t=firsttable;
	while(t)
	{
		if(dlcs_strcasecmp(t->name,tablen))
			return t;
		t=t->next;
	}
	return t;
}

///////////////////////////////////////////////////

void	CDLDB::AddTable(char *tablen)
{
	CDBTable *t=firsttable;

	if(t!=NULL)
	{
		while(t->next!=NULL)
		{
			t=t->next;
		}
		t->next=new CDBTable;
		t=t->next;
	}
	else
	{
		t=new CDBTable;
		firsttable=t;
	}
	strcpy(t->name,tablen);
}

////////////////////////////////////////////////////

void	CDLDB::DropTable(char *tablen)
{

}

////////////////////////////////////////////////////////

CDBVar *CDLDB::select(char *c)
{
	CDBVar *retv;
	retv=NULL;
	std::vector <std::string> tv;
	tv=explode(" ",c);
	table=GetTable((char *)tv[0].c_str());
	if(!table) return NULL;
	retv=table->GetRowByColumnValue((char *)tv[1].c_str(),(char *)tv[2].c_str());
	return retv;
}

////////////////////////////////////////////////////////

char *CDLDB::parse(CDBVar *key,char *var)
{
	CDBVar *wt;
	wt=table->GetVar(key,var);
	if(wt)
	return wt->value;
	return NULL;

}

////////////////////////////////////////////////////////

void CDLDB::row_update(CDBVar *key, char *var, char *val)
{
	fill_rows_with_all_columns(key);	
	CDBVar *wkey;
	wkey=table->GetVar(key,var);
	if(wkey)
	{
		strcpy(wkey->value,val);
		//Log("%s %s",wkey->name,wkey->value);
	}
}

///////////////////////////////////////////////////////////

void CDLDB::fill_rows_with_all_columns(CDBVar *key)
{
	CDBVar *wkey;
	CDBVar *xkey;
	CDBTable *xtab;
	xtab=GetParentTable(key);
	if(xtab)
	{
		xkey=xtab->firstkey;
		wkey=key;
		while(xkey)
		{
			if(wkey)
			{
				strcpy(wkey->name,xkey->name);
				if(!strlen(wkey->value))
				{
					strcpy(wkey->value,"null");
				}
			}

			if(!wkey->next_var)
			{
				if(xkey->next_var)
				{
					wkey->next_var=new CDBVar;
				}
			}
			wkey=wkey->next_var;
			xkey=xkey->next_var;
		}
	}
}

///////////////////////////////////////////////////////////

CDBTable *CDLDB::GetParentTable(CDBVar *key)
{
	CDBTable *xtab;
	CDBVar	 *xkey;
	xtab=firsttable;
	while(xtab)
	{
		xkey=xtab->firstkey;
		while(xkey)
		{
			if(xkey==key) return xtab;
			xkey=xkey->next_key;
		}
		xtab=xtab->next;
	}
	return NULL;
}


// SQLiteTest.cpp : Defines the entry point for the console application.
//


#include "SQLLiteWrapper.h"
#include <iostream>

using namespace std;



int main(int argc, _TCHAR* argv[])
{	
	
	SQLiteDB *pSQLite;
	pSQLite=new SQLiteDB();

	//Connect TO database
	if(!pSQLite->OpenConnection("SQLiteTest.db","D:\\"))
	{
		cout<<"\nConnecting To DB Failed :"<<pSQLite->GetLastError().c_str();
		return -1;
	}

	//Create DB
	int effected=pSQLite->Excute("create table test (seq int(2),name varchar(50));");

	if(effected>0)
		cout<<"\nColoumn Effected :"<<effected;

	else
		cout<<"\nError in Query : "<<pSQLite->GetLastError().c_str();

	//Insert Into DB
	effected=pSQLite->Excute("insert into test (seq,name) values (1,'SQLLiteDBtest');");
	if(effected>0)
		cout<<"\nColoumn Effected :"<<effected;
	else
		cout<<"\nError in Query : "<<pSQLite->GetLastError().c_str();


	//Get Data From DB
	IResult *res=pSQLite->ExcuteSelect("Select * from test;");
	if(!res)
	  cout<<"\n Error:"<<pSQLite->GetLastError().c_str();
	
	else
	{
		//Get Column Count
		int i = res->GetColumnCount();
		
		//Print Colomn Name
		for(int k=0;k<i;k++)
		{
			printf("%s\t",res->NextColomnName(k));	
		}
		
		cout<<endl;

		//Print Result
		while(res->Next())
		{
			for(int k=0;k<i;k++)
				printf("%s\t",res->ColomnData(k));		
			cout<<endl;
		}

		//release Result Data
		res->Release();
	}


	//Insert Heavey Data in DB
	pSQLite->BeginTransaction();
	
	for(int i=0;i<10000;i++)
		pSQLite->Excute("insert into test (seq,name) values (1,'SQLLiteDBtest');");

	pSQLite->CommitTransection();

	delete pSQLite;

	cin.get();
	return 0;
}

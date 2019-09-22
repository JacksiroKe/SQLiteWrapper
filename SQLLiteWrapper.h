/******************************************************************************************
       Author : Saurabh Bhushan
       Dated  : 22 Feb 2013
  
    This Class provide the simple wrapper for SQLite DB.			 
******************************************************************************************/

#ifndef  _SQLLITE_DB
#define  _SQLLITE_DB
#endif

#pragma once

//OS Headers
#include <Windows.h>
#include <string>

//SQLite Header and library
#include "SQLite\sqlite3.h"
#pragma comment (lib,".\\SQLite\\sqlite3.lib")

using namespace std;


//Struct for Sync database for Multithreading
typedef struct Sync
{
	CRITICAL_SECTION _cs;
	Sync()			 { ::InitializeCriticalSection(&_cs); }
	void LockDB()    { ::EnterCriticalSection(&_cs);      }
	void UnLockDB()  { ::LeaveCriticalSection(&_cs);      }
	~Sync()          { ::DeleteCriticalSection(&_cs);     }
}SyncDB;




/*Interface class for Result data of query*/
class IResult
{
public:
	/*This function return of count of column 
	  present in result set of last excueted query*/
	virtual int	    GetColumnCount() = 0;

	/*Get the next coloumn name*/
	virtual const char* NextColomnName(int iClmnCount) = 0;
	
	/*This function returns TRUE if still rows are 
	der in result set of last excueted query FALSE 
	if no row present*/
	virtual bool  Next() = 0;

	/*Get the next coloumn data*/
	virtual const char*  ColomnData(int clmNum) = 0;

	/*RELEASE all result set as well as RESET all data*/
	virtual void Release() = 0;
};



//SQLite Wrapper Class
class SQLiteDB : public IResult
{

public:
	SQLiteDB();
	~SQLiteDB();
	
	/*Open Connection*/
	bool OpenConnection(string DatabaseName,string DatabaseDir); 


	/*Close Connection*/
	void CloseConnection();

	/*Query Wrapper*/
	/*For large insert operation Memory Insert option for SQLLITE dbJournal*/
	void BeginTransaction();
	void CommitTransection();

	/*This Method called when SELECT Query to be excuted. 
	Return RESULTSET class pointer on success else NULL of failed*/
	IResult*  ExcuteSelect(const char *Query);

	/*This Method called when INSERT/DELETE/UPDATE Query to be excuted. 
	Return UINT count of effected data on success*/
	UINT	    Excute(const char *Query);

	/*Get Last Error of excution*/
	string GetLastError();

	/*Return TRUE if databse is connected else FALSE*/
	bool  isConnected() ;	


protected:
	/*SQLite Connection Object*/
	typedef struct SQLLITEConnection
	{
		string		 SQLiteDatabaseName;   //Database Name
		string		 SQLiteDBPath;		   //Databse File Dir
		sqlite3		 *pCon;				   //SQLite Connection Object
		sqlite3_stmt *pRes;				   //SQLite statement object 
	}SQLITECONNECTIONOBJECT;

	//SQLite Connection Details
	SQLITECONNECTIONOBJECT	 *pSQLiteConn;

	/*Sync Database in Case of Multiple Threads using class object*/
	SyncDB					 *Sync;	

	bool	m_bConnected;      /*Is Connected To DB*/
	bool    m_bConsole;	       /*If Console Application*/
	string  m_strLastError;    /*Last Error String*/
	int	    m_iColumnCount;    /*No.Of Column in Result*/


private:
	/*This function return of count of column 
	  present in result set of last excueted query*/
	int	    GetColumnCount();

	/*Get the next coloumn name*/
	const char* NextColomnName(int iClmnCount);
	
	/*This function returns TRUE if still rows are 
	der in result set of last excueted query FALSE 
	if no row present*/
	bool  Next();

	/*Get the next coloumn data*/
	const char*  ColomnData(int clmNum);

	/*RELEASE all result set as well as RESET all data*/
	void Release(); 

};



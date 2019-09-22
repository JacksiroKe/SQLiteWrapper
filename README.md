# SQLite Wrapper for C++
## About SQLite
This is a simple SQLite wrapper written in c++. SQLite is an embedded SQL database engine,fully tested. SQLite does not have a separate server process. SQLite reads and writes directly to ordinary disk files.
The description about SQLite database is HERE.
SQLite release for Windows OS can be download from HERE: http://www.sqlite.org/download.html.
## SQLite Wrapper
This source code provide the simple methods to create database and run queries in portable database. Created SQLite database file is platform independent and can be use or replicate to Windows or Linux OS.

## SQLiteDB Class
This class contains the method of SQLite wrapper.

Method to open connection accept two argument first one is the database file name this could be * .db format and the directory where the DB file will create. If DB file is already present it will be open or else new file will create.
```
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
```

Where the select query can be excuted from the method IResult* ExcuteSelect(const char *Query); Which return the object of Interface class IResult.
```
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
```

## How to Use
The test file SQLiteTest.cpp has implmented using SQLLite Wrapper class. For large data insert insertion void void BeginTransaction(); and void CommitTransection(); method. Without using these method multiple insertion will take much longer time thn usual.

IResult class have pure virtual method to provide interface to read result data.
```
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
  ```
  Attachments: [SQLiteTest.cpp] [SQLLiteWrapper.cpp] [SQLLiteWrapper.h]

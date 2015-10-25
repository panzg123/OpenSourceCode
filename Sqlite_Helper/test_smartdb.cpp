// SmartDB1.03.cpp : 定义控制台应用程序的入口点。
//


#include "SmartDB.hpp"
#include "Json.hpp"
#include "json_helper.h"

void TestJson(SmartDB& db, const string& sqlinsert)
{
	//这里通过jsoncpp封装类来简化json对象的创建
	rapidjson::StringBuffer buf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
	writer.StartArray();
	for (size_t i = 0; i < 10; i++)
	{
		writer.StartObject();
		writer.String("ID");
		writer.Int(i + 1);

		writer.String("Name");
		writer.String("Peter");

		writer.String("Address");
		writer.String("Zhuhai");
		writer.EndObject();
	}
	writer.EndArray();
	auto r = db.ExecuteJson(sqlinsert, buf.GetString());
}

void TestJsonCpp()
{
	SmartDB db;
	db.Open("test.db");
	const string sqlcreat = "CREATE TABLE if not exists TestInfoTable(ID INTEGER NOT NULL, KPIID INTEGER, CODE INTEGER, V1 INTEGER, V2 INTEGER, V3 REAL, V4 TEXT);";
	if (!db.Execute(sqlcreat))
		return;


	JsonCpp jcp;
	jcp.StartArray();
	for (size_t i = 0; i < 1000000; i++)
	{
		jcp.StartObject();
		jcp.WriteJson("ID", i);
		jcp.WriteJson("KPIID", i);
		jcp.WriteJson("CODE", i);
		jcp.WriteJson("V1", i);
		jcp.WriteJson("V2", i);
		jcp.WriteJson("V3", i + 1.25);
		jcp.WriteJson("V3", "it is a test");

		jcp.EndObject();
	}
	jcp.EndArray();
	const string sqlinsert = "INSERT INTO TestInfoTable(ID, KPIID, CODE, V1, V2, V3, V4) VALUES(?, ?, ?, ?, ?, ?, ?);";
	bool r = db.ExecuteJson(sqlinsert, jcp.GetString());

	auto p = db.Query("select * from TestInfoTable");

	rapidjson::Document& doc = *p;
	for (size_t i = 0, len = doc.Size(); i < len; i++)
	{
		for (size_t i = 0, size = doc[i].GetSize(); i < size; ++i)
		{

		}
	}
	cout << "size: " << p->Size() << endl;
}

void TestCreateTable()
{
	SmartDB db;
	db.Open("test.db");

	const string sqlcreat = "CREATE TABLE if not exists PersonTable(ID INTEGER NOT NULL, Name Text, Address BLOB);";
	if (!db.Execute(sqlcreat))
		return;

	const string sqlinsert = "INSERT INTO PersonTable(ID, Name, Address) VALUES(?, ?, ?);";
	int id = 2;
	string name = "Peter";
	string city = "zhuhai";
	blob bl = { city.c_str(), city.length() + 1 };
	if (!db.Execute(sqlinsert, id, "Peter", nullptr))
		return;

	TestJson(db, sqlinsert);

	auto r = db.ExecuteTuple(sqlinsert, std::forward_as_tuple(id, "Peter", bl));
	char* json;
	string strQery = "select * from PersonTable";
	for (size_t i = 0; i < 10000; i++)
	{
		db.Query(strQery);
	}

	const string str = "select Address from PersonTable where ID=2";
	auto pname = db.ExecuteScalar<string>(str);
	auto l = strlen(pname.c_str());
	cout << pname << endl;
}

void TestPerformance()
{
	SmartDB db;
	db.Open("test.db");
	const string sqlcreat = "CREATE TABLE if not exists TestInfoTable(ID INTEGER NOT NULL, KPIID INTEGER, CODE INTEGER, V1 INTEGER, V2 INTEGER, V3 REAL, V4 TEXT);";
	if (!db.Execute(sqlcreat))
		return;

	
	const string sqlinsert = "INSERT INTO TestInfoTable(ID, KPIID, CODE, V1, V2, V3, V4) VALUES(?, ?, ?, ?, ?, ?, ?);";
	bool ret = db.Prepare(sqlinsert);
	db.Begin();
	for (size_t i = 0; i < 1000000; i++)
	{
		ret = db.ExecuteArgs(i, i, i, i, i, i + 1.25, "it is a test");
		if (!ret)
			break;
	}

	if (ret)
		db.Commit(); //提交事务
	else
		db.RollBack(); //回滚


	//100w 3.5-4秒左右

	auto p = db.Query("select * from TestInfoTable");


	cout << "size: " << p->Size() << endl;
	//100W 4秒左右
}

int main()
{
	TestJsonCpp();
	TestPerformance();
	TestCreateTable();
	system("pause");
	return 0;
}


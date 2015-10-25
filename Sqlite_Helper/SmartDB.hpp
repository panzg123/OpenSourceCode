#pragma  once
#include "sqlite3.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <functional>
#include <cctype>
#include <memory>
#include <type_traits>

#include "Define.h"
#include "BindParams.hpp"
#include "Json.hpp"
#include "Tuple.hpp"
#include "NonCopyable.hpp"

class SmartDB :NonCopyable
{
public:
	SmartDB():m_jsonHelper(m_buf,m_code){}

	/**
	* 连接数据库
	* 如果数据库不存在，数据库将被创建并打开, 如果创建失败则设置失败标志
	* @param[in] fileName：数据库文件的位置。
	*/
	explicit SmartDB(const string& fileName) :m_dbHandle(nullptr), m_statement(nullptr), m_isConned(false), m_code(0), m_jsonHelper(m_buf, m_code)
	{
		Open(fileName);
	}

	/*释放资源，关闭数据库*/
	~SmartDB()
	{
		Close();
	}

	/*打开数据库*/
	void Open(const string& fileName)
	{
		m_code = sqlite3_open(fileName.data(), &m_dbHandle);
		if (SQLITE_OK == m_code)
		{
			m_isConned = true;
		}
	}
	
	/*释放资源，关闭数据库*/
	bool Close()
	{
		if (m_dbHandle == nullptr)
		{
			return true;
		}
		sqlite3_finalize(m_statement);
		m_code = CloseDBHandle();
		bool ret = (SQLITE_OK == m_code);
		m_statement = nullptr;
		m_dbHandle = nullptr;
		return ret;
	}
	/*是否连接数据库*/
	bool IsConned()
	{
		return m_isConned;
	}

	/**
	* 不带占位符。执行sql，不带返回结果, 如insert,update,delete等
	* @param[in] query: sql语句, 不带占位符
	* @return bool, 成功返回true，否则返回false
	*/
	bool Execute(const string& sqlStr)
	{
		m_code = sqlite3_exec(m_dbHandle, sqlStr.data(), nullptr, nullptr, nullptr);
		return SQLITE_OK == m_code;
	}

	/**
	* 带占位符。执行sql，不带返回结果, 如insert,update,delete等
	* @param[in] query: sql语句, 可能带占位符"?"
	* @param[in] args: 参数列表，用来填充占位符
	* @return bool, 成功返回true，否则返回false
	*/
	template<typename... Args>
	bool Execute(const string& sqlStr, Args &&... args)
	{
		if (!Prepare(sqlStr))
		{
			return false;
		}
		return ExecuteArgs(std::forward<Args>(args)...);
	}


	/**
	* 批量操作之前准备sql接口，必须和ExcecuteBulk一起调用，准备批量操作的sql，可能带占位符
	* @param[in] query: sql语句, 带占位符"?"
	* @return bool, 成功返回true，否则返回false
	*/
	bool Prepare(const string& sqlStr)
	{
		m_code = sqlite3_prepare_v2(m_dbHandle, sqlStr.data(), -1, &m_statement, nullptr);
		if (m_code != SQLITE_OK)
		{
			return false;
		}
		return true;
	}


	/**
	* 批量操作接口，必须先调用Prepare接口
	* @param[in] args: 参数列表
	* @return bool, 成功返回true，否则返回false
	*/
	template<typename... Args>
	bool ExecuteArgs(Args &&... args)
	{
		//绑定参数
		if (SQLITE_OK != panzg_tool::BindParams(m_statement,1,std::forward<Args>(args)...))
		{
			return false;
		}
		//执行sql语句
		m_code = sqlite3_step(m_statement); 
		sqlite3_reset(m_statement);
		return m_code == SQLITE_DONE;
	}

	template<typename Tuple>
	bool ExecuteTuple(const string& sqlStr, Tuple&& t)
	{
		if (!Prepare(sqlStr))
		{
			return false;
		}
		m_code = panzg_tool::ExcecuteTuple(m_statement, panzg_tool::MakeIndexes<std::tuple_size<Tuple>::value>::type(), std::forward<Tuple>(t));
		return m_code == SQLITE_DONE;
	}

	bool ExecuteJson(const string& sqlStr,const char* json)
	{
		rapidjson::Document doc;
		doc.Parse<0>(json);
		if (doc.HasParseError())
		{
			cout << doc.GetParseError()<< endl;
			return false;
		}
		if (!Prepare(sqlStr))
		{
			return false;
		}
		return JsonTransaction(doc);
	}

	/**
	* 执行sql，返回函数执行的一个值, 执行简单的汇聚函数，如select count(*), select max(*)等
	* 返回结果可能有多种类型，返回Value类型，在外面通过get函数去取
	* @param[in] query: sql语句, 可能带占位符"?"
	* @param[in] args: 参数列表，用来填充占位符
	* @return int: 返回结果值，失败则返回-1
	*/
	template<typename R=sqlite_int64,typename... Args>
	R ExecuteScalar(const string& sqlStr, Args&&... args)
	{
		if (!Prepare(sqlStr))
		{
			return GetErrorVal<R>();
		}
		if (SQLITE_OK != panzg_tool::BindParams(m_statement,1,std::forward<Args>(args)...))
		{
			return GetErrorVal<R>();
		}
		m_code = sqlite3_step(m_statement);
		if (m_code != SQLITE_ROW)
		{
			return GetErrorVal<R>();
		}
		SqliteValue val = GetValue(m_statement, 0);
		R result = val.Get<R>();
		sqlite3_reset(m_statement);
		return result;
	}

	template<typename... Args>
	std::shared_ptr<rapidjson::Document> Query(const string& query, Args&&... args)
	{
		if (!PrepareStatement(query,std::forward<Args>(args)...))
		{
			nullptr;
		}
		auto doc = std::make_shared<rapidjson::Document>();
		m_buf.Clear();
		m_jsonHelper.BuildJsonObject(m_statement);
		doc->Parse<0>(m_buf.GetString());
		return doc;
	}

	bool Begin()
	{
		return Execute(BEGIN);
	}

	bool RollBack()
	{
		return Execute(ROLLBACK);
	}

	bool Commit()
	{
		return Execute(COMMIT);
	}

	int GetLastErrorCode()
	{
		return m_code;
	}
private:
	int CloseDBHandle()
	{
		int code = sqlite3_close(m_dbHandle);
		while (code == SQLITE_BUSY)
		{
			code = SQLITE_OK;
			sqlite3_stmt *stmt = sqlite3_next_stmt(m_dbHandle,NULL);

			if (stmt == nullptr)
			{
				break;
			}
			code = sqlite3_finalize(stmt);
			if (code == SQLITE_OK)
			{
				code = sqlite3_close(m_dbHandle);
			}
		}
		return code;
	}

	template <typename... Args>
	bool PrepareStatement(const string& sqlStr, Args&&... args)
	{
		if (!Prepare(sqlStr))
		{
			return false;
		}

		if (SQLITE_OK != panzg_tool::BindParams(m_statement, 1, std::forward<Args>(args)...))
		{
			return false;
		}

		return true;
	}

	//通过json串写到数据库
	bool JsonTransaction(const rapidjson::Document& doc)
	{
		Begin();
		for (size_t i = 0, size = doc.Size(); i < size;i++)
		{
			if (!m_jsonHelper.ExecuteJson(m_statement,doc[i]))
			{
				RollBack();
				break;
			}
		}
		if (m_code != SQLITE_OK)
			return false;
		Commit();
		return true;
	}
	//取列的值
	SqliteValue GetValue(sqlite3_stmt *stmt, const int& index)
	{
		int type = sqlite3_column_type(stmt, index);
		auto it = m_valmap.find(type);
		if (it == m_valmap.end())
			throw std::invalid_argument("can not find this type");

		return it->second(stmt, index);
	}

	template<typename T>
	typename std::enable_if<std::is_arithmetic<T>::value, T>::type GetErrorVal()
	{
		return T(-9999);
	}


	template<typename T>
	typename std::enable_if <!std::is_arithmetic<T>::value&&!std::is_same<T, blob>::value, T>::type GetErrorVal()
	{
			return "";
	}

	template<typename T>
	typename std::enable_if <std::is_same<T, blob>::value, T>::type GetErrorVal()
	{
		return{ nullptr, 0 };
	}

private:
	sqlite3* m_dbHandle;
	sqlite3_stmt* m_statement;
	bool m_isConned;
	int m_code;//记录最近一次的错误码

	panzg_tool::JsonHelper m_jsonHelper;
	rapidjson::StringBuffer m_buf; //json字符串的buf
	static std::unordered_map<int, std::function<SqliteValue(sqlite3_stmt*, int)>> m_valmap;
};
std::unordered_map<int, std::function <SqliteValue(sqlite3_stmt*, int)>> SmartDB::m_valmap = 
{
	{ std::make_pair(SQLITE_INTEGER, [](sqlite3_stmt *stmt, int index){return sqlite3_column_int64(stmt, index); }) },
	{ std::make_pair(SQLITE_FLOAT, [](sqlite3_stmt *stmt, int index){return sqlite3_column_double(stmt, index); }) },
	{ std::make_pair(SQLITE_BLOB, [](sqlite3_stmt *stmt, int index){return string((const char*)sqlite3_column_blob(stmt, index));/* SmartDB::GetBlobVal(stmt, index);*/ }) },
	{ std::make_pair(SQLITE_TEXT, [](sqlite3_stmt *stmt, int index){return string((const char*)sqlite3_column_text(stmt, index)); }) },
	{ std::make_pair(SQLITE_NULL, [](sqlite3_stmt *stmt, int index){return nullptr; }) }
};
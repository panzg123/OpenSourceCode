#pragma  once
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include <unordered_map>
#include <functional>
#include <cctype>
#include "sqlite3.h"

using JsonBuilder = rapidjson::Writer<rapidjson::StringBuffer>;
namespace panzg_tool
{
	class JsonHelper
	{
	public:
		JsonHelper(rapidjson::StringBuffer& buf, int code) :m_code(code), m_jsonBuilder(buf)
		{

		}

		/*绑定json值，并执行*/
		bool ExecuteJson(sqlite3_stmt *stmt, const rapidjson::Value& val)
		{
			auto p = val.GetKeyPtr();
			for (size_t i = 0, size = val.GetSize(); i < size;++i)
			{
				//获取json值
				const char* key = val.GetKey(p++);
				auto &t = val[key];

				//绑定json值
				if (SQLITE_OK!=BindJsonValue(stmt,t,i+1))
				{
					return false;
				}
			}

			m_code = sqlite3_step(stmt);
			sqlite3_reset(stmt);
			return SQLITE_DONE == m_code;
		}
		void BuildJsonObject(sqlite3_stmt* stmt)
		{
			//返回stmt结果的烈数目
			int colCount = sqlite3_column_count(stmt);
			m_jsonBuilder.StartArray();
			while (true)
			{
				m_code = sqlite3_step(stmt);
				if (m_code == SQLITE_DONE)
				{
					break;
				}
				BuildJsonArray(stmt, colCount);
			}
			m_jsonBuilder.EndArray();
			sqlite3_reset(stmt);
		}
	private:
		int BindJsonValue(sqlite3_stmt *stmt, const rapidjson::Value& t, int index)
		{
			auto type = t.GetType();
			if (type == rapidjson::kNullType)
			{
				m_code = sqlite3_bind_null(stmt, index);
			}
			else if (type == rapidjson::kStringType)
			{
				m_code = sqlite3_bind_text(stmt, index, t.GetString(), -1, SQLITE_STATIC);
			}
			else if (type == rapidjson::kNumberType)
			{
				BindNumber(stmt, t, index);
			}
			else
				throw std::invalid_argument("can't find this type");

			return m_code;
		}

		void BindNumber(sqlite3_stmt *stmt, const rapidjson::Value &t, int index)
		{
			if (t.IsInt() || t.IsUint())
			{
				m_code = sqlite3_bind_int(stmt, index, t.GetInt());
			}
			else if (t.IsInt64() || t.IsUint64())
			{
				m_code = sqlite3_bind_int64(stmt, index, t.GetInt64());
			}
			else
				m_code = sqlite3_bind_double(stmt, index, t.GetDouble());
		}

		//大写转换
		void ToUpper(char* s)
		{
			size_t len = strlen(s);
			for (size_t i = 0; i < len;i++)
			{
				s[i] = std::toupper(s[i]);
			}
		}

		void BuildJsonArray(sqlite3_stmt *stmt, int colCount)
		{
			m_jsonBuilder.StartObject();
			for (int i = 0; i < colCount;i++)
			{
				//sqlite3_column_name返回指定列的列名
				char *name = (char*)sqlite3_column_name(stmt, i);
				ToUpper(name);

				m_jsonBuilder.String(name);  //写段名
				BuildJsonValue(stmt, i);
			}
			m_jsonBuilder.EndObject();
		}

		void BuildJsonValue(sqlite3_stmt *stmt, int index)
		{
			int type = sqlite3_column_type(stmt, index);
			auto it = m_builderMap.find(type);
			if (it == m_builderMap.end())
			{
				throw std::invalid_argument("can not find this type");
			}
			it->second(stmt, index, m_jsonBuilder);
		}

	private:
		int& m_code;
		JsonBuilder m_jsonBuilder;
		static std::unordered_map<int, std::function<void(sqlite3_stmt *stmt, int index, JsonBuilder&)>> m_builderMap;
	};

	/*m_builderMap是一个静态unordered_map，用来将sqlite中获取的值写入到json串中*/
	std::unordered_map<int, std::function<void(sqlite3_stmt *stmt, int index, JsonBuilder&)>> JsonHelper::m_builderMap  
	{
		{ std::make_pair(SQLITE_INTEGER, [](sqlite3_stmt *stmt, int index, JsonBuilder& builder){ builder.Int64(sqlite3_column_int64(stmt, index)); }) },
		{ std::make_pair(SQLITE_FLOAT, [](sqlite3_stmt *stmt, int index, JsonBuilder& builder){ builder.Double(sqlite3_column_double(stmt, index)); }) },
		{ std::make_pair(SQLITE_BLOB, [](sqlite3_stmt *stmt, int index, JsonBuilder& builder)
		{
			//blob字段要注意获取实际的流长度
			builder.String((const char*)sqlite3_column_blob(stmt, index), sqlite3_column_bytes(stmt, index));
		})
		},
		{ std::make_pair(SQLITE_TEXT, [](sqlite3_stmt *stmt, int index, JsonBuilder& builder){ builder.String((const char*)sqlite3_column_text(stmt, index)); }) },
		{ std::make_pair(SQLITE_NULL, [](sqlite3_stmt *stmt, int index, JsonBuilder& builder){builder.Null(); }) }
	};
}
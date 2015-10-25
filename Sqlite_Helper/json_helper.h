#include <string>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
using namespace rapidjson;
using namespace std;

class JsonCpp
{
	typedef Writer<StringBuffer> JsonWriter;
public:
	JsonCpp() :m_writer(m_buf){}
	~JsonCpp(){}

	void StartArray()
	{
		m_writer.StartArray();
	}
	void EndArray()
	{
		m_writer.EndArray();
	}
	void StartObject()
	{
		m_writer.StartObject();
	}
	void EndObject()
	{
		m_writer.EndObject();
	}
	//写键值对
	template<typename T>
	void WriteJson(string &key, T &&value)
	{
		m_writer.String(key.c_str());
		//通过enable_if来重载writeValue将基本类型写入
		WriteValue(std::forward<T>(value));
	}
	
	template<typename T>
	void WriteJson(const char* key, T&& value)
	{
		m_writer.String(key);
		/*forward转发提高性能*/
		WriteValue(std::forward<T>(value));
	}

	/*返回对象序列化后端的json字符串*/
	const char* GetString() const
	{
		return m_buf.GetString();
	}
private:

	/*下面是重载统一各个类型的接口，让writeValue支持基本类型*/
	template<typename V>
	typename std::enable_if<std::is_same<V, int>::value>::type WriteValue(V value)
	{
		m_writer.Int(value);
	}
	template<typename V>
	typename std::enable_if<std::is_same<V, unsigned int>::value>::type WriteValue(V value)
	{
		m_writer.Uint(value);
	}
	template<typename V>
	typename std::enable_if<std::is_same<V, int64_t>::value>::type WriteValue(V value)
	{
		m_writer.Int64(value);
	}
	template<typename V>
	typename std::enable_if<std::is_floating_point<V>::value>::type WriteValue(V value)
	{
		m_writer.Double(value);
	}
	template<typename V>
	typename std::enable_if<std::is_same<V, bool>::value>::type WriteValue(V value)
	{
		m_writer.Bool(value);
	}
	template<typename V>
	typename std::enable_if<std::is_pointer<V>::value>::type WriteValue(V value)
	{
		m_writer.String(value);
	}
	template<typename V>
	typename std::enable_if<std::is_array<V>::value>::type WriteValue(V value)
	{
		m_writer.String(value);
	}
	template<typename V>
	typename std::enable_if<std::is_same<V, std::nullptr_t>::value>::type WriteValue(V value)
	{
		m_writer.Null();
	}
private:
	StringBuffer m_buf;
	JsonWriter m_writer;
	
};
#pragma once

#include <locale>
#include <codecvt>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <memory>

namespace mylib
{
    using namespace std;
    class JsonElement
    {
    public:
        using JsonNull = nullptr_t;
        using JsonBool = bool;
        using JsonInt = int;
        using JsonFloat = double;
        using JsonString = string;
        using JsonArray = vector<JsonElement>;
        using JsonObject = map<string, JsonElement>;
        using ElementValue = variant<JsonNull, JsonBool, JsonInt, JsonFloat, shared_ptr<JsonString>, shared_ptr<JsonArray>, shared_ptr<JsonObject>>;
        enum ElementType : size_t
        {
            JSON_NULL,
            JSON_BOOL,
            JSON_INT,
            JSON_FLOAT,
            JSON_STRING,
            JSON_ARRAY,
            JSON_OBJECT,
        };

    public:
        JsonElement() {}
        JsonElement(nullptr_t) {}
        JsonElement(JsonBool b) : m_type(JSON_BOOL), m_value(b) {}
        JsonElement(JsonInt n) : m_type(JSON_INT), m_value(n) {}
        JsonElement(JsonFloat n) : m_type(JSON_FLOAT), m_value(n) {}
        JsonElement(const char *str) : m_type(JSON_STRING), m_value(make_shared<JsonString>(str)) {}
        JsonElement(const JsonString &str) : m_type(JSON_STRING), m_value(make_shared<JsonString>(str)) {}
        JsonElement(JsonArray arr) : m_type(JSON_ARRAY), m_value(make_shared<JsonArray>(arr)) {}
        JsonElement(JsonObject obj) : m_type(JSON_OBJECT), m_value(make_shared<JsonObject>(obj)) {}
        // 浅拷贝以提高效率
        JsonElement(const JsonElement &other) : m_type(other.m_type), m_value(other.m_value) {}
        JsonElement &operator=(const JsonElement &other);

        bool operator==(const JsonElement &other) const;
        bool operator!=(const JsonElement &other) const { return !(*this == other); }

        // 类型转换
        operator JsonBool() const;
        operator JsonInt() const;
        operator JsonFloat() const;
        operator JsonString() const;
        JsonBool asBool() const { return static_cast<JsonBool>(*this); }
        JsonInt asInt() const { return static_cast<JsonInt>(*this); }
        JsonFloat asFloat() const { return static_cast<JsonFloat>(*this); }
        JsonString asString() const { return static_cast<JsonString>(*this); }

        // Json数组相关操作
        JsonElement &operator[](int index);
        void push(const JsonElement &other);
        JsonElement pop();
        void erase(size_t index);
        vector<JsonElement>::iterator begin() const;
        vector<JsonElement>::iterator end() const;

        // Json对象相关操作
        JsonElement &operator[](const char *key);
        JsonElement &operator[](const string &key);
        bool has(const char *key) const { return has(string(key)); }
        bool has(const string &key) const;
        void erase(const char *key) { erase(string(key)); }
        void erase(const string &key);

        // 对象和数组
        size_t size() const;
        bool empty() const { return size() == 0; }

        // 转换到字符串
        string toString() const;
        // 从字符中构造
        static JsonElement generate(const string &str) { return JsonParser(str).parse(); }

    private:
        // 不同类型的值
        JsonBool getBool() const { return get<JsonBool>(m_value); }
        JsonInt getInt() const { return get<JsonInt>(m_value); }
        JsonFloat getFloat() const { return get<JsonFloat>(m_value); }
        shared_ptr<JsonString> getString() const { return get<shared_ptr<JsonString>>(m_value); }
        shared_ptr<JsonArray> getArray() const { return get<shared_ptr<JsonArray>>(m_value); }
        shared_ptr<JsonObject> getObject() const { return get<shared_ptr<JsonObject>>(m_value); }
        // 不同类型的字符串
        string nullToString() const { return "null"; };
        string boolToString() const { return getBool() ? "true" : "fase"; };
        string intToString() const { return std::to_string(getInt()); };
        string floatToString() const { return std::to_string(getFloat()); }
        string stringToString() const { return "\"" + *getString() + "\""; }
        string arrayToString() const;
        string objectToString() const;
        // 类型检测
        void checkType(ElementType type, const string &msg) const;
        // 相等性检测
        bool nullEqual(const JsonElement &other) const { return true; }
        bool boolEqual(const JsonElement &other) const { return getBool() == other.getBool(); }
        bool intEqual(const JsonElement &other) const { return getInt() == other.getInt(); }
        bool floatEqual(const JsonElement &other) const { return getFloat() == other.getFloat(); }
        bool stringEqual(const JsonElement &other) const { return *getString() == *(other.getString()); }
        bool arrayEqual(const JsonElement &other) const { return *getArray() == *(other.getArray()); }
        bool objectEqual(const JsonElement &other) const { return *getObject() == *(other.getObject()); }

        // Json字符串解析
    private:
        class JsonParser
        {
        public:
            JsonParser(const string &str) : m_str(str), m_index(0) {}
            JsonElement parse();

        private:
            JsonElement parseNull();
            JsonElement parseBool();
            JsonElement parseNumber();
            JsonElement parseString();
            JsonElement parseArray();
            JsonElement parseObject();
            void skipSpace();

        private:
            static bool isNull(char ch) { return ch == 'n'; }
            static bool isBool(char ch) { return ch == 't' || ch == 'f'; }
            static bool isHex(char ch) { return isdigit(ch) || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f'); }
            static bool isNumber(char ch) { return isdigit(ch) || ch == '+' || ch == '-'; }
            static bool isString(char ch) { return ch == '"'; }
            static bool isArray(char ch) { return ch == '['; }
            static bool isObject(char ch) { return ch == '{'; }

        private:
            string m_str;
            size_t m_index;
        };

    private:
        ElementType m_type = JSON_NULL;
        ElementValue m_value = nullptr;
    };
}
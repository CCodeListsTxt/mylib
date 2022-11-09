#include "../include/json.hpp"

namespace mylib
{
    JsonElement &JsonElement::operator=(const JsonElement &other)
    {
        m_type = other.m_type;
        m_value = other.m_value;
        return *this;
    }

    bool JsonElement::operator==(const JsonElement &other) const
    {
        if (m_type != other.m_type)
            return false;

        switch (m_type)
        {
        case JSON_NULL:
            return nullEqual(other);
        case JSON_BOOL:
            return boolEqual(other);
        case JSON_INT:
            return intEqual(other);
        case JSON_FLOAT:
            return floatEqual(other);
        case JSON_STRING:
            return stringEqual(other);
        case JSON_ARRAY:
            return arrayEqual(other);
        case JSON_OBJECT:
            return objectEqual(other);
        }

        return false;
    }

    JsonElement::operator JsonElement::JsonBool() const
    {
        checkType(JSON_BOOL, "Json对象类型错误::JsonBool " + to_string(__LINE__));
        return getBool();
    }

    JsonElement::operator JsonInt() const
    {
        checkType(JSON_INT, "Json对象类型错误::JsonInt " + to_string(__LINE__));
        return getInt();
    }

    JsonElement::operator JsonElement::JsonFloat() const
    {
        checkType(JSON_FLOAT, "Json对象类型错误::JsonFloat " + to_string(__LINE__));
        return getFloat();
    }

    JsonElement::operator JsonElement::JsonString() const
    {
        checkType(JSON_STRING, "Json对象类型错误::JsonString " + to_string(__LINE__));
        return *getString();
    }

    JsonElement &JsonElement::operator[](int index)
    {
        if (m_type != JSON_ARRAY)
        {
            m_type = JSON_ARRAY;
            m_value = make_shared<JsonArray>();
        }
        shared_ptr<JsonArray> arr = getArray();
        // 如果index越界，在后面加上null
        if (index >= arr->size())
        {
            arr->push_back(JsonElement());
            return arr->back();
        }
        return arr->at(index);
    }

    void JsonElement::push(const JsonElement &other)
    {
        if (m_type != JSON_ARRAY)
        {
            m_type = JSON_ARRAY;
            m_value = make_shared<JsonArray>();
        }
        shared_ptr<JsonArray> arr = getArray();
        arr->push_back(other);
    }

    JsonElement JsonElement::pop()
    {
        checkType(JSON_ARRAY, "Json对象类型错误::JsonArray " + to_string(__LINE__));
        if (size() == 0)
            return JsonElement();
        JsonElement ret = getArray()->back();
        getArray()->pop_back();
        return ret;
    }

    void JsonElement::erase(size_t index)
    {
        checkType(JSON_ARRAY, "Json对象类型错误::JsonArray " + to_string(__LINE__));
        shared_ptr<JsonArray> arr = getArray();
        if (index < arr->size())
            arr->erase(arr->begin() + index);
    }

    vector<JsonElement>::iterator JsonElement::begin() const
    {
        checkType(JSON_ARRAY, "此JsonElement对象不可遍历 " + to_string(__LINE__));
        return getArray()->begin();
    }

    vector<JsonElement>::iterator JsonElement::end() const
    {
        checkType(JSON_ARRAY, "此JsonElement对象不可遍历 " + to_string(__LINE__));
        return getArray()->end();
    }

    size_t JsonElement::size() const
    {
        if (m_type == JSON_ARRAY)
            return getArray()->size();
        else if (m_type == JSON_OBJECT)
            return getObject()->size();
        return 0;
    }

    JsonElement &JsonElement::operator[](const char *key)
    {
        return (*(this))[string(key)];
    }

    JsonElement &JsonElement::operator[](const string &key)
    {
        if (m_type != JSON_OBJECT)
        {
            m_type = JSON_OBJECT;
            m_value = make_shared<JsonObject>();
        }
        shared_ptr<JsonObject> json_obj = getObject();
        return (*json_obj)[key];
    }

    bool JsonElement::has(const string &key) const
    {
        checkType(JSON_OBJECT, "Json对象类型错误::JsonObject " + to_string(__LINE__));
        return getObject()->contains(key);
    }

    void JsonElement::erase(const string &key)
    {
        checkType(JSON_OBJECT, "Json对象类型错误::JsonObject " + to_string(__LINE__));
        getObject()->erase(key);
    }

    string JsonElement::toString() const
    {
        switch (m_type)
        {
        case JSON_NULL:
            return nullToString();
        case JSON_BOOL:
            return boolToString();
        case JSON_INT:
            return intToString();
        case JSON_FLOAT:
            return floatToString();
        case JSON_STRING:
            return stringToString();
        case JSON_ARRAY:
            return arrayToString();
        case JSON_OBJECT:
            return objectToString();
        }
        return "";
    }

    // [ arg1, agr2, agr3 ]
    string JsonElement::arrayToString() const
    {
        if (size() == 0)
            return "[ ]";

        string buf;
        buf += "[ ";
        for (JsonElement &element : *getArray())
            buf += element.toString() + ", ";
        buf.erase(buf.size() - 2, 1); // 删除最后多余的 ','
        buf += "]";

        return buf;
    }

    // { key1:val1, key2:val2 }
    string JsonElement::objectToString() const
    {
        if (size() == 0)
            return "{ }";

        string buf;
        buf += "{ ";
        for (auto &[key, val] : *getObject())
            buf += '"' + key + '"' + ':' + val.toString() + ", ";
        buf.erase(buf.size() - 2, 1); // 删除最后多余的 ','
        buf += "}";

        return buf;
    }

    void JsonElement::checkType(ElementType type, const string &msg) const
    {
        if (m_type != type)
            throw new runtime_error(msg);
    }

    JsonElement JsonElement::JsonParser::parse()
    {
        skipSpace();
        char ch = m_str[m_index];
        if (isNull(ch))
            return parseNull();
        else if (isBool(ch))
            return parseBool();
        else if (isNumber(ch))
            return parseNumber();
        else if (isString(ch))
            return parseString();
        else if (isArray(ch))
            return parseArray();
        else if (isObject(ch))
            return parseObject();
        else
            throw new invalid_argument("Json字符串无效 " + to_string(__LINE__));
    }

    JsonElement JsonElement::JsonParser::parseNull()
    {
        if (m_str.substr(m_index, 4) == "null")
        {
            m_index += 4;
            skipSpace();
            return JsonElement();
        }
        throw new invalid_argument("Json字符串无效::null " + to_string(m_index) + " " + to_string(__LINE__));
    }

    JsonElement JsonElement::JsonParser::parseBool()
    {
        if (m_str.substr(m_index, 4) == "true")
        {
            m_index += 4;
            skipSpace();
            return JsonElement(true);
        }
        else if (m_str.substr(m_index, 5) == "false")
        {
            m_index += 5;
            skipSpace();
            return JsonElement(false);
        }
        throw new invalid_argument("Json字符串无效::bool " + to_string(m_index) + " " + to_string(__LINE__));
    }

    JsonElement JsonElement::JsonParser::parseNumber()
    {
        int signal = 1;                                     // 正负号
        if (m_str[m_index] == '+' || m_str[m_index] == '-') // '+' == 43,'-' == 45
        {
            signal = 44 - m_str[m_index];
            m_index += 1;
            skipSpace();
        }
        // 数字字符数量
        int number_count = 0; // 数字字符数量
        bool is_int = true;   // 是否是整数
        for (; m_index < m_str.size(); ++m_index)
        {
            if (isdigit(m_str[m_index]))
                number_count += 1;
            else if (m_str[m_index] == '.' && is_int)
                is_int = false;
            else if (m_str[m_index] == '.')
                throw new invalid_argument("Json字符串无效::number " + to_string(m_index) + " " + to_string(__LINE__));
            else
                break;
        }
        if (number_count == 0)
            throw new invalid_argument("Json字符串无效::number " + to_string(m_index) + " " + to_string(__LINE__));
        // 转换
        if (is_int)
        {
            int num = signal * atoi(m_str.substr(m_index - number_count, number_count).data());
            return JsonElement(num);
        }
        else
        {
            double num = signal * atof(m_str.substr(m_index - number_count - 1, number_count + 1).data());
            return JsonElement(num);
        }
    }

    JsonElement JsonElement::JsonParser::parseString()
    {
        string ret;
        // ++m_index，直接略过开始的 '"'
        while (++m_index < m_str.size())
        {
            char ch = m_str[m_index];
            // 右 '"' ，闭合字符串
            if (ch == '"')
            {
                m_index += 1;
                skipSpace();
                return JsonElement(ret);
            }
            // 控制字符
            else if (ch == '\\')
            {
                ch = m_str[++m_index];
                u32string unicode_str;
                switch (ch)
                {
                case '"':
                    ret += '"';
                    break;
                case '\\':
                    ret += '\\';
                    break;
                case 'b':
                    ret += '\b';
                    break;
                case 'f':
                    ret += '\f';
                    break;
                case 'n':
                    ret += '\n';
                    break;
                case 'r':
                    ret += '\r';
                    break;
                case 't':
                    ret += '\t';
                    break;
                // \uxxxx，unicode字符
                case 'u':
                    // \u后必须是四个十六进制的数
                    if (m_index + 4 >= m_str.size())
                        throw new invalid_argument("Json字符串无效::string " + to_string(m_index) + " " + to_string(__LINE__));
                    for (int i = 1; i <= 4; ++i)
                        if (!isHex(m_str[m_index + i]))
                            throw new invalid_argument("Json字符串无效::string " + to_string(m_index) + " " + to_string(__LINE__));
                    // unicode编码转到string
                    unicode_str += (char32_t)stoul(m_str.substr(m_index + 1, 4), nullptr, 16);
                    ret += wstring_convert<codecvt_utf8<char32_t>, char32_t>{}.to_bytes(unicode_str);

                    m_index += 4;
                    skipSpace();
                    break;
                default:
                    throw new invalid_argument("Json字符串无效::string " + to_string(m_index) + " " + to_string(__LINE__));
                }
            }
            else
                ret += ch;
        }
        throw new invalid_argument("Json字符串无效::string " + to_string(m_index) + " " + to_string(__LINE__));
    }

    JsonElement JsonElement::JsonParser::parseArray()
    {
        m_index += 1;
        skipSpace();
        vector<JsonElement> ret;
        if (m_str[m_index] == ']')
        {
            m_index += 1;
            skipSpace();
            return JsonElement(ret);
        }
        while (true)
        {
            ret.push_back(parse());
            // 闭合array
            if (m_str[m_index] == ']')
            {
                m_index += 1;
                skipSpace();
                return JsonElement(ret);
            }
            if (m_str[m_index] != ',' || m_index >= m_str.size())
                throw new invalid_argument("Json字符串无效::array " + to_string(m_index) + " " + to_string(__LINE__));
            m_index += 1;
            skipSpace();
        }
        return JsonElement(ret);
    }

    JsonElement JsonElement::JsonParser::parseObject()
    {
        m_index += 1;
        skipSpace();
        map<string, JsonElement> ret;
        if (m_str[m_index] == '}')
        {
            m_index += 1;
            skipSpace();
            return JsonElement(ret);
        }
        while (true)
        {
            if (m_index > m_str.size())
                throw new invalid_argument("Json字符串无效::object " + to_string(m_index) + " " + to_string(__LINE__));
            // 解析键
            JsonElement key = parseString();
            //
            if (m_str[m_index] != ':')
                throw new invalid_argument("Json字符串无效::object " + to_string(m_index) + " " + to_string(__LINE__));
            m_index += 1;
            skipSpace();
            // 解析值
            ret.emplace(key.asString(), parse());
            // 闭合object
            if (m_str[m_index] == '}')
            {
                m_index += 1;
                skipSpace();
                return ret;
            }
            if (m_str[m_index] != ',')
                throw new invalid_argument("Json字符串无效::object " + to_string(m_index) + " " + to_string(__LINE__));
            m_index += 1;
            skipSpace();
        }
        return JsonElement(ret);
    }

    void JsonElement::JsonParser::skipSpace()
    {
        while (m_str[m_index] == ' ' || m_str[m_index] == '\n' || m_str[m_index] == '\t')
            m_index += 1;
    }
}
#include "../include/json.hpp"
#include <iostream>

using namespace mylib;
using namespace std;

#define LINE cout << "--------------------\n";
#define SHOW(obj) cout << obj.toString() << " " << obj.size() << endl;
#define TEST(str) cout << "-------" << str << "----------\n";

int main()
{
    // null测试
    TEST("null测试");
    JsonElement json_null;
    SHOW(json_null);
    json_null = JsonElement::generate(R"(null)");
    SHOW(json_null);
    LINE;

    // bool测试
    TEST("bool测试");
    JsonElement json_true = true;
    JsonElement json_false = false;
    SHOW(json_true);
    SHOW(json_false);
    json_true = JsonElement::generate(R"(true)");
    json_false = JsonElement::generate(R"(false)");
    SHOW(json_true);
    SHOW(json_false);
    LINE;

    // number测试
    TEST("number测试");
    JsonElement json_int = 10;
    JsonElement json_double = 20.;
    SHOW(json_int);
    SHOW(json_double);
    json_int = json_int.asInt() + 30;
    SHOW(json_int);
    json_double = json_double.asFloat() - 33.2;
    SHOW(json_double);
    json_int = JsonElement::generate("12345");
    json_double = JsonElement::generate("12.345");
    SHOW(json_int);
    SHOW(json_double);
    LINE;

    // string测试
    TEST("string测试");
    JsonElement json_str = R"("hello json")";
    SHOW(json_str);
    json_str = R"("hello\njson\t\u2620")";
    SHOW(json_str);
    json_str = JsonElement::generate(R"("hello\njson\t\u2620")");
    SHOW(json_str);
    LINE;

    // array测试
    TEST("array测试");
    JsonElement json_arr;
    json_arr[0] = "first_element";
    json_arr[1] = 1;
    json_arr[2] = 2.;
    json_arr.push(nullptr);
    SHOW(json_arr);
    json_arr.pop();
    json_arr.erase(1);
    SHOW(json_arr);
    json_arr = JsonElement::generate(R"( ["hello",1,false,null,"hello\njson\t\u2620"] )");
    SHOW(json_arr);
    LINE;

    // object测试
    TEST("object测试");
    JsonElement json_obj;
    json_arr["key"] = "value";
    json_arr["int"] = 1;
    json_arr["float"] = 22.3;
    json_arr["arr"] = JsonElement::generate("[1,2,3]");
    json_arr["obj"] = JsonElement::generate(R"({"key":"value"})");
    SHOW(json_arr);
    json_obj = JsonElement::generate(R"(
        {
            "key":"value",
            "bool":false,
            "int":-   123456,
            "float":12.3456,
            "arr":[
                1,2,3,[4,5,6]
            ],
            "obj":{
                "key":"value",
                "str":"hello\njson\t\u2620"
            }
        }
        )");

    SHOW(json_obj);
    cout << json_obj["invalid_key"].toString() << endl;
    LINE;

    return 0;
}
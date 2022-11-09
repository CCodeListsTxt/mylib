#### 简介

* 提供Json数据格式的基本操作，未实现注释解析。

* **JsonElement**是负责处理Json数据的核心类，通过**std::variant**实现几种Json类型的联合体数据。

  ```cpp
  using JsonNull = nullptr_t;						// null
  using JsonBool = bool;							// 布尔值
  using JsonInt = int;							// 整数
  using JsonFloat = double;						// 浮点数
  using JsonString = string;						// 字符串
  using JsonArray = vector<JsonElement>;			// json数组
  using JsonObject = map<string, JsonElement>;	// json对象，键值对集合
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
  ```

* **JsonElement**维护**m_type**和**m_value**数据成员，对应Json数据类型和Json数据值。

  ```cpp
  ElementType m_type = JSON_NULL;
  ElementValue m_value = nullptr;
  ```
  
* **JsonElement**的拷贝控制函数均为浅拷贝，以提高拷贝时效率。暂未实现深拷贝。

---

#### 基本使用

```cpp
#include "json.hpp"
using namespace mylib;

int main()
{
    // 通过json字符串解析JsonElement对象
	JsonElement json = JsonElement::generate("true");
    
    // 修改JsonElement对象的值
    json = false;
    
    // 输出json字符串
    cout << json.toString() << endl;
    
    // json array
    JsonElement json_arr;
    json_arr[0] = 1;
    json_arr[10] = 2;	// 越界时，自动添加到末尾
    json_arr.push(3);	// 添加到末尾
    json_arr.pop();		// 移除末尾元素
    json_arr.erase(1);	// 删除元素
    
    // json object
    JsonElement json_obj;
    json_obj["key"] = "value";
    json_obj["key2"] = null;
    json_obj.erase("key2");	// 删除元素
    
	return 0;
}
```

---

#### API

```cpp
// 比较JsonElemnt对象
bool operator==(const JsonElement &other) const;
bool operator!=(const JsonElement &other) const;

// 隐式类型转换，如果转换失败，会抛出 std::runtime_error* 异常
operator JsonBool() const;
operator JsonInt() const;
operator JsonFloat() const;
operator JsonString() const;
// 显示类型转换，如果转换失败，会抛出 std::runtime_error* 异常
JsonBool asBool() const;
JsonInt asInt() const;
JsonFloat asFloat() const;
JsonString asString() const;

// 通过下标访问json_array，如果越界，将在末尾添加null元素
// 如果此JsonElement对象不是json_array，会清空所有数据，并将其设置为json_array
JsonElement &operator[](int index);
// 在json array末尾插入元素
// 如果此JsonElement对象不是json_array，会清空所有数据，并将其设置为json_array
void push(const JsonElement &other);
// 删除json array末尾元素，如果array为空，返回null元素
// 如果此JsonElement对象不是json_array，会抛出 std::runtime_error* 异常
JsonElement pop();
// 删除指定下标元素，如果下标无效，什么都不做
// 如果此JsonElement对象不是json_array，会抛出 std::runtime_error* 异常
void erase(size_t index);
// 迭代器，只限json_array类型的对象使用
// 如果此JsonElement对象不是json_array，会抛出 std::runtime_error* 异常
vector<JsonElement>::iterator begin() const;
vector<JsonElement>::iterator end() const;


// 通过键访问json object
// 如果此时JsonElement对象不是json_object，会清空所有数据，并将其设置为json_object，且返回null元素
JsonElement &operator[](const char *key);
JsonElement &operator[](const string &key);
// 判断是否拥有某个键
// 如果此时JsonElement对象不是json_object，会抛出 std::runtime_error* 异常
bool has(const string &key) const;

// json_array和json_object的大小属性
// 对于其他类型，返回值无意义
size_t size() const;
bool empty() const;

// 转换到字符串，以及从字符串构造JsonElement
// 如果json字符串有误，会抛出 std::invalid_argument* 异常
string toString() const;
static JsonElement generate(const string &str);
```


#include <string>  
#include <iostream>  
#include <fstream>  
#include <walle/ext/xml/rapidxml.hpp>  
#include <walle/ext/xml/rapidxml_utils.hpp> 
#include <walle/ext/xml//rapidxml_print.hpp>  

  
  
using namespace rapidxml;  
using namespace std;  
  
int main()  
{  
  
    xml_document<> doc; //是解析器  
    char a[] = "<top>"//如果单独传, 就不能加上xml的头部信息，  
               //否则会报错  
               "<name>tangqiang</name>"  
               "<age>22</age>"  
               "</top>";  
    char* p = a;  
    doc.parse<0>(p);  
  
    xml_node<>* node = doc.first_node();//去顶级结点  
    cout << (node->name())<< endl;  
    node = node->first_node();  
    while (node) {  
        cout << node->name() << node->value() << endl;//name() value()返回的字符串不会去掉首尾的空白字符  
        node = node->next_sibling();  
    }  
  
    ofstream out("test.xml");//ofstream 默认时，如果文件存在则会覆盖原来的内容，不存在则会新建  
    out << doc;//doc 这样输出时在目标文件中不会有xml 头信息---<?xml version='1.0' encoding='utf-8' >  
    out.close();  
    return 0;  
}  


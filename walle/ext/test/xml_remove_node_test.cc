#include <walle/ext/xml/rapidxml.hpp>  
#include <walle/ext/xml/rapidxml_utils.hpp> 
#include <walle/ext/xml//rapidxml_print.hpp>  
  

#include<iostream>  
using namespace rapidxml;  
  
int main()  
{  
    file<> fdoc("config.xml");  
    xml_document<> doc;  
    doc.parse<0>(fdoc.data());  
  
    std::string text;    
    rapidxml::print(std::back_inserter(text), doc, 0);    
    std::cout<<text<<std::endl;   
  
    xml_node<>* root = doc.first_node();  
  
    xml_node<>* sec = root->first_node();  
  
    root->remove_node(sec); //移除根节点下的sec结点(包括该结点下所有结点)  
    text="删除一个节点\r\n";    
    rapidxml::print(std::back_inserter(text), doc, 0);    
    std::cout<<text<<std::endl;   
  
    root->remove_all_nodes(); //移除根节点下所有结点  
    text="删除所有节点\r\n";    
    rapidxml::print(std::back_inserter(text), doc, 0);    
    std::cout<<text<<std::endl;   
  
    std::ofstream out("test.xml");  
    out<<doc;  
    system("pause");  
    return 0;  
} 


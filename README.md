# clib (Library For C) 
clib是一个以C语言开发的工具包。将开发过程常用的数据结构，算法和工具模块组织起来，形成一个帮助开发的工具包。

## 基本组成

### 数据结构和算法
工具包中实现了几个常用的数据结构和算法，如 sstring, HashTable, Sigle List, bitmap 。
* sstring

sstring取名自Smart String，意为智能字符串。能够根据需求自动扩展空间，从此不再为字符串溢出和烦恼 。

* HashTable

一个参考PHP内核的HashTable实现，现仅支持字符串索引，实现了常用的HashTable操作函数。


### 工具模块

* logger
日志记录模块。

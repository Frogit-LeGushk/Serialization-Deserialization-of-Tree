# Serialization-Deserialization-of-Tree
### How it work:
![alt-text](https://github.com/Acool4ik/Serialization-Deserialization-of-Tree/blob/master/images/Screencast.gif)
### Structure of tree:
![alt-text](https://github.com/Acool4ik/Serialization-Deserialization-of-Tree/blob/master/images/Tree.gif)
### Description: 
The simple serialization-deserialization API for storing a structure such as a tree in a binary file for the purpose of transferring data to another host or retrieving data later.
### Notes: 
- `server` do only serialization of tree into binary file `text.txt`
- To view a binary file, I recommend such a utility as `ghex`. Also you can use `printBuffer` function (her give the same result)
- `client` only read generated lately binary file and restore Tree
- Used breadth-first traversal 

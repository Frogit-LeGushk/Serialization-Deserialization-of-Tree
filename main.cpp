#include "Tree.hpp"



void server() {
    /** 1) create root of tree with needed entity **/
    Tree::NodeTree root(Types::I8(8));


    /** 2) form tree needed topology **/
    root.pushChild(Types::I16(16));
    root.pushChild(Types::I32(32));
    root.pushChild(Types::I64(64));
    root[0].pushChild(Types::Float(3.14));
    root[1].pushChild(Types::Double(6.28));
    root[2].pushChild(Types::Text(std::string("Hello")));
    root[2].pushChild(Types::Text(std::string("World")));


    /** 3) serialize tree into buffer **/
    uint64_t bytes = 0;
    Tree::traversalTree(root, Tree::calc_save_print_tree, static_cast<void *>(&bytes));
    Buffer treebuffer(bytes);
    Tree::traversalTree(root, Tree::packTree, static_cast<void *>(&treebuffer));


    /** 4) print buffer and save him in file **/
    //treebuffer.printConsole();
    treebuffer.saveInFile("text.txt");
}

void client() {
    /** 1) load file into buffer and print buffer **/
    Buffer filebuffer;
    filebuffer.loadFromFile("text.txt");
    //filebuffer.printConsole();


    /** 2) deserialize buffer and print retrieved tree **/
    Tree::NodeTree * retrieved_root;
    Tree::retrieveTree(&retrieved_root, filebuffer);
    Tree::traversalTree(*retrieved_root, Tree::calc_save_print_tree, nullptr);
}


int main(int n, char **v)
{
    if(n == 2 && v[1][0] == 's') server();
    if(n == 2 && v[1][0] == 'c') client();
    return 0;
}

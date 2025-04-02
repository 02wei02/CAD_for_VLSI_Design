/* create the b*-tree from graph.cpp */
#ifndef BTREE_H
#define BTREE_H
//----------------------------------------------------------------------------------
#include "structure.h"
#include "contour.h"
#include "constraint.h"
#include <iostream>
#include <vector>
#include <string>
#include <vector>
#include <map>
#include <chrono>
using namespace std;
//----------------------------------------------------------------------------------
class BTree
{
public:
    BTree(const vector<InSym *> &sym_map,
          const vector<string> &order,
          const map<string, InBlock *> &BBF,
          const int &num_sym,
          const map<string, vector<string>> &net,
          const map<string, string> &MToB,
          const map<string, vector<string>> &npmos,
          const double &ratio)
    {
        this->sym_map = sym_map;
        this->order = order;
        this->BBF = BBF;
        this->num_sym = num_sym;
        this->net = net;
        this->MToB = MToB;
        this->npmos = npmos;
        this->ratio = ratio;
    }

    void Run()
    {
        srand(time(NULL));
        Tour contour;
        // std::cout << "start CreateSymNode ok" << endl;
        createNodeMap(head, ans_map);
        // std::cout << "end CreateSymNode ok" << endl;
        // contour.printContour(head);
        SA(order);
        // std::cout << "end SA ok" << endl;
        // printTree(a_trees);
        // printBTreeMap();
        std::cout << "\nCreate ok" << endl;
    }
    vector<TreeNode *> trees;        // inital trees including all nodes (single node and symmetry node)
    TreeNode *a_trees;               // trees to a B*-tree
    map<string, TreeNode *> ans_map; // add B*-tree to map;
    map<string, vector<string>> net; // net from placement
    map<string, string> MToB;        // find the mos in a block

    double ans_HPWL;
    double ans_x; // total width
    double ans_y; // total height
    double min_x;
    double min_y;
    Contour *head;
    double ratio; // expected ratio
    time_t start;
    time_t end;
    Constraint constraint; // class for constraint

private:
    void connectTrees(TreeNode *&parent, TreeNode *&child, int isSym, double &max_x, double &max_y, map<string, TreeNode *> &map, Contour *&path); // two tree connect
    void printForest();                                                                                                                            // print all value in tree vector
    void printTree(TreeNode *root);                                                                                                                // preorder traversal
    void createNodeMap(Contour *&head, map<string, TreeNode *> &ans_map);                                                                          // create symmetry module node
    bool compareAllInOrder(TreeNode *a, TreeNode *b, vector<string> &now_order);                                                                   // all node compare rule according to its distance from VDD or VSS
    void TreeSort(vector<TreeNode *> &one_trees, vector<string> &now_order);                                                                       // Sort tree according to CompareAllInOrder
    void buildATree(TreeNode *&root, bool left, bool recurse, double &max_x, double &max_y, map<string, TreeNode *> &map, Contour *&path);         // create the a tree output
    void addTreeNodeToMap(std::map<std::string, TreeNode *> &map, const std::string &key, TreeNode *node);                                         // turn the TreeNode* into map
    void printBTreeMap();                                                                                                                          // print the B*-tree map
    void findSymAxis(TreeNode *&root, double &axis);                                                                                               // find the symmetry axis
    void createSymLoc(TreeNode *row, double &axis, double &max_x, double &max_y, map<string, TreeNode *> &map, Contour *&path);                    // find the location of symmetry node
    void calculateHPWL(double &HPWL, map<string, TreeNode *> &map);                                                                                // calculated the half-perimeter wire length
    void refreshAxis(TreeNode *&root, double axis);                                                                                                // refresh the symmetry axis in preorder traversal
    void pickBockSize(TreeNode *&node);                                                                                                            // pick the block size from .block
    void SA(vector<string> &order);
    void swapOrder(vector<string> &order);
    TreeNode *copyTree(TreeNode *root);
    map<string, TreeNode *> copyTreeMap(const map<string, TreeNode *> &originalMap);
    void swapNodes(TreeNode *&nodeA, TreeNode *&nodeB);
    // Function to perform deep copy of tree
    vector<InSym *> sym_map;    // from placement then graph refresh inform sym_map
    vector<string> order;       // the order close -> far from VSS or VDD
    map<string, InBlock *> BBF; // from placement .block
    int num_sym;                // symmetry number
    Tour tour;                  // class for contour calculation
    map<string, vector<string>> npmos;
};
#endif
// B*tree
//-------------------------------------------------------------------------------------------

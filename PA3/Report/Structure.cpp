// structure.h
#ifndef STRUCT_H
#define STRUCT_H
#include <string>
#include <vector>

using namespace std;
//-----------------------------------------------------------------------
struct InBlock // intermediate files (.block)
{
    string name;               // the first BB or MM that appears
    vector<string> contents;   // if it is BB that has MM
    vector<vector<double>> wh; // width, height, col_multiple, row_multiple
    int size_pick;             // pick the size of blocks in block.wh
    int num;                   // number types of block
};
//-----------------------------------------------------------------------
struct InSym // symmetry constraint files (.sym)
{
    int index;          // symmetry key
    vector<string> sym; // symmetry block (at most 2)
};
//-----------------------------------------------------------------------
// define b*-tree structure
struct TreeNode
{
    InBlock block;
    string sym_name;          // the block name which is symmetry to the first BB or MM
    double axis;              // symmetry axis
    vector<double> xy;        // xy location of the block
    vector<double> center_xy; // center xy value of the block
    vector<double> max_xy;    // max xy value of the block
    int isSym;                // that is the symmetry block
    TreeNode *left;
    TreeNode *right;
    TreeNode *parent;
    TreeNode(InBlock n, string sym) : block(n), sym_name(sym), axis(0), left(nullptr), right(nullptr), parent(nullptr), xy(2), center_xy(2), max_xy(2), isSym(0) {}
};
//-----------------------------------------------------------------------
struct Contour
{
    double x1, x2, y; // (x1, x2)
    Contour *next;

    Contour() : x1(0), x2(0), y(0), next(nullptr) {} // Proper default constructor
};

#endif // STRUCT_H

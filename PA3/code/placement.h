/* Doing parser and Run other class */
#ifndef PLACEMENT_H
#define PLACEMENT_H
//----------------------------------------------------------------------------------------------------------------------------------------------------
#include "graph.h"
#include "btree.h"
#include "structure.h"
#include <string>
#include <map>
#include <vector>
#include <set>
using namespace std;
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
class Placement
{
public:
    Placement(string expected_aspect_ratio, string netlist_file, string symmetry_constraint_file, string building_block_file, string output_file)
        : expected_ratio_str(move(expected_aspect_ratio)),
          netlist_str(move(netlist_file)),
          symmetry_constraint_str(move(symmetry_constraint_file)),
          building_block_str(move(building_block_file)),
          output_str(std::move(output_file)) {} // receive the data from argv

    void Run()
    {
        NetlistInput();
        SymmetryInput();
        BuildingInput();
        RatioInput();
        // NetlistOutput();
        // NetOutput();
        // BuildingOutput();
        // cout << "Input ok" << endl;
        Graph graph(net,
                    npmos,
                    MToB,
                    sym_map);
        graph.Run();
        // cout << "Graph ok" << endl;
        BTree tree(graph.sym_map,
                   graph.result,
                   BBF,
                   num_sym,
                   net,
                   MToB,
                   npmos,
                   ratio);
        // cout << "Build tree ok" << endl;
        tree.Run();
        trees_map = tree.ans_map;
        // cout << "Tree ok" << endl;
        AnsOutput(tree.a_trees,
                  tree.ans_x,
                  tree.ans_y,
                  tree.ans_HPWL);

        // cout << "Ans ok" << endl;
    }
    map<string, vector<string>> mos_map; // mos information in .netlist
    map<string, vector<string>> net;     // key is the net and value is mos
    map<string, vector<string>> npmos;   // store n or pmos
    vector<InSym *> sym_map;             // symmetry constraint file information
    map<string, InBlock *> BBF;          // Building block file store structure
    map<string, string> MToB;            // key is mos and value is it belongs to which block
    map<string, TreeNode *> trees_map;   // map from btree.cpp
    double ratio;

private:
    string expected_ratio_str;
    string netlist_str;
    string symmetry_constraint_str;
    string building_block_str;
    string output_str;

    int num_mos; // mos count
    int num_sym; // symmetry count

    void NetlistInput();                                                                              // process the .netlist into mos_map
    void RatioInput();                                                                                // process the expected aspect ratio
    void SymmetryInput();                                                                             // process the .sym into sym_map
    void BuildingInput();                                                                             // process the .block into Inblock structure
    void AnsOutput(const TreeNode *trees, const double max_x, const double max_y, const double HPWL); // process .output file
    void printTree(const TreeNode *root, ofstream &out);

    void NetlistOutput();  // print mos and its net
    void NetOutput();      // print net and its mos
    void SymmetryOutput(); // print symmetry block
    void BuildingOutput(); // print building block
    void MToBOutput();     // turn the mos into block
};

#endif
// PLACEMENT_H
//----------------------------------------------------------------------------------------------------------------------------------------------------
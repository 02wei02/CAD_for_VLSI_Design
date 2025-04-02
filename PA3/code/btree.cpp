/* create the b*-tree from graph.cpp */
#include "btree.h"
#include "constraint.h"

#include <iostream>
#include <algorithm> // remove_if
#include <float.h>   // double max
#include <chrono>

using namespace std;

void BTree::printForest()
{
    std::cout << "Binary Tree Forest:" << endl;
    for (auto tree : trees)
    {
        printTree(tree);
        std::cout << endl;
    }
}

void BTree::printTree(TreeNode *root)
{
    if (root == nullptr)
        return;
    std::cout << root->block.name;
    std::cout << "\tSN  " << root->sym_name;
    std::cout << "\tWH";
    const auto &row = root->block.wh[root->block.size_pick];

    int i = 0;
    for (const auto &value : row)
    {
        if (i < 2)
            std::cout << "\t" << value;
        i++;
    }
    std::cout << "\tXY";
    for (const auto &value : root->xy)
    {
        std::cout << "\t" << value;
    }
    std::cout << "\tC  ";
    for (const auto &content : root->block.contents)
    {
        std::cout << content << " ";
    }
    std::cout << endl;
    printTree(root->left);
    printTree(root->right);
}

void BTree::connectTrees(TreeNode *&parent, TreeNode *&child, int isSym, double &max_x, double &max_y, map<string, TreeNode *> &map, Contour *&path)
{
    // std::cout << "connect trees start ok\n";
    int random;
    if (parent == nullptr || child == nullptr)
    {
        return;
    }

    if (isSym != 0)
    {
        // std::cout << "Connecting Sym" << std::endl;
        parent->right = child; // vertical
        child->parent = parent;
        child->isSym = isSym;
        buildATree(child, random, 0, max_x, max_y, map, path);
    }
    else
    {
        TreeNode *current = parent;
        while (true)
        {
            random = rand() % 2;

            while (current->isSym != 0)
            {
                if (current->right == nullptr)
                    break;
                current = current->right; // vertical
            }

            if (random)
            {
                if (current->left == nullptr)
                {
                    // std::cout << "Connecting horizontally" << std::endl;
                    current->left = child; // horizontal
                    child->parent = current;
                    break;
                }
                else
                {
                    current = current->left;
                }
            }
            else
            {
                if (current->right == nullptr)
                {
                    // std::cout << "Connecting vertically" << std::endl;
                    current->right = child; // vertical
                    child->parent = current;
                    break;
                }
                else
                {
                    current = current->right;
                }
            }
        }
        // std::cout << "Building tree for child" << std::endl;
        buildATree(child, random, 0, max_x, max_y, map, path);
    }
    // std::cout << "connect trees end ok\n";
}

void BTree::pickBockSize(TreeNode *&node)
{
    int pick = rand() % node->block.num;
    vector<double> whpick = node->block.wh[pick];
    vector<double> whnow;
    for (int i = 0; i < node->block.num; i++)
    {
        int now = i;
        whnow = node->block.wh[now];

        if (this->constraint.AspectRatio(whpick[0], whpick[1]) >= this->constraint.AspectRatio(whnow[0], whnow[1]))
        {
            pick = i;
        }
        whpick = node->block.wh[pick];
    }
    node->block.size_pick = pick; // choose block size
}

// The main function to call other function to create the map
void BTree::createNodeMap(Contour *&head, map<string, TreeNode *> &ans_map)
{
    time(&start);
    bool isFirst;
    bool isSecond;
    vector<string> order1 = this->order; // initial order
    TreeNode *root;
    TreeNode *r1;
    TreeNode *r2; // r2 to record the previous TreeNode
    double axis;  // symmetry axis
    ans_x = 0;
    ans_y = 0;

    for (int i = 0; i < this->num_sym; i++)
    {
        isFirst = true; // to determine whether have to the root
        axis = 0;
        head = nullptr;
        // std::cout << "num_sym ok: " << i << endl;
        for (auto &row : this->sym_map) // row in the sym_map
        {
            string name = row->sym[0]; // first M or B in the row
            string name2;
            if (row->sym.size() == 2)
            {
                name2 = row->sym[1]; // second M or B in the row
                order.erase(remove_if(order.begin(), order.end(), [&name2](const string &s)
                                      { return s == name2; }),
                            order.end()); // find name in order1 if find it delete
                order1.erase(remove_if(order1.begin(), order1.end(), [&name2](const string &s)
                                       { return s == name2; }),
                             order1.end()); // find name in order1 if find it delete
            }
            if (row->index == i)
            {
                if (isFirst)
                {
                    cout << "first" << endl;
                    if (row->sym.size() == 1)
                        root = new TreeNode(*this->BBF[name], "0"); // self-symmetric module
                    else
                        root = new TreeNode(*this->BBF[name], name2); // symmetry pair
                    pickBockSize(root);
                    root->isSym = i + 1;
                    buildATree(root, 0, 0, ans_x, ans_y, ans_map, this->head);
                    isFirst = false;
                    r2 = root;
                }
                else
                {
                    cout << "connect" << endl;
                    if (row->sym.size() == 1)
                        r1 = new TreeNode(*this->BBF[name], "0"); // self-symmetric module
                    else
                        r1 = new TreeNode(*this->BBF[name], name2); // symmetry pair
                    // Example operation: connect parent r2 to child r1
                    pickBockSize(r1);
                    connectTrees(r2, r1, i + 1, ans_x, ans_y, ans_map, this->head); // i+1 is needed , due to 0 is set as block is no needed to consider symmetry
                    r2 = r1;                                                        // Update r2 to the current TreeNode
                    order.erase(remove_if(order.begin(), order.end(), [&name](const string &s)
                                          { return s == name; }),
                                order.end()); // find name in order1 if find it delete
                }
                findSymAxis(r2, axis);
                order1.erase(remove_if(order1.begin(), order1.end(), [&name](const string &s)
                                       { return s == name; }),
                             order1.end()); // find name in order1 if find it delete
            } // end if row = i
        } // end all row in sym_map
        refreshAxis(root, axis); // refresh all axis in this symmetry tree
        head = nullptr;
        cout << "build" << endl;
        buildATree(root, 0, 1, ans_x, ans_y, ans_map, this->head); // horizontal is 1
        printTree(root);
        cout << "sym head " << endl;
        tour.printContour(head);
        this->trees.push_back(root); // initial trees
    }
    cout << "end sym" << endl;
    for (auto &i : order1) // push back other node (exclude block in symmetry file) to trees in order
    {

        axis = 0;
        root = new TreeNode(*this->BBF[i], "0"); // a block (don't need to consider symmetry)
        pickBockSize(root);
        root->isSym = 0;
        findSymAxis(root, axis);
        refreshAxis(root, axis); // refresh all axis in this symmetry tree
        buildATree(root, 1, 0, ans_x, ans_y, ans_map, this->head);
        this->trees.push_back(root);
    }
    cout << "end single" << endl;

    std::cout << endl;
    TreeSort(this->trees, order); // sort the trees in order from graph.cpp
    // std::cout << "TreeSort ok" << endl;
    ans_x = 0;
    ans_y = 0;
    head = nullptr;
    a_trees = copyTree(trees[0]); // allocate a_trees node
    buildATree(a_trees, 0, 1, ans_x, ans_y, ans_map, this->head);
    // this->tour.printContour(this->head);
    if (trees.size() > 1)
    {
        for (size_t i = 1; i < trees.size(); ++i)
        {
            root = copyTree(trees[i]);
            connectTrees(a_trees, root, 0, ans_x, ans_y, ans_map, this->head);
        }
    }
    head = nullptr;
    buildATree(a_trees, 0, 1, ans_x, ans_y, ans_map, head);
    calculateHPWL(ans_HPWL, ans_map);
    cout << "end initial" << endl;
}
void BTree::SA(vector<string> &order)
{
    int times = 0;
    std::vector<std::string> now_order = order; // order for BB and MOS
    std::vector<std::string> temp_order = order;
    double now_HPWL, temp_HPWL;
    TreeNode *now_trees = nullptr, *temp_trees = nullptr;
    double t = 6000; // temperature
    double cost;
    map<std::string, TreeNode *> now_map = ans_map;
    map<std::string, TreeNode *> temp_map;
    Contour *path = nullptr;

    // 初始化 start 和 end
    time(&start);

    if (trees.size() == 1)
        return;
    double now_x = 0, now_y = 0, temp_x = 0, temp_y = 0;
    while (!this->constraint.TimeLimited(times, start, end))
    {
        temp_order = now_order;

        int a = rand() % temp_order.size();
        cout << "a " << a << "\t";
        int b = rand() % temp_order.size();
        cout << "b " << b << endl;
        // Ensure a and b are not the same
        while (a == b)
        {
            b = rand() % temp_order.size();
        }
        std::swap(temp_order[a], temp_order[b]);
        TreeSort(this->trees, temp_order);
        path = nullptr;
        cout << "order: ";
        for (const auto &o : order)
        {
            std::cout << o << " ";
        }
        cout << endl;
        std::cout << "Swapped Order: ";
        for (const auto &o : temp_order)
        {
            std::cout << o << " ";
        }
        std::cout << std::endl;

        temp_trees = copyTree(trees[0]); // Deep copy tree
        buildATree(temp_trees, 0, 1, temp_x, temp_y, temp_map, path);
        // std::cout << "0 r: ";
        // printTree(temp_trees);
        for (size_t i = 1; i < trees.size(); i++)
        {
            TreeNode *root = copyTree(trees[i]);
            connectTrees(temp_trees, root, 0, temp_x, temp_y, temp_map, path);
            // cout << i << " r: ";
            // printTree(root);
        }
        path = nullptr;
        temp_map.clear();
        temp_x = 0, temp_y = 0;
        min_x = 0, min_y = 0;
        buildATree(temp_trees, 0, 1, temp_x, temp_y, temp_map, path);
        // cout << " temp ok : " << endl;
        // printTree(temp_trees);

        calculateHPWL(temp_HPWL, temp_map);

        cost = this->constraint.Cost(temp_x, temp_y, temp_HPWL, ratio) - this->constraint.Cost(ans_x, ans_y, ans_HPWL, ratio);
        cout << "cost: " << cost << endl;
        if (this->constraint.pick(cost, t))
        {
            now_map = copyTreeMap(temp_map);
            now_x = temp_x - min_x;
            now_y = temp_y - min_y;
            now_HPWL = temp_HPWL;
            now_order = temp_order;
            now_trees = temp_trees;
        }
        if (cost < 0)
        {
            std::cout << "change" << std::endl;
            ans_map = copyTreeMap(temp_map);
            ans_x = temp_x - min_x;
            ans_y = temp_y - min_y;
            order = temp_order;
            ans_HPWL = temp_HPWL;
            a_trees = temp_trees;
        }
        // delete temp_trees; // Free memory if already allocated
        t = t * 0.99;
        times++;
        // std::cout << "Iteration: " << times << ", Temperature: " << t << ", Cost: " << cost << std::endl;
    }
}

map<string, TreeNode *> BTree::copyTreeMap(const map<string, TreeNode *> &originalMap)
{
    map<string, TreeNode *> newMap;
    for (const auto &pair : originalMap)
    {
        newMap[pair.first] = copyTree(pair.second);
    }
    return newMap;
}

TreeNode *BTree::copyTree(TreeNode *root)
{
    if (root == nullptr)
    {
        return nullptr;
    }

    // create new node and copy
    TreeNode *new_node = new TreeNode(*root);

    // copy left right child
    new_node->left = copyTree(root->left);
    new_node->right = copyTree(root->right);

    // parent node
    if (new_node->left != nullptr)
    {
        new_node->left->parent = new_node;
    }
    if (new_node->right != nullptr)
    {
        new_node->right->parent = new_node;
    }

    return new_node;
}

void BTree::createSymLoc(TreeNode *root, double &axis, double &max_x, double &max_y, map<string, TreeNode *> &map, Contour *&path)
{
    // std::cout << "loc 1 ok" << endl;
    if (root == nullptr || root->sym_name == "0")
        return;
    TreeNode *r1 = new TreeNode(root->block, root->block.name); // block copy and its name is sym
    string name = root->block.name;                             // first M or B in the row
    string sym = root->sym_name;
    r1->axis = root->axis;
    // std::cout << name << " axis:\t" << r1->axis << endl;

    r1->block.size_pick = root->block.size_pick;
    double width = root->block.wh[r1->block.size_pick][0];
    double height = root->block.wh[r1->block.size_pick][1];
    // 0: x 1: y
    r1->isSym = root->isSym;
    r1->xy[0] = root->max_xy[0]; // this need to notice
    r1->xy[1] = root->xy[1];

    // std::cout << "loc 2 ok" << endl;

    r1->center_xy[0] = r1->xy[0] + width / 2;
    r1->center_xy[1] = r1->xy[1] + height / 2;
    // std::cout << "loc down ok" << endl;

    r1->max_xy[0] = r1->xy[0] + width;
    r1->max_xy[1] = r1->xy[1] + height;
    // std::cout << "loc 3 ok" << endl;

    this->tour.refresh(r1->xy[0], r1->max_xy[0], r1->max_xy[1], path);

    if (r1->max_xy[0] > max_x)
        max_x = r1->max_xy[0];
    if (r1->xy[0] < min_x)
        min_x = r1->xy[0];
    if (r1->max_xy[1] > max_y)
        max_y = r1->max_xy[1];
    if (r1->xy[0] < min_y)
        min_y = r1->xy[1];
    // std::cout << "loc 5 ok" << endl;
    addTreeNodeToMap(map, sym, r1);
    // std::cout << "loc 6 ok" << endl;
    // std::cout << "sym node: " << sym << endl;
}
void BTree::refreshAxis(TreeNode *&root, double axis)
{
    if (root == nullptr)
        return;
    root->axis = axis;
    refreshAxis(root->left, axis);
    refreshAxis(root->right, axis);
}
void BTree::findSymAxis(TreeNode *&root, double &axis)
{
    double w = root->block.wh[root->block.size_pick][0];
    if (axis == 0) // axis shouldn't be 0 (every block has its width)
    {
        if (root->sym_name == "0") // self symmetry
            axis = w / 2;
        else // pair
            axis = w;
    }
    if (root->max_xy[0] > axis && root->parent != nullptr) // this block center is axis
    {
        if (root->sym_name == "0") // self symmetry
            axis = w / 2;
        else // pair
            axis = w;
    }
}

void BTree::addTreeNodeToMap(map<string, TreeNode *> &map, const string &key, TreeNode *node)
{
    map[key] = node; // map create
}

void BTree::TreeSort(vector<TreeNode *> &one_trees, vector<string> &now_order)
{
    sort(one_trees.begin(), one_trees.end(), [this, &now_order](TreeNode *a, TreeNode *b)
         { return compareAllInOrder(a, b, now_order); });
}

bool BTree::compareAllInOrder(TreeNode *a, TreeNode *b, vector<string> &now_order)
{
    // Ensure the pointers are not null
    if (a && b)
    {
        // Ensure the vectors in a and b are not empty
        if (!a->block.name.empty() && !b->block.name.empty())
        {
            auto iter_a = find(now_order.begin(), now_order.end(), a->block.name);
            auto iter_b = find(now_order.begin(), now_order.end(), b->block.name);
            // std::cout << a->block.name << ":" << iter_a - now_order.begin() << ", " << b->block.name << ":" << iter_b - now_order.begin() << endl;

            if (iter_a != now_order.end() && iter_b != now_order.end())
            {
                return iter_a < iter_b;
            }
        }
    }
    return false;
}

void BTree::buildATree(TreeNode *&root, bool left, bool recurse, double &max_x, double &max_y, map<string, TreeNode *> &map, Contour *&path) // include contour
{
    // Return if the root node is null
    if (root == nullptr)
    {
        return;
    }

    // Retrieve the current block dimensions
    vector<double> c_pick = root->block.wh[root->block.size_pick]; // current pick
    double w = c_pick[0];                                          // current width
    double h = c_pick[1];                                          // current height

    double y;
    double axis = root->axis;

    // Determine the x and y coordinates based on the parent node
    if (root->parent == nullptr) // root node
    {
        root->xy[0] = 0;            // x
        root->xy[1] = 0;            // y
        root->max_xy[0] = w;        // the max x value of the block
        root->center_xy[0] = w / 2; // center x value of the block
        root->max_xy[1] = h;        // the max y value of the block
        root->center_xy[1] = h / 2; // center y value of the block
    }
    else
    {
        vector<double> p_pick = root->parent->block.wh[root->parent->block.size_pick]; // parent pick
        double pw = p_pick[0];                                                         // parent width
        double ph = p_pick[1];                                                         // parent height

        if (root->parent->sym_name == "0") // parent is self-symmetry
        {
            root->xy[0] = root->parent->center_xy[0];
        }
        else // parent is symmetry pair
        {
            root->xy[0] = root->parent->max_xy[0];
        }

        if (root->parent->isSym == root->isSym && root->isSym != 0) // parent and current are in symmetry block
        {
            if (root->sym_name == "0") // current node is self-symmetry
            {
                root->xy[0] -= w / 2;
            }
            else // current node is symmetry pair
            {
                root->xy[0] -= w;
            }
        }
        else if (left) // horizontal positioning
        {
            root->xy[0] = root->xy[0] + root->parent->axis + root->axis;
            if (root->sym_name == "0") // current node is self-symmetric module
            {
                root->xy[0] -= w / 2;
            }
            else // current node is symmetry pair
            {
                root->xy[0] -= w;
            }
        }
        else // vertical positioning
        {
            root->xy[0] = root->xy[0] - root->parent->axis + root->axis;
            if (root->sym_name == "0") // current node is self-symmetric module
            {
                root->xy[0] -= w / 2;
            }
            else // current node is symmetry pair
            {
                root->xy[0] -= w;
            }
        }

        root->center_xy[0] = root->xy[0] + w / 2;
        root->max_xy[0] = root->xy[0] + w;

        if (root->sym_name == "0")
        {
            y = this->tour.findY(root->xy[0], root->max_xy[0], path); // Use the contour to find the y value
        }
        else
        {
            y = this->tour.findY(root->xy[0], root->max_xy[0] + w, path);
        }

        root->xy[1] = y;                // y coordinate
        root->max_xy[1] = y + h;        // max y
        root->center_xy[1] = y + h / 2; // center y
    }
    // cout << "b loc ok" << endl;
    // Update maximum x and y coordinates
    if (root->max_xy[0] > max_x)
    {
        max_x = root->max_xy[0];
    }
    if (root->xy[0] < min_x)
    {
        min_x = root->xy[0];
    }
    // cout << "max x: " << root->max_xy[0] << "\t";
    if (root->max_xy[1] > max_y)
    {
        max_y = root->max_xy[1];
    }
    if (root->xy[1] < min_y)
    {
        min_y = root->xy[1];
    }
    // cout << "max y: " << root->max_xy[1] << endl;
    // cout << "b max ok" << endl;
    // Refresh the contour with the current block
    this->tour.refresh(root->xy[0], root->max_xy[0], root->max_xy[1], path);
    // cout << "tour ok: " << root->block.name << endl;
    // Add the current node to the map
    if (root != nullptr) // in build a tree need to generate map
    {
        addTreeNodeToMap(map, root->block.name, root);

        if (root->sym_name != "0")
        {
            createSymLoc(root, root->axis, max_x, max_y, map, path); // Create symmetry block into map
        }
    }
    if (recurse)
    {
        buildATree(root->left, 1, 1, max_x, max_y, map, path);
        buildATree(root->right, 0, 1, max_x, max_y, map, path);
    }
    // cout << "b end ok" << endl;
}

void BTree::printBTreeMap()
{
    int count = 0;
    std::cout << "PrintTreeMap: " << endl;
    cout << "Name\tX\tY\tCenterX\tCenterY\tMaxX\tMaxY\taxis\tsym" << endl;
    for (const auto &i : ans_map)
    {
        std::cout << i.first << "\t";
        auto &j = i.second;
        for (const auto &xy : j->xy)
            std::cout << xy << "\t";
        for (const auto &xy : j->center_xy)
            std::cout << xy << "\t";
        for (const auto &xy : j->max_xy)
            std::cout << xy << "\t";
        std::cout << i.second->axis << "\t";
        std::cout << i.second->isSym;
        std::cout << endl;
        count++;
    }
    std::cout << "count in ans_map: " << count << endl;
}
void BTree::calculateHPWL(double &HPWL, map<string, TreeNode *> &map)
{
    double x1, x2;
    double y1, y2;
    HPWL = 0;
    for (const auto &i : net)
    {
        // if (!npmos[i.first].empty())
        //     continue;
        x1 = DBL_MAX; // the left x
        x2 = 0;       // the right x
        y1 = DBL_MAX; // the lowest y
        y2 = 0;       // the highest y

        for (const auto &mos : i.second)
        {
            string name;
            if (this->MToB.count(mos) == 1) // MM turn to BB
            {
                name = MToB[mos];
            }
            else
            {
                name = mos; // MM
            }
            if (x1 > map[name]->center_xy[0])
                x1 = map[name]->center_xy[0];
            if (x2 < map[name]->center_xy[0])
                x2 = map[name]->center_xy[0];
            if (y1 > map[name]->center_xy[1])
                y1 = map[name]->center_xy[1];
            if (y2 < map[name]->center_xy[1])
                y2 = map[name]->center_xy[1];
        }
        // cout << "o: " << i.first << "\t";
        // cout << (x2 - x1) + (y2 - y1) << endl;
        HPWL += (x2 - x1) + (y2 - y1);
    }
    // cout << HPWL << endl;
}
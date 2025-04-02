void BTree::SA(vector<string> &order1)
{
    int times = 0;
    std::vector<std::string> now_order = order1; // order for BB and MOS
    double now_HPWL;
    TreeNode *now_trees = nullptr;
    double t = 6000; // temperature
    double cost;
    map<std::string, TreeNode *> now_map;
    Contour *path = nullptr;

    // Initialization start
    time(&start);

    while (!this->constraint.TimeLimited(times, start, end))
    {
        double now_x = 0, now_y = 0;
        swapOrder(now_order); // swap for two string in vector
        TreeSort(this->trees, now_order);
        path = nullptr;

        std::cout << "Swapped Order: ";
        for (const auto &o : now_order)
        {
            std::cout << o << " ";
        }
        std::cout << std::endl;

        std::cout << "Before buildATree" << std::endl;
        now_trees = copyTree(trees[0]); // copyTree
        buildATree(now_trees, 0, 1, now_x, now_y, now_map, path);
        std::cout << "After buildATree" << std::endl;

        if (trees.size() > 1)
        {
            for (size_t i = 1; i < trees.size(); ++i)
            {
                std::cout << "Before connectTrees, i: " << i << std::endl;
                connectTrees(now_trees, trees[i], 0, now_x, now_y, now_map, path);
                std::cout << "After connectTrees, i: " << i << std::endl;
            }
        }

        calculateHPWL(now_HPWL, now_map);

        cost = this->constraint.Cost(now_x, now_y, now_HPWL, ratio) - this->constraint.Cost(ans_x, ans_y, ans_HPWL, ratio);
        if (this->constraint.pick(cost, t))
        {
            std::cout << "change" << std::endl;
            ans_map = copyTreeMap(now_map);
            ans_x = now_x;
            ans_y = now_y;
            ans_HPWL = now_HPWL;
            a_trees = copyTree(now_trees);
        }
        t = t * 0.99;
        times++;
    }
}
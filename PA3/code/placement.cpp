/* Doing parser and Run other class */
#include "placement.h"
#include <iostream>
#include <fstream> //ifstream
#include <sstream> //istrinstream
#include <string>
#include <algorithm> // find
#include <iomanip>   // std::setprecision, std::fixed
// #include <cctype>

using namespace std;
//----------------------------------------------------------------------------------------------------------------------------------------------------
void Placement::NetlistInput() // process the .netlist into mos_map
{
    ifstream in(netlist_str);
    string s;
    string mos, drain, gate, source, body, width, length, wl_ratio; // mos information
    this->num_mos = 0;
    if (!in)
        cout << "SPICE netlist file can't be open!" << endl;
    else
        while (getline(in, s))
        {
            istringstream iss(s);
            iss >> mos >> drain >> gate >> source >> body;
            if (mos[0] != 'M') // this is not the mos
                continue;
            else
            {
                vector<string> t;
                t.clear();
                t.push_back(drain);
                t.push_back(gate);
                t.push_back(source);
                t.push_back(body);
                this->mos_map[mos] = t; // put the .netlist inf in map
                // 以 net 為準 連線 mos
                this->npmos[body].push_back(mos);
                this->net[drain].push_back(mos);
                this->net[gate].push_back(mos);
                this->net[source].push_back(mos);
                this->num_mos++;
            }
        }
    in.close();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void Placement::SymmetryInput() // process the .sym into sym_map
{
    ifstream in(symmetry_constraint_str);
    string s;
    string name;
    string sym_str;
    vector<string> record_sym;
    vector<string> t;
    int index;

    if (!in)
        cout << "Symmetry constraint file can't be open!" << endl;
    else
    {

        while (getline(in, s))
        {
            istringstream iss(s);
            InSym *temp_map = new InSym;
            iss >> name;

            // find current string in record_sym
            auto it = std::find(record_sym.begin(), record_sym.end(), name);

            while (iss >> sym_str)
            {
                t.push_back(sym_str); // put data in t
            }

            if (it != record_sym.end())
            {
                // if find the current string in record_sym, get the index
                index = it - record_sym.begin();
            }
            else
            {
                // if don't find current string in record_sym , add to record_sym , and get new index
                record_sym.push_back(name);
                index = record_sym.size() - 1;
            }
            temp_map->index = index;
            temp_map->sym = t;
            sym_map.push_back(temp_map);

            t.clear();
        }

        // 更新 num_sym
        this->num_sym = record_sym.size();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Placement::BuildingInput() // process the .block into Inblock structure
{
    ifstream in(building_block_str);
    string s;
    string name;            // the first words in line
    string all_devices;     // temp
    vector<string> devices; // all_devices_names
    string number;          // temp
    vector<double> size;    // block size include width height col and row
    if (!in)
    {
        cout << "Building block file can't be open!" << endl;
    }
    else
    {
        while (getline(in, s))
        {
            InBlock *t = new InBlock;
            istringstream iss(s);
            double wh;
            iss >> name;
            int i = 0;
            int j = 0; // count number shapes of blocks
            // bool detectp = 1; // detect (
            devices.clear();
            size.clear();
            while (iss >> all_devices)
            {
                if (all_devices[0] == '(')
                {
                    all_devices = all_devices.substr(1, all_devices.length() - 1);
                    istringstream ss(all_devices);
                    ss >> wh;           // 將 all_devices 轉換為 double
                    size.push_back(wh); // 添加到 t->wh 中
                    i++;
                    break;
                }
                devices.push_back(all_devices);
                this->MToB[all_devices] = name;
            }
            t->name = name;
            t->contents = devices;
            while (iss >> number)
            {
                if (i == 0)
                {
                    number = number.substr(1, number.length() - 1);
                }
                // else if (i == 3)
                // {
                //     number = number.substr(0, number.length() - 1);
                // }
                istringstream ss(number);
                ss >> wh;           // 將 number 轉換為 double
                size.push_back(wh); // 添加到 t->wh 中
                if (i == 3)
                {
                    j++;
                    t->wh.push_back(size);
                    size.clear();
                }
                i++;
                i = i % 4;
            }
            t->num = j;
            this->BBF[name] = t; // create BBF
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool is_digit(const string &str)
{
    // Check if the string is a valid double
    istringstream iss(str);
    double val;
    iss >> noskipws >> val;          // noskipws considers leading whitespace invalid
    return iss.eof() && !iss.fail(); // Check if the entire string was consumed and is valid
}

void Placement::RatioInput()
{
    istringstream iss(expected_ratio_str);
    iss >> ratio;
    // ifstream in(expected_ratio_str);
    // string s;

    // if (!in)
    // {
    //     cout << ".txt can't be open !" << endl;
    //     return;
    // }

    // ratio = 0.0;
    // bool found_ratio = false;

    // while (getline(in, s))
    // {
    //     string t;
    //     istringstream iss(s);
    //     while (iss >> t)
    //     {
    //         if (is_digit(t))
    //         {
    //             ratio = stod(t); // Convert the string to a double
    //             found_ratio = true;
    //             break; // If we found a valid ratio, break the inner loop
    //         }
    //     }
    //     if (found_ratio)
    //     {
    //         break; // If we found a valid ratio, break the outer loop
    //     }
    // }
    // cout << "ratio: " << ratio << endl;

    // if (found_ratio)
    // {
    //     cout << "ratio ok: " << ratio << endl;
    // }
    // else
    // {
    //     cout << "ratio not found in the file!" << endl;
    // }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void Placement::NetlistOutput()
{
    cout << ".netlist" << endl;
    map<string, vector<string>>::iterator iter;

    for (iter = this->mos_map.begin();
         iter != this->mos_map.end(); iter++)
    {
        cout << iter->first << endl;
        for (int i = 0; i < iter->second.size(); i++)
        {
            cout << iter->second[i] << " ";
        }
        cout << endl;
    }
    cout << "-----------------------------------------------------------------------------------\n";
}

void Placement::NetOutput()
{
    cout << ".netlist just net and its mos" << endl;
    for (const auto &pair : this->net)
    { // auto to move class
        cout << pair.first << endl;
        for (const string &mos : pair.second)
        { // print mos according to net
            cout << mos << " ";
        }
        cout << endl;
    }
    cout << "-----------------------------------------------------------------------------------\n";
    cout << "nmos or pmos" << endl;
    for (const auto &mos : this->npmos)
    {
        cout << mos.first << endl;
        for (const string &c : mos.second)
        {
            cout << c << " ";
        }
        cout << endl;
    }
    cout << "-----------------------------------------------------------------------------------\n";
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Placement::SymmetryOutput()
{
    cout << ".sym" << endl;

    for (size_t i = 0; i < sym_map.size(); ++i)
    {
        cout << sym_map[i]->index << "\t";
        for (size_t j = 0; j < sym_map[i]->sym.size(); j++)
            cout << sym_map[i]->sym[j] << " ";
        cout << endl;
    }
    cout << "-----------------------------------------------------------------------------------\n";
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void Placement::BuildingOutput()
{
    cout << ".block" << endl;

    // InBlock *i = BBF[1];
    map<string, InBlock *>::iterator iter;
    for (const auto &iter : BBF)
    {
        InBlock *i = iter.second; // Get the pointer to InBlock
        cout << i->name << " " << i->num << endl;
        for (const string &j : i->contents) // Use const reference to avoid unnecessary copies
        {
            cout << j << " ";
        }
        cout << endl; // End the line after contents

        for (const auto &row : i->wh)
        {
            for (double element : row)
                cout << element << " ";
            cout << endl; // End the line after each row
        }
        cout << endl; // Separate InBlocks
    }

    cout << "-----------------------------------------------------------------------------------\n";
}

void Placement::MToBOutput()
{
    map<string, string>::iterator iter;
    for (const auto &iter : this->MToB)
    {
        cout << iter.first << " " << iter.second << endl;
    }
}

string formatDouble(double value)
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(16) << value;
    std::string str = out.str();

    // Remove trailing zeros
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    // If the string ends with a decimal point, remove it
    if (str.back() == '.')
    {
        str.pop_back();
    }
    return str;
}

void Placement::AnsOutput(const TreeNode *trees, const double max_x, const double max_y, const double HPWL)
{
    ofstream out(output_str);
    string s;

    if (!out)
        cout << "Output file can't be open!" << endl;
    else
    {
        out << formatDouble(HPWL) << endl;
        out << formatDouble(max_x * max_y) << endl;
        out << formatDouble(max_x) << " " << formatDouble(max_y) << endl;
        printTree(trees, out);
    }
}

void Placement::printTree(const TreeNode *root, ofstream &out)
{
    if (root == nullptr)
        return;
    for (const auto &i : trees_map)
    {
        out << i.first << " ";
        for (const auto &m : i.second->block.contents) // all mos in this block
        {
            out << m << " ";
        }
        for (const auto &value : i.second->xy)
        {
            out << value << " ";
        }
        // cout << "map value ok" << endl;
        out << "(";
        bool isF = 1;
        for (const auto &value : i.second->block.wh[i.second->block.size_pick])
        {
            if (isF)
                out << value;
            else
                out << " " << value;
            isF = 0;
        }
        // cout << "map width ok" << endl;
        out << ")\n";
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

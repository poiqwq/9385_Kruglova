#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <cstdlib>

using namespace std;
int LEVEL = 10;
int TIMES = 10;
int VER1 = 8;
int VER2 = 15;

class Vertex
{
public:
    Vertex(int index) // конструктор класса
    {
        set_vertex(index);
    }
    unordered_map<int, float> parents;
    vector<int> parents_idx;
    unordered_map<int, float> children;
    vector<int> children_idx;
    void set_vertex(int index)
    {
        idx = index;
    }
    void add_parent(int parent_index, float parent_weight)
    {
        parents.insert({parent_index, parent_weight});
        parents_idx.push_back(parent_index);
    }
    void add_child(int child_index, float child_weight)
    {
        children.insert({ child_index, child_weight });
        children_idx.push_back(child_index);
    }

private:
    int idx;
};

bool if_not_exists_diagramm(vector<int> new_diagramm, vector<vector<int>> flore_list, int cur_index, int index_prev_level);
void add_vertex(int new_index, int cur_index);

vector<Vertex> vertexList;

int main()
{
    ofstream outf(to_string(VER1) + "to" + to_string(VER2) + ".txt");
    if (!outf)
	{
		// то выводим сообщение об ошибке и выполняем функцию exit()
		cerr << "Uh oh, SomeText.txt could not be opened for writing!" << endl;
		exit(1);
	}
	outf << "Здесь будут вероятности перехода, посчитанные для ребра "
	<< VER1 << " -- " << VER2 << " при количестве этажей -- " << LEVEL << endl;

    cout << 1 << endl;
    vector<vector<vector<int>>> jung_list;
    vector<vector<int>> prev_flore_list;
    vector<vector<int>> flore_list;
    vector<int> curr_diagramm;
    jung_list.push_back({{0}});
    jung_list.push_back({{1}});

    vertexList.push_back(Vertex(0));
    int index_prev_level = 1;
    int cur_index = 1;
    int new_index = 1;
    vertexList.push_back(Vertex(cur_index));

    for (int level = 1; level < LEVEL; level++) // По уровням
    {
        cout << "1 " << jung_list.at(level).size() << endl;
        prev_flore_list = jung_list.at(level);
        for (int k = 0; k < prev_flore_list.size(); k++) // По диаграммам на уровне
        {
            cout << "2 " << prev_flore_list.at(k).at(0) << endl;
            curr_diagramm = prev_flore_list.at(k);
            vector<int> new_diagramm(curr_diagramm.begin(), curr_diagramm.end());
            new_diagramm.push_back(1);
            if (if_not_exists_diagramm(new_diagramm, flore_list, cur_index, index_prev_level))
            {
                new_index++;
                add_vertex(new_index, cur_index);
                flore_list.push_back(new_diagramm);
                cout << "index " << new_index << endl;
            }

            for (int i = 0; i < curr_diagramm.size(); i++) // По столбцам диаграммы
            {
                cout << "3 " << curr_diagramm.at(i) << endl;
                int cur_elem = curr_diagramm.at(i);
                if (i > 0)
                {
                    if (curr_diagramm.at(i - 1) == cur_elem) { continue; }
                }
                vector<int> new_diagramm(curr_diagramm.begin(), curr_diagramm.end());
                new_diagramm.at(i) = cur_elem + 1;
                if (if_not_exists_diagramm(new_diagramm, flore_list, cur_index, index_prev_level))
                {
                    new_index++;
                    add_vertex(new_index, cur_index);
                    flore_list.push_back(new_diagramm);
                    cout << "new_index " << new_index << endl;
                }



            }
            cur_index++;
        }
    jung_list.push_back(flore_list);
    flore_list.clear();
    index_prev_level = new_index;
    }







//    ifstream in("hello.txt");
//    if (!in)
//    {
//        cerr << "Uh oh, hello.txt could not be opened for reading!" << endl;
//        exit(1);
//    }



//    string line;
//    getline(in, line);
//    int n = stoi(line); // Количество циклов центрирования
//    getline(in, line);
//    int lastPick = stoi(line); // Последняя рассматриваемая вершина
//
//    for (int i = 0; i < lastPick + 1; i++)
//    {
//        vertexList.push_back(Vertex(i));
//    }
//
//    while (getline(in, line))
//    {
//        stringstream ss(line);
//        string substring;
//
//        getline(ss, substring, ' '); // Индекс вершины-родителя
//        int firstPick = stoi(substring);
//        getline(ss, substring, ' '); // Индекс вершины-ребенка
//        int secondPick = stoi(substring);
//        getline(ss, substring, ' '); // Вероятность перехода
//        float weight = stof(substring);
//
//        vertexList[firstPick].add_child(secondPick, weight);
//        vertexList[secondPick].add_parent(firstPick, weight);
//    }
//    in.close();

    vector<vector<vector<int>>> rombsLists; // Вектор ромбов, образующихся для вершин-родителей
    for (int i = 1; i < vertexList.size(); i++)
    {
        vector<int> level0Children = vertexList.at(i).children_idx; // Дети рассматриваемой вершины
        vector<vector<int>> level1Children; // Массив детей для каждого ребенка
        for (auto& idx : level0Children)
        {
            vector<int> level1ChildrenIdx = vertexList.at(idx).children_idx;
            level1Children.push_back(level1ChildrenIdx);
        }

        // Поиск ромбов
        for (int k = 0; k < level0Children.size(); k++)
        {
            // Переопределим вероятности

            int sisse = level0Children.size();
            float pk = 1.0 / sisse;
            cout << i << " new_index " << level0Children.at(k) << " " << pk << endl;
            vertexList.at(i).children.at(level0Children.at(k)) = pk;
            vertexList.at(level0Children.at(k)).parents.at(i) = pk;
            //
            vector<vector<int>> rombs = {};
            for (auto& currentChild : level1Children.at(k))
            {
                for (int j = k+1; j < level1Children.size(); j++)
                {
                    for (auto& idx : level1Children.at(j))
                    {
                        // У родителей общий ребенок
                        if (currentChild == idx)
                        {
                            if (level0Children.at(k) < level0Children.at(j))
                            {
                                vector<int> romb = {i, level0Children.at(k), currentChild, level0Children.at(j)};
                                rombs.push_back(romb);
                            }
                            else
                            {
                                vector<int> romb = {i, level0Children.at(j), currentChild, level0Children.at(k)};
                                rombs.push_back(romb);
                            }
                            break;
                        }
                    }
                }
            }
            if (rombs.size())
            {
                rombsLists.push_back(rombs);
                rombs.clear();
            }

        }
    }

    for (int times = 0; times < TIMES; times++)
    {
        // Пройдемся по всем ребрам
        for (const vector<vector<int>>& rombs : rombsLists)
        {
            int pick;
            float pickSum = 0;

            for (const vector<int>& romb : rombs)
            {
                for (int value : romb) { cout << value << " "; }
                cout << endl;
            }

            for (const vector<int>& romb : rombs)
            {
                // Центрирование
                pick = romb.at(0);
                cout << pick << " to " << romb.at(1) << " weight " << vertexList.at(romb.at(0)).children.at(romb.at(1)) << endl;
                cout << pick << " to " << romb.at(3) << " weight " << vertexList.at(romb.at(0)).children.at(romb.at(3)) << endl;
                cout << romb.at(1) << " to " << romb.at(3) << " weight " << vertexList.at(romb.at(1)).children.at(romb.at(2)) << " to " << vertexList.at(romb.at(2)).parents.at(romb.at(3)) << endl;

                float p3 = (vertexList.at(romb.at(0)).children.at(romb.at(1)) *
                vertexList.at(romb.at(1)).children.at(romb.at(2))) /
                vertexList.at(romb.at(2)).parents.at(romb.at(3));
                vertexList.at(romb.at(0)).children.at(romb.at(3)) = p3;
                vertexList.at(romb.at(3)).parents.at(romb.at(0)) = p3;
                cout << pick << " to " << romb.at(3) << " central_weight " << p3 << endl;
            }
            // Нормирование
            for (int oval = 0; oval < vertexList.at(pick).children_idx.size(); oval++)
            {
                pickSum += vertexList.at(pick).children.at(vertexList.at(pick).children_idx.at(oval));
                cout << pick << " to " << vertexList.at(pick).children_idx.at(oval) << " sum " << pickSum << endl;

            }
            for (int j = 0; j < vertexList.at(pick).children_idx.size(); j++)
            {
                int childIdx = vertexList.at(pick).children_idx.at(j);
                float normal = vertexList.at(pick).children.at(childIdx) / pickSum;
                vertexList.at(pick).children.at(childIdx) = normal;
                vertexList.at(childIdx).parents.at(pick) = normal;
                cout << pick << " to " << vertexList.at(pick).children_idx.at(j) << " normal " << normal << endl;
            }
        }
        cout << endl;
        outf << vertexList.at(VER1).children.at(VER2) << endl;
        cout << vertexList.at(VER1).children.at(VER2) << endl;
        cout << endl;
    }
    outf.close();

    return 0;
}


void add_vertex(int new_index, int cur_index)
{
    vertexList.push_back(Vertex(new_index));
    vertexList.at(cur_index).add_child(new_index, 0.5);
    vertexList.at(new_index).add_parent(cur_index, 0.5);
}

bool if_not_exists_diagramm(vector<int> new_diagramm, vector<vector<int>> flore_list, int cur_index, int index_prev_level)
{
    if (new_diagramm.size() == 1)
    {
        return true;
    }
    for (int d = 0; d < flore_list.size(); d++)
    {
        cout << "d " << flore_list.size() << endl;
        int count = 0;
        if (flore_list.at(d).size() == new_diagramm.size())
        {
            cout << "dd " << new_diagramm.size() << endl;
            for (int m = 0; m < new_diagramm.size(); m++)
            {
                cout << "m " << new_diagramm.size() << endl;
                if (flore_list.at(d).at(m) != new_diagramm.at(m))
                {
                    break;
                }
                else { count++; }
            }
            if (count == new_diagramm.size())
            {
                cout << "sticked " << new_diagramm.size() << endl;
                vertexList.at(cur_index).add_child(index_prev_level + d + 1, 0.5);
                vertexList.at(index_prev_level + d + 1).add_parent(cur_index, 0.5);
                return false;
            }

        }
    }
    return true;
}
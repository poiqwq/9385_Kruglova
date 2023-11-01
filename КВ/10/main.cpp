#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

using namespace std;

class Vertex
{
public:
    Vertex(int index) // конструктор класса
    {
        setVertex(index);
    }
    unordered_map<int, float> parents;
    vector<int> parentsIdx;
    unordered_map<int, float> children;
    vector<int> childrenIdx;
    void setVertex(int index)
    {
        idx = index;
    }
    void addParent(int parentIndex, float parentWeight)
    {
        parents.insert({ parentIndex, parentWeight });
        parentsIdx.push_back(parentIndex);
    }
    void addChild(int childIndex, float childWeight)
    {
        children.insert({ childIndex, childWeight });
        childrenIdx.push_back(childIndex);
    }

private:
    int idx;
};

int main()
{
    ifstream in("hello.txt");
    if (!in)
    {
        cerr << "Uh oh, hello.txt could not be opened for reading!" << endl;
        exit(1);
    }

    vector<Vertex> vertexList;

    string line;
    getline(in, line);
    int n = stoi(line); // Количество циклов центрирования
    getline(in, line);
    int lastPick = stoi(line); // Последняя рассматриваемая вершина

    for (int i = 0; i < lastPick + 1; i++)
    {
        vertexList.push_back(Vertex(i));
    }

    while (getline(in, line))
    {
        stringstream ss(line);
        string substring;

        getline(ss, substring, ' '); // Индекс вершины-родителя
        int firstPick = stoi(substring);
        getline(ss, substring, ' '); // Индекс вершины-ребенка
        int secondPick = stoi(substring);
        getline(ss, substring, ' '); // Вероятность перехода
        float weight = stof(substring);

        vertexList[firstPick].addChild(secondPick, weight);
        vertexList[secondPick].addParent(firstPick, weight);
    }
    in.close();

    vector<vector<vector<int>>> rombsLists; // Вектор ромбов, образующихся для вершин-родителей
    for (int i = 0; i < lastPick + 1; i++)
    {
        vector<int> level0Children = vertexList.at(i).childrenIdx; // Дети рассматриваемой вершины
        vector<vector<int>> level1Children; // Массив детей для каждого ребенка
        for (auto& idx : level0Children)
        {
            vector<int> level1ChildrenIdx = vertexList.at(idx).childrenIdx;
            level1Children.push_back(level1ChildrenIdx);
        }

        // Поиск ромбов
        for (int k = 0; k < level0Children.size(); k++)
        {
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

    for (int times = 0; times < n; times++)
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
                float p3 = vertexList.at(romb.at(0)).children.at(romb.at(1)) *
                vertexList.at(romb.at(1)).children.at(romb.at(2)) /
                vertexList.at(romb.at(2)).parents.at(romb.at(3));
                vertexList.at(romb.at(0)).children.at(romb.at(3)) = p3;
                cout << pick << " to " << romb.at(3) << " central_weight " << p3 << endl;
            }
            // Нормирование
            for (int oval = 0; oval < vertexList.at(pick).childrenIdx.size(); oval++)
            {
                pickSum += vertexList.at(pick).children.at(vertexList.at(pick).childrenIdx.at(oval));
                cout << pick << " to " << vertexList.at(pick).childrenIdx.at(oval) << " sum " << pickSum << endl;

            }
            for (int j = 0; j < vertexList.at(pick).childrenIdx.size(); j++)
            {
                int childIdx = vertexList.at(pick).childrenIdx.at(j);
                float normal = vertexList.at(pick).children.at(childIdx) / pickSum;
                vertexList.at(pick).children.at(childIdx) = normal;
                cout << pick << " to " << vertexList.at(pick).childrenIdx.at(j) << " normal " << normal << endl;
            }
        }
        cout << endl;
        cout << vertexList.at(2).children.at(5) << endl;
        cout << endl;
    }

    return 0;
}



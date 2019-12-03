#include <bits/stdc++.h>
using namespace std;
#define INF 0x3f3f3f3f



class Graph
{
    int V;
    list<pair<int, int>> *adj;

  public:
    Graph(int V);


    void addEdge(int u, int v, int w);


    vector<int> shortestPath(int s);
};

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<pair<int, int>>[V];
}

void Graph::addEdge(int u, int v, int w)
{
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w));
}


vector<int> Graph::shortestPath(int src)
{

    set<pair<int, int>> setds;


    vector<int> dist(V, INF);


    setds.insert(make_pair(0, src));
    dist[src] = 0;


    while (!setds.empty())
    {

        pair<int, int> tmp = *(setds.begin());
        setds.erase(setds.begin());


        int u = tmp.second;


        list<pair<int, int>>::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
        {

            int v = (*i).first;
            int weight = (*i).second;


            if (dist[v] > dist[u] + weight)
            {

                if (dist[v] != INF)
                    setds.erase(setds.find(make_pair(dist[v], v)));


                dist[v] = dist[u] + weight;
                setds.insert(make_pair(dist[v], v));
            }
        }
    }

    return dist;
}


void display(vector<int> d)
{
    for (int i = 0; i < d.size(); i++)
    {
        cout << i << "\t\t" << d[i] << endl;
    }
}

vector<int> d;
pair<int, int> tmp;

class Truck
{
  public:
    int fuel, cap, capDefault, price, fuelDefault;
    int trucksUsed = 0, travelled = 0, p = 0, c = 0;

    Truck(int f, int c, int p)
    {
        fuel = fuelDefault = f;
        cap = capDefault = c;
        price = p;
    }


    pair<int, int> closestCargo(vector<int> flag, vector<int> d)
    {
        pair<int, int> min = make_pair(0, INF);
        for (int i = 0; i < d.size(); i++)
        {
            if (flag[i])
            {
                if (d[i] < min.second)
                    min = make_pair(i, d[i]);
            }
        }
        return min;
    }


    int pick(pair<int, int> tmp, vector<int> &flag)
    {
        travelled += tmp.second;
        fuel -= tmp.second;

        if (cap >= flag[tmp.first])
        {
            ++p;
            cap -= flag[tmp.first];
            flag[tmp.first] = 0;
        }
        else
        {
            flag[tmp.first] -= cap;
            cap = 0;
        }
        return tmp.first;
    }


    int drop(int src, int des, Graph &g)
    {
        d = g.shortestPath(src);
        travelled += d[des];
        c += p;
        p = 0;
        return -1;
    }

    int start(int src, vector<int> &flag, Graph &g)
    {
        d = g.shortestPath(src);
        tmp = closestCargo(flag, d);
        trucksUsed += 1;
        fuel = fuelDefault;
        cap = capDefault;
        return pick(tmp, flag);
    }

    int check(int src, int des, vector<int> &flag, Graph &g)
    {
        d = g.shortestPath(src);
        tmp = closestCargo(flag, d);
        int weight = tmp.second;
        d = g.shortestPath(tmp.first);
        weight += d[des];
        if (fuel >= weight && cap > 0)
            return pick(tmp, flag);
        else
            return drop(src, des, g);
    }
};


int minFuel(int src, int des, vector<int> &flag, Graph &g)
{
    vector<int> d1;
    d = g.shortestPath(src);
    int weight = 0;
    for (int i = 0; i < flag.size(); i++)
    {
        if (flag[i])
        {
            d1 = g.shortestPath(i);
            if (d[i] + d1[des] > weight)
                weight = d[i] + d1[des];
        }
    }
    return weight;
}

int main()
{

    int V = 9;
    Graph g(V);


    g.addEdge(0, 1, 4);
    g.addEdge(0, 7, 8);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 7, 11);
    g.addEdge(2, 3, 7);
    g.addEdge(2, 8, 2);
    g.addEdge(2, 5, 4);
    g.addEdge(3, 4, 9);
    g.addEdge(3, 5, 14);
    g.addEdge(4, 5, 10);
    g.addEdge(5, 6, 2);
    g.addEdge(6, 7, 1);
    g.addEdge(6, 8, 6);
    g.addEdge(7, 8, 7);


    vector<int> flag(V, 0);
    flag[1] = flag[7] = flag[6] = flag[5] = 9;
    int nCargo = 4, src = 0, des = 4, u = 0, v = 0;

    int fuel = minFuel(src, des, flag, g);
    cout << "Minimum fuel per truck for this graph is: " << fuel << endl
         << endl;


    Truck small(fuel, 3, 500);
    Truck med(fuel * 1.3, 6, 1500);
    Truck large(fuel * 1.7, 9, 2500);
    Truck xl(fuel * 2, 15, 4500);
    Truck arr[] = {small, med, large, xl};
    string name[] = {"small", "Mid-Sized", "large", "Xtra-Large"};

    for (int i = 0; i < 4; i++)
    {

        flag[1] = flag[7] = flag[6] = flag[5] = 9;

        while (arr[i].c != nCargo)
        {
            u = arr[i].start(src, flag, g);
            v = arr[i].check(u, des, flag, g);
            while (v != -1)
                v = arr[i].check(v, des, flag, g);
        }
        cout << "For " << name[i] << " truck" << endl;
        cout << "---------------------------------------------------------" << endl;
        cout << "distance covered: \t\t" << arr[i].travelled << endl;
        cout << "Trucks Used: \t" << arr[i].trucksUsed << "\t\t"
             << "Hiring Cost: \t$" << (arr[i].price * arr[i].trucksUsed) << endl;
        cout << "Fuel Used: \t" << arr[i].travelled << "\t\t"
             << "Fuel Cost: \t$" << (arr[i].travelled * 50) << endl;
        cout << "\t\t\t\t"
             << "Total Cost: \t$" << (arr[i].price * arr[i].trucksUsed) + (arr[i].travelled * 50) << endl;
        cout << "---------------------------------------------------------" << endl;
        cout << endl
             << endl;
    }

    return 0;
}

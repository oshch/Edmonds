#include <iostream>
#include <queue>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

const int INF = 1e9;

class Graph {

public:
    struct Edge {
        int reverse_id;
        int capacity;
        int flow;
        int from,to;
        Edge(){}
        Edge(int _from, int _to, int _capacity, int _reverse_id):
            from(_from),
            to(_to),
            capacity(_capacity),
            flow(0),
            reverse_id(_reverse_id) {};
    };

    vector< vector<Edge*> > edges;


    Graph(int n) {
        edges.assign(n, vector<Edge*>());
    }

    void addEdge(int from, int to, int capacity) {
        edges[from].push_back(new Edge(from, to, capacity, -1));
        for (int j = 0; j < edges[to].size(); j++) {
            if (edges[to][j]->to == from) {
                edges[to][j]->reverse_id = edges[from].size() - 1;
                edges[from][edges[from].size() - 1]->reverse_id = j;
                break;
            }
        }

        if (edges[from].back()->reverse_id == -1) {
                    edges[to].push_back(new Edge(to, from, 0, edges[from].size() - 1));
                    edges[from][edges[from].size() - 1]->reverse_id = edges[to].size() - 1;
                }

    }


    void bfs(int source, int dest, vector< vector<Edge*> >& edges, vector<Edge*>& p) {
        queue<int> q;
        q.push(source);
        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            for (int i = 0; i < edges[curr].size(); i++) {
                if (p[ edges[curr][i] -> to ] == NULL &&
                     edges[curr][i]->to != source &&
                     edges[curr][i]->capacity > edges[curr][i]->flow &&
                     edges[curr][i]->capacity > 0) {
                        p[edges[curr][i]->to] = edges[curr][i];
                        q.push(edges[curr][i]->to);

                }
            }
            if (p[dest] != NULL)
                break;
        }
    }


    int findFlow(int source, int dest) {
        int flow = 0;
        vector<Edge*> p(edges.size());
        while (true) {
            for (int i = 0; i < p.size(); i++)
                p[i] = NULL;
            bfs(source, dest, edges, p);
            if (p[dest] == NULL) {
                return flow;
            }
            int df = INF;
            for (Edge* e = p[dest]; e != NULL; e = p[e->from]) {
                df = min(df, e->capacity - e->flow);
            }
            for (Edge* e = p[dest]; e != NULL; e = p[e->from]) {
                e->flow += df;

                edges[e->to][e->reverse_id]->flow -= df;
            }
            flow += df;
        }
    }

};
int main()
{
    freopen("input.txt", "rt", stdin);
    int n, m, x, y, z;
    cin >> n >> m;
    Graph gr(n);
    for (int i = 0; i < m; i++) {
        cin >> x >> y >> z;
        gr.addEdge(x - 1, y - 1, z);
    }


    cout << gr.findFlow(0, n - 1);
    return 0;
}

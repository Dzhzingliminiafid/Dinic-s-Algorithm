#include <bits/stdc++.h>

#define MAX_VERTICES 1000

using namespace std;

// Структура для представления ребра между двумя вершинами
struct Edge {
    int v;            // Вершина v
    int flow;         // Поток данных в ребре
    int capacity;     // Пропускная способность
    int reverse_edge; // Индекс обратного ребра в списке смежности
};

// Структура для представления вершины в графе
struct Vertex {
    vector<Edge*> edges; // Массив ребер
};

// Структура для представления графа
struct Graph {
    int V; // Число вершин
    vector<Vertex> vertices; // Массив вершин
};

// Создание ребра
Edge* createEdge(int v, int capacity) {
    Edge* edge = new Edge();
    edge->v = v;
    edge->flow = 0;
    edge->capacity = capacity;
    edge->reverse_edge = -1; // Изначально установлено в -1
    return edge;
}

// Добавление ребра в граф
void addEdge(Graph& graph, int u, int v, int capacity) {
    // Прямое ребро
    Edge* forward_edge = createEdge(v, capacity);
    graph.vertices[u].edges.push_back(forward_edge);

    // Обратное ребро (реверс ребро)
    Edge* backward_edge = createEdge(u, 0); // Пропускная способность обратного ребра равна 0
    graph.vertices[v].edges.push_back(backward_edge);

    // Обновление указателей на обратное ребро
    forward_edge->reverse_edge = v;
    backward_edge->reverse_edge = u;
}

// Поиск в ширину для проверки существования пути от источника к стоку
bool BFS(Graph& graph, int source, int sink, vector<int>& parent) {
    vector<bool> visited(MAX_VERTICES, false);
    visited[source] = true;
    parent[source] = -1;

    // Создание очереди для BFS
    queue<int> q;
    q.push(source);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto edge : graph.vertices[u].edges) {
            int v = edge->v;
            if (!visited[v] && edge->flow < edge->capacity) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return visited[sink];
}

// Реализация алгоритма Диница для поиска максимального потока
int dinicsMaxFlow(Graph& graph, int source, int sink) {
    int max_flow = 0;
    vector<int> parent(MAX_VERTICES);

    // Пока существует путь от источника к стоку
    while (BFS(graph, source, sink, parent)) {
        int path_flow = INT_MAX;

        // Нахождение минимальной остаточной пропускной способности на пути
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            for (auto edge : graph.vertices[u].edges) {
                if (edge->v == v) {
                    path_flow = min(path_flow, edge->capacity - edge->flow);
                    break;
                }
            }
        }

        // Обновление потока вдоль пути
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            for (auto edge : graph.vertices[u].edges) {
                if (edge->v == v) {
                    edge->flow += path_flow;
                    // Обновление обратного ребра
                    for (auto rev_edge : graph.vertices[v].edges) {
                        if (rev_edge->v == u) {
                            rev_edge->flow -= path_flow;
                            break;
                        }
                    }
                    break;
                }
            }
        }

        // Добавление потока пути к общему потоку
        max_flow += path_flow;
    }

    return max_flow;
}

int main() {
    int V, E;
    cin >> V >> E;

    cout << "\n" << "Graph with " << V << " vertices and " << E << " edges" << endl;
    Graph graph;
    graph.V = V;
    graph.vertices.resize(V);
    for (int i = 0; i < E; i++) {
        int u, v, capacity;
        cin >> u >> v >> capacity;
        addEdge(graph, u, v, capacity);
    }
    // Нахождение вершин источника и стока
    int source = 0; // Вершина-источник
    int sink = graph.V - 1; // Вершина-сток (последняя вершина)

    // Нахождение и вывод максимального потока
    cout << "Max Flow: " << dinicsMaxFlow(graph, source, sink) << endl;
}

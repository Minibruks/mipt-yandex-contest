#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <map>
#include <set>

struct Edge {
    uint64_t from;
    uint64_t to;
};

class Graph {
protected:
    uint64_t vertex_degree = 0;

public:
    [[nodiscard]] uint64_t getVertexDegree() const {
        return vertex_degree;
    }

    [[nodiscard]] virtual std::multiset<uint64_t> getNeighbors(const uint64_t& v) const = 0;

    virtual void addEdge(const uint64_t& source, const uint64_t& dest) = 0;

    [[nodiscard]] virtual bool checkMulti(const uint64_t from, const uint64_t to) const = 0;
};

class GraphList final : public Graph {
private:
    std::unordered_map<uint64_t, std::multiset<uint64_t>> adjacency_list;
public:
    explicit GraphList(const uint64_t& vertex_number) {
        vertex_degree = vertex_number;
        for (uint64_t i = 0; i < vertex_number; i++) {
            adjacency_list[i] = std::multiset<uint64_t>();
        }
    }

    [[nodiscard]] std::multiset<uint64_t> getNeighbors(const uint64_t& v) const override {
        return adjacency_list.at(v);
    }

    void addEdge(const uint64_t& source, const uint64_t& dest) override {
        adjacency_list[source].insert(dest);
        adjacency_list[dest].insert(source);
    }

    [[nodiscard]] bool checkMulti(const uint64_t from, const uint64_t to) const override {
        if (adjacency_list.at(from).count(to) == 1) {
            return true;
        }
        return false;
    }
};

std::pair<uint64_t, uint64_t> returnEdge(uint64_t from, uint64_t to) {
    if (from > to) {
        std::swap(from, to);
    }
    return std::make_pair(from, to);
}

void dfs(const Graph& graph, uint64_t current_vertex, std::vector<bool>& used, std::vector<uint64_t>& time_up, std::vector<uint64_t>& time_in, int& time, uint64_t root = -1) {
    used[current_vertex] = true;
    time_in[current_vertex] = time;
    time_up[current_vertex] = time;
    time++;

    std::multiset<uint64_t> neighbors = graph.getNeighbors(current_vertex);

    for (auto& neighbor: neighbors) {
        if (neighbor == root) {
            continue;
        }
        if (used[neighbor]) {
            time_up[current_vertex] = std::min(time_up[current_vertex], time_in[neighbor]);
        } else {
            dfs(graph, neighbor, used, time_up, time_in, time, current_vertex);
            time_up[current_vertex] = std::min(time_up[current_vertex], time_up[neighbor]);
        }
    }
}

void paint(const Graph& graph, uint64_t current_vertex, uint64_t color, std::vector<uint64_t>& colors, uint64_t& max_color, std::vector<uint64_t>& time_up,
           std::vector<uint64_t>& time_in, std::vector<std::vector<uint64_t>>& components) {
    colors[current_vertex] = color;
    components[color - 1].push_back(current_vertex);
    std::multiset<uint64_t> neighbors = graph.getNeighbors(current_vertex);

    for (auto& neighbor: neighbors) {
        if (colors[neighbor] == 0) {
            if (time_up[neighbor] > time_in[current_vertex]) {
                max_color++;
                paint(graph, neighbor, max_color, colors, max_color, time_up, time_in, components);
            } else {
                paint(graph, neighbor, color, colors, max_color, time_up, time_in, components);
            }
        }
    }
}

bool checkNeighbor(std::vector<uint64_t>& comp, uint64_t current_vertex) {
    for (auto& i: comp) {
        if (i == current_vertex) {
            return true;
        }
    }
    return false;
}

void findBridges(const Graph& graph) {
    std::vector<bool> used(graph.getVertexDegree(), false);
    std::vector<uint64_t> time_up(graph.getVertexDegree() + 1);
    std::vector<uint64_t> time_in(graph.getVertexDegree() + 1);
    std::vector<uint64_t> colors(graph.getVertexDegree() + 1, 0);
    std::vector<std::vector<uint64_t>> components(graph.getVertexDegree(), std::vector<uint64_t>());
    uint64_t max_color = 0;
    int leaves_count = 0;

    int time = 1;

    for (auto i = 0; i < graph.getVertexDegree(); i++) {
        if (!used[i]) {
            dfs(graph, i, used, time_up, time_in, time);
        }
    }

    for (auto i = 0; i < graph.getVertexDegree(); i++) {
        if (colors[i] == 0) {
            max_color++;
            paint(graph, i, max_color, colors, max_color, time_up, time_in, components);
        }
    }
    int bridge_count = 0;
    bool flag = true;
    for (auto& component: components) { // прохожусь по каждой компоненте
        if (component.empty()) {
            continue;
        }
        bridge_count = 0;
        for (auto vertex = 0; vertex < component.size(); vertex++) { // прохожусь по всем вершинам из одной компоненты
            std::multiset<uint64_t> neighbors = graph.getNeighbors(component[vertex]); // получаю соседей текущей вершины
            for (auto& neighbor: neighbors) { // прохожусь по соседям
                flag = true;
                if (!checkNeighbor(component, neighbor)) { // если сосед лежит не в компоненте то это мост
                    flag = false;
                }
                if (!flag) {
                    bridge_count++;
                }
            }
        }
        if (bridge_count == 1) {
            leaves_count++;
        }
    }
    if (leaves_count % 2 == 0) {
        std::cout << leaves_count / 2;
    } else {
        std::cout << leaves_count / 2 + 1;
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint64_t vertex = 0, edges = 0;
    std::cin >> vertex >> edges;

    GraphList graph(vertex);

    for (uint64_t i = 0; i < edges; i++) {
        uint64_t a = 0, b = 0;
        std::cin >> a >> b;
        graph.addEdge(a - 1, b - 1);
    }

    findBridges(graph);

    return 0;
}
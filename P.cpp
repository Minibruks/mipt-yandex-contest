#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>

enum {
    ORIENTED = true,
    NOT_ORIENTED = false
};

class Graph {
protected:
    uint64_t vertex_degree = 0;
    bool oriented;

public:
    [[nodiscard]] uint64_t getVertexDegree() const {
        return vertex_degree;
    }

    [[nodiscard]] virtual std::vector<uint64_t> getNeighbors(const uint64_t& v) const = 0;

    virtual void addEdge(const uint64_t& source, const uint64_t& dest) = 0;
};

class GraphList final : public Graph {
private:
    std::unordered_map<uint64_t, std::vector<uint64_t>> adjacency_list;
public:
    explicit GraphList(const uint64_t& vertex_number, bool oriented_status) {
        vertex_degree = vertex_number;
        for (uint64_t i = 0; i < vertex_number; i++) {
            adjacency_list[i] = std::vector<uint64_t>();
        }
        oriented = oriented_status;
    }

    [[nodiscard]] std::vector<uint64_t> getNeighbors(const uint64_t& v) const override {
        return adjacency_list.at(v);
    }

    void addEdge(const uint64_t& source, const uint64_t& dest) override {
        adjacency_list[source].push_back(dest);
        if (!oriented) {
            adjacency_list[dest].push_back(source);
        }
    }
};

void dfsGraph(const Graph& graph, uint64_t current_vertex, std::vector<bool>& used, std::vector<uint64_t>& order) {
    used[current_vertex] = true;

    std::vector<uint64_t> neighbors = graph.getNeighbors(current_vertex);
    for (auto& neighbor: neighbors) {
        if (!used[neighbor]) {
            dfsGraph(graph, neighbor, used, order);
        }
    }
    order.push_back(current_vertex);
}

void dfsTransposeGraph(const Graph& graph, uint64_t current_vertex, std::vector<bool>& used, std::vector<uint64_t>& comp) {
    used[current_vertex] = true;
    comp.push_back(current_vertex);

    std::vector<uint64_t> neighbors = graph.getNeighbors(current_vertex);
    for (auto& neighbor: neighbors) {
        if (!used[neighbor]) {
            dfsTransposeGraph(graph, neighbor, used, comp);
        }
    }
}

void findStrongConnectivity(const Graph& graph, const Graph& graph_transpose) {
    std::vector<bool> used(graph.getVertexDegree(), false);
    std::vector<uint64_t> comp;
    std::vector<uint64_t> order;
    std::vector<uint64_t> answer(graph.getVertexDegree());
    int num = 1;

    for (auto i = 0; i < graph.getVertexDegree(); i++) {
        if (!used[i]) {
            dfsGraph(graph, i, used, order);
        }
    }

    used.assign(graph.getVertexDegree(), false);

    for (auto i = 0; i < graph.getVertexDegree(); i++) {
        uint64_t current_vertex = order[graph.getVertexDegree() - i - 1];
        if (!used[current_vertex]) {
            dfsTransposeGraph(graph_transpose, current_vertex, used, comp);
            for (auto& j: comp) {
                answer[j] = num;
            }
            num++;
            comp.clear();
        }
    }

    std::cout << num - 1 << "\n";
    for (int i = 0; i < graph.getVertexDegree(); i++) {
        std::cout << answer[i] << " ";
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint64_t vertex = 0, edges = 0;
    std::cin >> vertex >> edges;

    GraphList graph(vertex, ORIENTED);
    GraphList graph_transpose(vertex, ORIENTED);

    for (uint64_t i = 0; i < edges; i++) {
        uint64_t a = 0, b = 0;
        std::cin >> a >> b;
        graph.addEdge(a - 1, b - 1);
        graph_transpose.addEdge(b - 1, a - 1);
    }

    findStrongConnectivity(graph, graph_transpose);

    return 0;
}
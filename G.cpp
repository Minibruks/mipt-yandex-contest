#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <list>

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
    bool transpose;
public:
    explicit GraphList(const uint64_t& vertex_number, bool transpose_status, bool oriented_status) {
        vertex_degree = vertex_number;
        transpose = transpose_status;
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

    void setNeighbors(const int64_t& current_vertex, const uint64_t& current_height, const uint64_t& current_width, const uint64_t& height, const uint64_t& width, std::vector<uint64_t>& special) {
        uint64_t left_border = width * current_height;
        uint64_t right_border = width * (current_height + 1);

        if (current_vertex - 1 >= left_border && current_vertex - 1 < right_border) {
            if (special[current_vertex - 1] <= special[current_vertex]) {
                if (transpose) {
                    addEdge(current_vertex - 1, current_vertex);
                } else {
                    addEdge(current_vertex, current_vertex - 1);
                }
            }
        }
        if (current_vertex + 1 >= left_border && current_vertex + 1 < right_border) {
            if (special[current_vertex + 1] <= special[current_vertex]) {
                if (transpose) {
                    addEdge(current_vertex + 1, current_vertex);
                } else {
                    addEdge(current_vertex, current_vertex + 1);
                }
            }
        }

        if ((int64_t)(current_vertex - width) >= 0) {
            if (special[current_vertex - width] <= special[current_vertex]) {
                if (transpose) {
                    addEdge(current_vertex - width, current_vertex);
                } else {
                    addEdge(current_vertex, current_vertex - width);
                }
            }
        }
        if (current_vertex + width < height * width) {
            if (special[current_vertex + width] <= special[current_vertex]) {
                if (transpose) {
                    addEdge(current_vertex + width, current_vertex);
                } else {
                    addEdge(current_vertex, current_vertex + width);
                }
            }
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

bool checkNeighbors(const Graph& graph, const uint64_t& vertex, std::vector<uint64_t>& comp) {
    std::vector<uint64_t> neighbors = graph.getNeighbors(vertex);
    if (neighbors.empty()) {
        return true;
    }
    bool res = false;
    for (auto& elem: neighbors) {
        res = false;
        for (auto& comp_elem: comp) {
            if (comp_elem == elem) {
                res = true;
            }
        }
        if (!res) {
            return false;
        }
    }
    return res;
}

void findStrongConnectivity(const Graph& graph, const Graph& graph_transpose) {
    std::vector<bool> used(graph.getVertexDegree(), false);
    std::vector<uint64_t> comp;
    std::vector<uint64_t> order;
    std::vector<uint64_t> answer(graph.getVertexDegree());
    int num = 1;
    int strong_comp = 0;

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
            bool res = true;
//            for (auto& j: comp) {
//                std::cout << j << " ";
//            }
            for (auto& j: comp) {
                //std::vector<uint64_t> neighbors = graph.getNeighbors(j);
                if (!checkNeighbors(graph, j, comp)) {
                    res = false;
                    break;
                }
            }
            if (res) {
                strong_comp++;
            }
            // std::cout << "\n";
            comp.clear();
        }
    }

    std::cout << strong_comp << "\n";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long height, width;
    std::cin >> height >> width;
    std::vector<uint64_t> pipes(height * width);

    GraphList graph(height * width, false, ORIENTED);
    GraphList graph_transpose(height * width, true, ORIENTED);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int square = 0;
            std::cin >> square;
            pipes[i * width + j] = square;
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            graph.setNeighbors(i * width + j, i, j, height, width, pipes);
            graph_transpose.setNeighbors(i * width + j, i, j, height, width, pipes);
        }
    }

    findStrongConnectivity(graph, graph_transpose);
}
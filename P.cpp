#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <queue>

// const int64_t kUndefinedDist = 1000 * 1000 * 1000 * 1LL;
using VertexT = int32_t;
using SizeT = uint64_t;

enum { ORIENTED = true, NOT_ORIENTED = false };

enum { TRANSPOSED = true, NOT_TRANSPOSED = false };

class IGraph {
protected:
    VertexT vertex_num_ = 0;
    bool oriented_;
    bool transposed_;

public:
    [[nodiscard]] VertexT GetVertexNum() const {
        return vertex_num_;
    }

    [[nodiscard]] virtual std::vector<VertexT> GetNeighbors(const VertexT& current_vertex) const = 0;

    virtual void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to) = 0;

    virtual bool IsTransposed() const = 0;
};

class GraphList final : public IGraph {
private:
    std::vector<std::vector<VertexT>> adjacency_list_;

public:
    explicit GraphList(const VertexT& vertex_number, bool oriented_status, bool transposed_status) {
        vertex_num_ = vertex_number;
        adjacency_list_.resize(vertex_number);
        for (VertexT i = 0; i < vertex_number; i++) {
            adjacency_list_[i] = std::vector<VertexT>();
        }
        oriented_ = oriented_status;
        transposed_ = transposed_status;
    }

    [[nodiscard]] std::vector<VertexT> GetNeighbors(const VertexT& v) const override {
        return adjacency_list_.at(v);
    }

    void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to) override {
        adjacency_list_[vertex_from].push_back(vertex_to);
        if (!oriented_) {
            adjacency_list_[vertex_to].push_back(vertex_from);
        }
    }

    bool IsTransposed() const override {
        return transposed_;
    }

    GraphList TransposeGraph() {
        GraphList transpose_graph((*this).GetVertexNum(), ORIENTED, TRANSPOSED);
        for (VertexT vert = 0; vert < (*this).GetVertexNum(); vert++) {
            auto neighbors = (*this).GetNeighbors(vert);
            for (auto& neighbor : neighbors) {
                transpose_graph.AddEdge(neighbor, vert);
            }
        }
        return transpose_graph;
    }
};

class GraphMatrix final : public IGraph {
private:
    std::vector<std::vector<VertexT>> matrix_;

public:
    explicit GraphMatrix(const VertexT& quantity, bool oriented_status, bool transposed_status) {
        for (VertexT i = 0; i < quantity; ++i) {
            std::vector<VertexT> tmp(quantity, 0);
            matrix_.push_back(tmp);
        }
        vertex_num_ = quantity;
        oriented_ = oriented_status;
        transposed_ = transposed_status;
    }

    [[nodiscard]] std::vector<VertexT> GetNeighbors(const VertexT& current_vertex) const override {
        std::vector<VertexT> answer;
        for (VertexT i = 0; i < GetVertexNum(); ++i) {
            if (matrix_[current_vertex][i] != 0) {
                answer.push_back(i);
            }
        }
        return answer;
    }

    void AddEdge(const VertexT& vertex_from, const VertexT& vertex_to) override {
        if (vertex_from == vertex_to) {
            return;
        }
        matrix_[vertex_from][vertex_to] = 1;
        if (!oriented_) {
            matrix_[vertex_to][vertex_from] = 1;
        }
    }

    bool IsTransposed() const override {
        return transposed_;
    }

    GraphMatrix TransposeGraph() {
        GraphMatrix transpose_graph((*this).GetVertexNum(), ORIENTED, TRANSPOSED);
        for (VertexT vert = 0; vert < (*this).GetVertexNum(); vert++) {
            auto neighbors = (*this).GetNeighbors(vert);
            for (auto& neighbor : neighbors) {
                transpose_graph.AddEdge(neighbor, vert);
            }
        }
        return transpose_graph;
    }
};

struct StrongConnectivity {
    std::vector<bool> used;
    std::vector<VertexT> current_comp;
    std::vector<VertexT> order;

    explicit StrongConnectivity(VertexT size) {
        used.resize(size, false);
    }

    void operator()(VertexT size) {
        used.assign(size, false);
    }
};

void DFStoFindStrongConnect(const IGraph& graph, VertexT current_vertex, StrongConnectivity& component) {
    component.used[current_vertex] = true;
    if (graph.IsTransposed()) {
        component.current_comp.push_back(current_vertex);
    }

    std::vector<VertexT> neighbors = graph.GetNeighbors(current_vertex);
    for (auto& neighbor : neighbors) {
        if (!component.used[neighbor]) {
            DFStoFindStrongConnect(graph, neighbor, component);
        }
    }
    if (!graph.IsTransposed()) {
        component.order.push_back(current_vertex);
    }
}

void PrintStrongConnectivity(std::vector<VertexT>& strong_comp_array) {
    int num_of_comp_in_answer = *std::max_element(strong_comp_array.begin(), strong_comp_array.end());
    std::cout << num_of_comp_in_answer << "\n";
    for (SizeT i = 0; i < strong_comp_array.size(); i++) {
        std::cout << strong_comp_array[i] << " ";
    }
}

std::vector<VertexT> FindStrongConnectivity(GraphList& graph) {
    GraphList transpose_graph = graph.TransposeGraph();

    StrongConnectivity component(graph.GetVertexNum());
    std::vector<VertexT> answer(graph.GetVertexNum());
    int num_of_comp_in_answer = 1;

    for (auto i = 0; i < graph.GetVertexNum(); i++) {
        if (!component.used[i]) {
            DFStoFindStrongConnect(graph, i, component);
        }
    }

    component(graph.GetVertexNum());

    std::reverse(component.order.begin(), component.order.end());

    for (auto vertex = 0; vertex < graph.GetVertexNum(); vertex++) {
        VertexT current_vertex = component.order[vertex];
        if (!component.used[current_vertex]) {
            DFStoFindStrongConnect(transpose_graph, current_vertex, component);
            for (auto& comp_elem : component.current_comp) {
                answer[comp_elem] = num_of_comp_in_answer;
            }
            num_of_comp_in_answer++;
            component.current_comp.clear();
        }
    }

    return answer;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    VertexT vertex_num = 0;
    VertexT edge_num = 0;
    std::cin >> vertex_num >> edge_num;

    GraphList graph(vertex_num, ORIENTED, NOT_TRANSPOSED);

    for (VertexT i = 0; i < edge_num; ++i) {
        VertexT vertex_from = 0;
        VertexT vertex_to = 0;
        std::cin >> vertex_from >> vertex_to;
        graph.AddEdge(vertex_from - 1, vertex_to - 1);
    }

    std::vector<VertexT> answer = FindStrongConnectivity(graph);
    PrintStrongConnectivity(answer);

    return 0;
}
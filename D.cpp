#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <list>

#define MAX_NUM 10000

enum {
    ORIENTED = true,
    NOT_ORIENTED = false
};

int plusOne(int n) {
    if (n / 1000 != 9) {
        return n + 1000;
    }
    return n;
}

int minusOne(int n) {
    if (n % 10 != 1) {
        return n - 1;
    }
    return n;
}

int shiftLeft(int n) {
    return (n % 1000) * 10 + n / 1000;
}

int shiftRight(int n) {
    return (n % 10) * 1000 + n / 10;
}

class Graph {
protected:
    int vertex_degree = 0;
    bool oriented;

public:
    [[nodiscard]] int getVertexDegree() const {
        return vertex_degree;
    }

    [[nodiscard]] virtual std::vector<int> getNeighbors(const int& v) const = 0;

    virtual void addEdge(const int& source, const int& dest) = 0;
};

class GraphList final : public Graph {
private:
    std::unordered_map<int, std::vector<int>> adjacency_list;
    bool oriented;

public:
    explicit GraphList(const int& vertex_number, bool oriented_status) {
        vertex_degree = vertex_number;
        for (int i = 0; i < vertex_number; i++) {
            adjacency_list[i] = std::vector<int>();
        }
        oriented = oriented_status;
    }

    [[nodiscard]] std::vector<int> getNeighbors(const int& v) const override {
        return adjacency_list.at(v);
    }

    void addEdge(const int& source, const int& dest) override {
        adjacency_list[source].push_back(dest);
        if (!oriented) {
            adjacency_list[dest].push_back(source);
        }
    }

    void setNeighbor(const int& current_num) {
        std::string number_str = std::to_string(current_num);
        if (number_str.find_first_of('0') != std::string::npos) {
            return;
        }
        int next_num = 0;

        if (current_num % 1000 != 9) {
            next_num = plusOne(current_num);
            addEdge(current_num, next_num);
        }

        if (current_num % 10 != 1) {
            next_num = minusOne(current_num);
            addEdge(current_num, next_num);
        }

        next_num = shiftLeft(current_num);
        addEdge(current_num, next_num);

        next_num = shiftRight(current_num);
        addEdge(current_num, next_num);
    }
};

void path(int n, std::vector<int>& parent) {
    std::vector<int> res;
    while (n != -1) {
        res.push_back(n);
        n = parent[n];
    }
    std::cout << res.size() << "\n";
    std::reverse(res.begin(), res.end());
    for (auto &num: res) {
        std::cout << num << "\n";
    }
}

void shortestPath(const Graph& graph, const int& start, const int& dest) {
    std::vector<int> used(MAX_NUM, 0);
    std::vector<int> parent(MAX_NUM, -1);
    std::list<int> queue;

    queue.push_front(start);
    used[start] = 1;

    while (!queue.empty()) {
        int current_num = queue.front();
        queue.pop_front();
        if (current_num == dest) {
            break;
        }
        std::vector<int> neighbors = graph.getNeighbors(current_num);

        for (auto& neighbor: neighbors) {
            if (used[neighbor] == 0) {
                used[neighbor] = 1;
                parent[neighbor] = current_num;
                queue.push_back(neighbor);
            }
        }
    }

    path(dest, parent);
}

int main() {
    int start = 0, dest = 0;
    std::cin >> start >> dest;

    GraphList graph(MAX_NUM, ORIENTED);

    for (int num = 1000; num < 10000; num++) {
        graph.setNeighbor(num);
    }

    shortestPath(graph, start, dest);

    return 0;
}
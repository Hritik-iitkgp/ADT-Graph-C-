#include <algorithm>
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

class Vertex;
class Edge;

class Edge {
public:
  Vertex *vertex1;
  Vertex *vertex2;
  string element;
  Edge(Vertex *vertex1, Vertex *vertex2, string element)
      : vertex1(vertex1), vertex2(vertex2), element(element) {}
  bool operator==(const Edge &other) const { return element == other.element; }
  Vertex *opposite(Vertex *vertex) {
    if (vertex == vertex1) {
      return vertex2;
    } else if (vertex == vertex2) {
      return vertex1;
    } else {
      return nullptr;
    }
  }
  bool isAdjacentTo(Vertex *vertex) {
    return (vertex == vertex1) || (vertex == vertex2);
  }
  bool isIncidentOn(Vertex *vertex) {
    return (vertex == vertex1) || (vertex == vertex2);
  }
};
class Vertex {
public:
  string element;
  Vertex(string element) : element(element) {}
  bool operator==(const Vertex &other) const {
    return element == other.element;
  }
  bool isAdjacentTo(Vertex *vertex) { return element == vertex->element; }
  vector<Edge *> incidentEdges(vector<Edge *> &edges);
};

vector<Edge *> Vertex::incidentEdges(vector<Edge *> &edges) {
  vector<Edge *> result;
  for (Edge *edge : edges) {
    if (edge->isIncidentOn(this)) {
      result.push_back(edge);
    }
  }
  return result;
}

class Graph {
public:
  vector<Vertex *> vertices;
  vector<Edge *> edges;
  Graph(string filename) { insertFromFile(filename); }
  Vertex *getVertex(string element);
  void insertVertex(string element);
  void insertEdge(Vertex *vertex1, Vertex *vertex2, string element);
  void eraseVertex(Vertex *vertex);
  void eraseEdge(Edge *edge);
  vector<Edge *> incidentEdges(vector<Edge *> &edges, string vertex_element);
  bool isEdgeExists(string vertex1_element, string vertex2_element);
  void insertFromFile(string filename);
  bool hasVertex(string element);
  vector<string> findPath(Vertex *start_vertex, Vertex *end_vertex,
                          set<string> &visited, vector<string> &path);
};

Vertex *Graph::getVertex(string element) {
  for (Vertex *vertex : vertices) {
    if (vertex->element == element) {
      return vertex;
    }
  }
  return nullptr;
}

void Graph::insertVertex(string element) {
  Vertex *vertex = new Vertex(element);
  vertices.push_back(vertex);
}

void Graph::insertEdge(Vertex *vertex1, Vertex *vertex2, string element) {
  Vertex *v1 = nullptr;
  Vertex *v2 = nullptr;
  for (Vertex *vertex : vertices) {
    if (vertex->isAdjacentTo(vertex1)) {
      v1 = vertex;
    } else if (vertex->isAdjacentTo(vertex2)) {
      v2 = vertex;
    }
  }

  if (v1 == nullptr || v2 == nullptr) {
    cout << "Insertion Failed" << endl;
  } else {
    Edge *edge = new Edge(v1, v2, element);
    edges.push_back(edge);
    cout << "Edge Inserted" << endl;
  }
}

void Graph::eraseVertex(Vertex *vertex) {
  vector<Edge *> incident_edges = vertex->incidentEdges(edges);
  for (Edge *edge : incident_edges) {
    eraseEdge(edge);
  }
  vertices.erase(remove(vertices.begin(), vertices.end(), vertex),
                 vertices.end());
  cout << "Removed" << endl;
}

void Graph::eraseEdge(Edge *edge) {
  edges.erase(remove(edges.begin(), edges.end(), edge), edges.end());
  cout << "Removed" << endl;
}

vector<Edge *> Graph::incidentEdges(vector<Edge *> &edges,
                                    string vertex_element) {
  vector<Edge *> incident_edges;
  for (Edge *edge : edges) {
    if (edge->vertex1->element == vertex_element ||
        edge->vertex2->element == vertex_element) {
      incident_edges.push_back(edge);
    }
  }
  return incident_edges;
}

bool Graph::isEdgeExists(string vertex1_element, string vertex2_element) {
  Vertex *vertex1 = getVertex(vertex1_element);
  Vertex *vertex2 = getVertex(vertex2_element);

  if (vertex1 == nullptr || vertex2 == nullptr) {
    return false;
  }

  for (Edge *edge : edges) {
    if ((edge->vertex1 == vertex1 && edge->vertex2 == vertex2) ||
        (edge->vertex1 == vertex2 && edge->vertex2 == vertex1)) {
      return true;
    }
  }

  return false;
}

void Graph::insertFromFile(string filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    cout << "Could not open file " << filename << endl;
    return;
  }

  string line;
  getline(file, line);
  stringstream ss(line);
  string element;
  while (getline(ss, element, ',')) {
    Vertex *vertex = new Vertex(element);
    vertices.push_back(vertex);
  }

  while (getline(file, line)) {
    stringstream ss(line);
    string element1, element2, weight;
    getline(ss, element1, '\t');
    getline(ss, weight, '\t');
    getline(ss, element2, '\t');

    Vertex *vertex1 = getVertex(element1);
    Vertex *vertex2 = getVertex(element2);

    if (vertex1 != nullptr && vertex2 != nullptr) {
      insertEdge(vertex1, vertex2, weight);
    }
  }

  file.close();
}

bool Graph::hasVertex(string element) {
  for (Vertex *vertex : vertices) {
    if (vertex->element == element) {
      return true;
    }
  }
  return false;
}

vector<string> Graph::findPath(Vertex *start_vertex, Vertex *end_vertex,
                               set<string> &visited, vector<string> &path) {
  visited.insert(start_vertex->element);
  path.push_back(start_vertex->element);

  if (start_vertex == end_vertex) {
    return path;
  }

  for (Edge *edge : incidentEdges(edges, start_vertex->element)) {
    Vertex *next_vertex = edge->opposite(start_vertex);
    if (visited.count(next_vertex->element) == 0) {
      vector<string> result = findPath(next_vertex, end_vertex, visited, path);
      if (!result.empty()) {
        return result;
      }
    }
  }

  path.pop_back();
  return vector<string>();
}

int main() {
  string filename;
  cout << "Enter the filename: ";
  cin >> filename;
  Graph g(filename);
  cout << "Graph is ready." << endl;

  while (true) {
    cout << "Please choose an option:" << endl;
    cout << "1. Find edges incident on a vertex" << endl;
    cout << "2. Find a path in the graph" << endl;
    cout << "3. Insert an edge" << endl;
    cout << "4. Erase a vertex" << endl;
    cout << "5. Quit" << endl;

    char choice;
    cout << "Enter your choice: ";
    cin >> choice;
    if (choice == '1') {
      string vertex_element;
      cout << "Enter the label of the vertex: ";
      cin >> vertex_element;

      Vertex *vertex = g.getVertex(vertex_element);
      if (vertex) {
        vector<Edge *> incident_edges = vertex->incidentEdges(g.edges);
        cout << "Edges incident on vertex " << vertex_element << ":" << endl;
        for (Edge *edge : incident_edges) {
          cout << edge->vertex1->element << " to " << edge->vertex2->element
               << " is " << edge->element << " mi" << endl;
        }
      } else {
        cout << "Vertex " << vertex_element << " not found." << endl;
      }
    }

    else if (choice == '2') {
      string vertex_element1, vertex_element2;
      cout << "Enter the label of the starting vertex: ";
      cin >> vertex_element1;
      cout << "Enter the label of the ending vertex: ";
      cin >> vertex_element2;

      Vertex *vertex1 = g.getVertex(vertex_element1);
      Vertex *vertex2 = g.getVertex(vertex_element2);

      if (vertex1 && vertex2) {
        set<string> visited;
        vector<string> path;

        path = g.findPath(vertex1, vertex2, visited, path);
        if (!path.empty()) {
          cout << "Path from " << vertex_element1 << " to " << vertex_element2
               << ": " << endl;
          for (string vertex : path) {
            cout << vertex << " to ";
          }
          cout << endl;
        } else {
          cout << "No path found from " << vertex_element1 << " to "
               << vertex_element2 << "." << endl;
        }
      } else {
        cout << "Invalid vertices. Please provide valid vertices." << endl;
      }
    }

    else if (choice == '3') {
      string vertex_element1, vertex_element2, element;
      cout << "Enter the label of the first vertex: ";
      cin >> vertex_element1;
      cout << "Enter the label of the second vertex: ";
      cin >> vertex_element2;

      bool is_presentv1 = g.hasVertex(vertex_element1);
      bool is_presentv2 = g.hasVertex(vertex_element2);
      if (!is_presentv1 || !is_presentv2) {
        cout << "Edge insert failed: vertices not exist" << endl;
        continue;
      }

      if (g.isEdgeExists(vertex_element1, vertex_element2)) {
        cout << "Edge insert failed: edge already exists" << endl;
        continue;
      }

      cout << "Enter the element of the new edge: ";
      cin >> element;

      Vertex *vertex1 = g.getVertex(vertex_element1);
      Vertex *vertex2 = g.getVertex(vertex_element2);
      if (vertex1 && vertex2) {
        g.insertEdge(vertex1, vertex2, element);
        cout << "Edge " << element << " added between " << vertex_element1
             << " and " << vertex_element2 << "." << endl;
      } else {
        cout << "Invalid vertices. Please provide valid vertices." << endl;
      }
    }
  }
}

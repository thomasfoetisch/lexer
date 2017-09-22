#ifndef _CIRCUIT_H_
#define _CIRCUIT_H_

enum class elem_type {resistor, voltage_source};
typedef std::string node_id;

struct edge {
  elem_type type;
  node_id source, target;
  double value;
};

std::ostream& operator<<(std::ostream& stream, const edge& e) {
  switch (e.type) {
  case elem_type::resistor: stream << "Resistor"; break;
  case elem_type::voltage_source: stream << "Voltage"; break;
  }
  
  std::cout << " (" << e.value << " ";
  switch (e.type) {
  case elem_type::resistor: stream << "Ohm"; break;
  case elem_type::voltage_source: stream << "Volt"; break;
  }
  std::cout << " ) ";
  
  std::cout << "from" << e.source << " to " << e.target << ";";

  return stream;
}

class circuit {
public:
  void add_edge(const edge& e) { edges.push_back(e); }
  void build_linear_system() {
    std::vector<node_id> nodes(get_nodes());

    std::vector<node_id>::const_iterator
      ground(nodes.begin()),
      n(nodes.begin() + 1);

    std::vector<node_id>
      r_edges(get_edges_of_type(elem_type::resistor)),
      vs_edges(get_edges_of_type(elem_type::resistor));

    /*
     * TODO: build the linear system.
     */
  }

  std::vector<node_id> get_edges_of_type(elem_type t) {
    std::vector<edge> es;

    for (auto e&: edges)
      if (e.type == t)
	es.push_back(e);

    return es;
  }
  
  std::vector<node_id> get_nodes() {
    std::set<node_id> nodes;

    for (const auto& e: edges) {
      nodes.insert(e.first);
      nodes.insert(e.second);
    }
    
    return std::vector<node_id>(nodes.begin(), nodes.end());
  }
  
private:
  std::vector<edge> edges;
};

#endif /* _CIRCUIT_H_ */

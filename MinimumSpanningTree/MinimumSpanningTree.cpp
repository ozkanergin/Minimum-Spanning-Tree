#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <utility>
#include <algorithm>
#include <string>
#include <vector>

// METIN OZKAN ERGIN 504201578  HW2 QUESTION 1

using namespace std;

class Path		// This is the class which we are going to add solutions path for minimum span tree.
{
public:
	int cost;
	string first_point;
	string second_point;
};

std::vector<Path> solution_paths;

std::vector<Path> Bubble_Sort_Algortihm(std::vector<Path> solution_paths) {

	for (int i = 0; i < solution_paths.size(); i++)			// Use the bubble sort algorithms to get the desired output. It sorts the path instances, according to their cost.
	{
		for (int j = 0; j < solution_paths.size() - i - 1; j++)
		{
			if (solution_paths[j].cost > solution_paths[j + 1].cost)
			{
				Path temp = solution_paths[j];
				solution_paths[j] = solution_paths[j + 1];
				solution_paths[j + 1] = temp;
			}
		}
	}
	return solution_paths;
}

class Graph {

	typedef pair<string, string> Edge; // It is a pair which include two different points which are given by text file for each line.
	typedef pair<int, Edge> CostEdge; // It  is a pair which include edge and it's cost.

private:
	vector<CostEdge> graph;
	map<string, string> point_map; // Find the unique place.They have to get connection at least one point.

	string Find_Unconnected_Point(string index)
	{
		if (index != point_map[index])		// If the input point has a connection (value is different from key) take the value point to continue the iteration.
		{
			point_map[index] = Find_Unconnected_Point(point_map[index]);
		}
		return point_map[index];
	}

	void Add_Node(string node)
	{
		point_map.insert(pair<string, string>(node, node));		// Add the points to the map. The map keeps the unique points because of key-value relation.
	}


public:
	void Add_Edge(string start, string end, int cost)
	{
		Add_Node(start);		// Add the node to the map. However if they are same, input is not going to be added to map.
		Add_Node(end);
		graph.push_back(CostEdge(cost, Edge(start, end)));
	}

	int Find_Minimum_Span_Tree() {

		bool church_added_to_great_palace = false;
		bool hippodrome_added_to_great_palace = false;
		int total_cost = 0;

		sort(graph.begin(), graph.end());		// Sorting the element of graph vector according the cost between two nodes. Principal of Kruskal algorithm. We are going to iterate them from beginning. That's the reason writing for this code.
		vector<CostEdge>::iterator cost_and_points_iterator;

		for (cost_and_points_iterator = graph.begin(); cost_and_points_iterator != graph.end(); ++cost_and_points_iterator) { // ITerate the graph.
			string start = cost_and_points_iterator->second.first;		// Take the starting and ending point for graph.
			string end = cost_and_points_iterator->second.second;

			string starting_point = Find_Unconnected_Point(start);		// If there is no any connection for the point , take this point. Otherwise it is going to change.
			string end_point = Find_Unconnected_Point(end);		// If there is no any connection for the point , take this point. Otherwise it is going to change.

			// Below 2 if conditions are written for satisfy the Great Palace condition first. Otherwise , we can not always get the true result. They are like our pre-condition.

			if ((start == "GP" && end.substr(0, 2) == "Ch") && false == church_added_to_great_palace)		// First make the conditions true for great palace. Since construction starts from GP. 
			{																							   // There are two conditions for GP. GP directly connect at least one church and hippodrome. These conditions are satisfied two different if else block.
				point_map[end_point] = starting_point;

				Path adding_edge;
				adding_edge.first_point = start;
				adding_edge.second_point = end;
				adding_edge.cost = cost_and_points_iterator->first;  // First element of iterator is cost.
				solution_paths.push_back(adding_edge);
				church_added_to_great_palace = true;
			}

			if (start == "GP" && end == "Hipp" && false == hippodrome_added_to_great_palace)
			{
				point_map[end_point] = starting_point;

				Path adding_edge;
				adding_edge.first_point = start;
				adding_edge.second_point = end;
				adding_edge.cost = cost_and_points_iterator->first;	// First element of iterator is cost.
				solution_paths.push_back(adding_edge);

				hippodrome_added_to_great_palace = true;
			}
			if (true == hippodrome_added_to_great_palace && true == church_added_to_great_palace) break;
		}

		for (cost_and_points_iterator = graph.begin(); cost_and_points_iterator != graph.end(); ++cost_and_points_iterator)
		{
			string start = cost_and_points_iterator->second.first;
			string end = cost_and_points_iterator->second.second;

			string starting_point = Find_Unconnected_Point(start);	// First check the starting point condition. It has connection or not.
			string end_point = Find_Unconnected_Point(end);	// Second  check the end point condition. It has connection or not.

			if ((start == "Hipp" && end.substr(0, 3) == "Bas") || (end == "Hipp" && start.substr(0, 3) == "Bas") || (end.substr(0, 2) == "Hp" && start.substr(0, 2) == "Hp")) // If graph contains undesired situation skip the step and checking the next one.
			{
				continue;
			}

			if (starting_point != end_point) // If these points are not equal each other which means they do not have any connection make them connected.
			{
				Path adding_edge;
				adding_edge.first_point = start;
				adding_edge.second_point = end;
				adding_edge.cost = cost_and_points_iterator->first;	// First element of iterator is cost.
				solution_paths.push_back(adding_edge);

				point_map[end_point] = starting_point;	// Make them connected each other. For this step only end point looks like have an output. Starting point still accepts connection.
			}
		}

		solution_paths = Bubble_Sort_Algortihm(solution_paths);		// Use the bubble sort algorithms to get the desired output. It sorts the path instances, according to their cost.
		for (Path current_path : solution_paths)				// Show the solution
		{
			cout << current_path.first_point << " " << current_path.second_point << " " << current_path.cost << endl;
			total_cost += current_path.cost;	// Add every path's cost to the total amount of cost.
		}
		cout << total_cost << endl;
		return 0;
	}
};

int main()
{
	Graph graph;

	string file_name;
	string cost;
	string line;
	string line_param;
	vector<string> line_vector;
	cout << "Dosya ismini giriniz" << endl;
	cin >> file_name;

	ifstream input_file(file_name);		// Take the input file.

	if (input_file.is_open())
	{
		while (getline(input_file, line)) {
			line_vector.clear();

			istringstream parsing_line(line);
			while (getline(parsing_line, line_param, ',')) {
				line_vector.push_back(line_param);
			}

			if (line_vector[2].empty() != true)	// If there is no any cost do not add the edge to the graph.
			{
				cost = line_vector[2];
				int weight = std::stoi(cost);
				graph.Add_Edge(line_vector[0], line_vector[1], weight);
			}
		}
		input_file.close();
	}

	graph.Find_Minimum_Span_Tree();

	return 0;
}

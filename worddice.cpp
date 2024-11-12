#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <set>

using namespace std;

// Node structure
struct Node {
    string nodeName;  // Name of the node
    vector<int> connectedNodes;  // Vector to hold connected node indices
};

int main(int argc, char *argv[]) {
    
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <diceFile> <wordFile>" << endl;
        return 1;
    }

    
    string diceFile = argv[1];
    string wordFile = argv[2];

    
    map<int, Node> graph;

    // Open the dice file using fstream
    fstream diceStream(diceFile, ios::in);  
    if (!diceStream) {
        cerr << "Error opening dice file!" << endl;
        return 1;
    }

    // Vector to store dice names
    vector<string> diceNames;
    string line;

    // Read dice names from the file
    while (getline(diceStream, line)) {
        diceNames.push_back(line);  // Add each line (dice name) to the vector
    }

    diceStream.close();  

    // Insert the source node (node 0) into the graph
    Node sourceNode;
    sourceNode.nodeName = "SOURCE";
    graph[0] = sourceNode;

    // Insert dice nodes into the graph and link them to the source node
    for (int i = 0; i < diceNames.size(); ++i) {
        Node diceNode;
        diceNode.nodeName = diceNames[i];  
        graph[i + 1] = diceNode;  

        // Add the dice node to the source node's connected nodes
        graph[0].connectedNodes.push_back(i + 1);
    }

    // Open the word file using fstream
    fstream wordStream(wordFile, ios::in);  
    if (!wordStream) {
        cerr << "Error opening word file!" << endl;
        return 1;
    }

    // Process each word
    while (getline(wordStream, line)) {
        // Start with a fresh index for each new word's graph
        int wordNodeIndex = diceNames.size() + 1;  // Start after dice nodes

        map<int, Node> wordGraph = graph;  // Copy the base graph for this word
        map<char, int> letterNodes;  // Map to store letter nodes

        // Insert letter nodes for the current word
        for (size_t i = 0; i < line.length(); ++i) {
            char letter = line[i];

            // Create a new node for the letter, even if it has been seen before
            Node letterNode;
            letterNode.nodeName = string(1, letter);  // Name the letter node as the letter itself
            wordGraph[wordNodeIndex] = letterNode;  

            // Connect this letter node to the corresponding dice node
            for (int diceIndex = 1; diceIndex <= diceNames.size(); ++diceIndex) {
                if (diceNames[diceIndex - 1].find(letter) != string::npos) {
                    wordGraph[diceIndex].connectedNodes.push_back(wordNodeIndex);  // Connect dice to letter node
                }
            }

            wordNodeIndex++;  // Increment the node index for the next letter node
        }

        // Connect letter nodes to the sink node (node after all letters)
        int sinkNodeIndex = wordNodeIndex;
        Node sinkNode;
        sinkNode.nodeName = "SINK";
        wordGraph[sinkNodeIndex] = sinkNode;

        for (const auto& entry : wordGraph) {
            if (entry.first >= diceNames.size() + 1 && entry.first < sinkNodeIndex) {
                wordGraph[entry.first].connectedNodes.push_back(sinkNodeIndex);
            }
        }

        // Print the graph for this word
        cout << "Graph for word: " << line << endl;
        for (const auto& entry : wordGraph) {
            cout << "Node " << entry.first << ": " << entry.second.nodeName;
            if (!entry.second.connectedNodes.empty()) {
                cout << " Edges to ";
                for (int nodeIndex : entry.second.connectedNodes) {
                    cout << nodeIndex << " ";
                }
            }
            cout << endl;
        }

        // Reset the graph for the next word
        wordGraph.clear();
    }

    wordStream.close();

    return 0;
}

#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <raylib.h>

using namespace std;

const int SCALE = 10;
const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 450;
const int xOFFSET = 0; //SCREENWIDTH / 2;
const int yOFFSET = 0; //SCREENHEIGHT / 2;

#define SAND YELLOW

class NodeType{
    public:
    Color color;
    string name;

    NodeType(){
        color = WHITE;
        name = "void";
    }

    NodeType(Color color_, string name_){
        color = color_;
        name = name_;
    }

    string to_string(){
        return name;
    }
};

class Node{
    int x;
    int y;
    public:
    Node* parent = NULL;
    NodeType nType;
    int inital = 9999;  // Distance from Node to start
    int heuristic  = 9999;  // Distance from Node to end
    int cost = 9999;  // inital + heuristic

    Node(){
        x = 0 + xOFFSET;
        y = 0 + yOFFSET;
        nType = NodeType();
    }

    Node(int x_, int y_){
        x = x_ + xOFFSET;
        y = y_ + yOFFSET;
        nType.color = GREEN;
        nType.name = "OPEN";
    }

    Node(Color color_,string name_){
        nType.color = color_;
        nType.name = name_;
    }

    Node(int x_, int y_, Color color_,string name_){
        x = x_ + xOFFSET;
        y = y_ + yOFFSET;
        nType.color = color_;
        nType.name = name_;
    }

    string to_string(){
        return nType.name;
    }

    int get_distance(Node goal){
        int distance = (int)ceil(sqrt(pow(goal.x - x,2) + pow(goal.y - y,2)));
        return distance;
    }

    int set_H(Node end){
        heuristic = get_distance(end);
        update_cost();
        return heuristic ;
    }

    int set_inital(Node start){
        inital = get_distance(start);
        update_cost();
        return inital;
    }

    void update_cost(){
        cost = heuristic + inital;
    }

    int get_x(){
        return x;
    }
    int get_y(){
        return y;
    }

    int get_x_scaled(){
        return (x * SCALE) + xOFFSET;
    }
    int get_y_scaled(){
        return (y * SCALE) + yOFFSET;
    }
    void set_x(int x_){
        x = x_;
    }
    void set_y(int y_){
        y = y_;
    }
};

class Compare {
    public:
    bool operator()(Node* a, Node* b){
        return a->cost > b->cost;
    }
};

void findPath(Node* start, Node* end, vector<vector<Node>> mat){
    priority_queue<Node*,vector<Node*>,Compare> open;   // Visited but not expanded
    priority_queue<Node*,vector<Node*>,Compare> closed; // Visited and expanded
    start->inital = 0;
    start->set_H(*end);
    open.push(start);
    
    while(!open.empty()){
        // From the open list, grab the node with the smallest cost
        Node* currentNode = open.top();
        cout<<currentNode->cost<<" ";
        
        if(currentNode == end){
            break; // Path found
        }

        // Add all neighboring Nodes to a vector
        int x = currentNode->get_x();
        int y = currentNode->get_y();
        vector<Node*> neighborNodes(8);
        neighborNodes[0] = &mat[x][y-1];    // North
        neighborNodes[1] = &mat[x+1][y-1];    // North-East
        neighborNodes[2] = &mat[x+1][y];    // East
        neighborNodes[3] = &mat[x+1][y+1];    // South-East
        neighborNodes[4] = &mat[x][y+1];    // South
        neighborNodes[5] = &mat[x-1][y+1];    // South-West
        neighborNodes[6] = &mat[x-1][y];    // West
        neighborNodes[7] = &mat[x-1][y-1];    // North-West

        for (auto& i : neighborNodes) { 
            cout << i->heuristic << ' '; 
        } 

        // Move current node to closed queue
        closed.push(currentNode);  
        open.pop();
    }

    return;
}

int main () {

    vector<vector<Node>> matrix(SCREENWIDTH / SCALE,vector<Node>(SCREENHEIGHT / SCALE));
    for(int w=0;w<(int)matrix.size();w++){
            for(int h=0;h<(int)matrix[w].size();h++){
                matrix[w][h].set_x(w);
                matrix[w][h].set_y(h);
            }
        }
    cout << "Debugging Output" << endl << endl;
    Node* start = &matrix[5][25];
    Node* end = &matrix[30][5];
    start->nType.color = BLUE;
    end->nType.color = ORANGE;

    Node* test = &matrix[30][4];
    test->nType.color = GREEN;
    end->parent = test;

    Node* test2 = &matrix[31][4];
    test2->nType.color = GREEN;
    test->parent = test2;

    findPath(start,end,matrix);



    cout << "End of Debugging Output" << endl << endl;
    
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Particle Sandbox 2");
	SetTargetFPS(60);               // Set game to run at 60 frames-per-second

	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
	    // Update variables
	    //----------------------------------------------------------------------------------
		
	    	
	    //----------------------------------------------------------------------------------

	    // Draw
	    //----------------------------------------------------------------------------------
	    BeginDrawing();
	    ClearBackground(GRAY);

        for(int w=0;w<(int)matrix.size();w++){
            for(int h=0;h<(int)matrix[w].size();h++){
                DrawRectangle(matrix[w][h].get_x_scaled(),matrix[w][h].get_y_scaled(),SCALE,SCALE,matrix[w][h].nType.color);
            }
        }

        Node* next = end->parent;
        while(next != NULL){
            DrawRectangle(next->get_x_scaled(),next->get_y_scaled(),SCALE,SCALE,next->nType.color);
            next = next->parent;

        }
       
		
	    DrawText("Path Finding", 190, 200, 20, BLACK);
		
	    WaitTime(0.05);
		EndDrawing();
	    //----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------
    
    return 0;
}
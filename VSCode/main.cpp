#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <list>
#include <math.h>
#include <raylib.h>

using namespace std;

const int SCALE = 10;
const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 450;
const int xOFFSET = 0; //SCREENWIDTH / 2;
const int yOFFSET = 0; //SCREENHEIGHT / 2;
const Color WALL_COLOR = GRAY;
const Color PATH_COLOR = GREEN;
const Color BACKGROUND_COLOR = RAYWHITE;

enum nodeTypes{WALL, PATH, VOID};




class NodeType{
    public:
    Color color = BACKGROUND_COLOR;
    nodeTypes name = VOID;

    NodeType(){}

    NodeType(Color color_, nodeTypes name_){
        color = color_;
        name = name_;
    }

    string to_string(){
        switch(name){
            case VOID :
                return "Void";
            case PATH :
                return "Path";
            case WALL :
                return "Wall";
        }
        return "Error: Undefinable type";
    }
};

class Node{
    int x = 9999;
    int y = 9999;
    public:
    Node* parent = NULL;
    NodeType nType;
    int initial = 9999;  // Distance from Node to start
    int heuristic  = 9999;  // Distance from Node to end
    int cost = 9999;  // initial + heuristic

    Node(){
        x = 0 + xOFFSET;
        y = 0 + yOFFSET;
        nType = NodeType();
    }

    Node(int x_, int y_){
        x = x_ + xOFFSET;
        y = y_ + yOFFSET;
        nType = NodeType();
    }

    Node(Color color_,nodeTypes name_){
        nType.color = color_;
        nType.name = name_;
    }

    Node(int x_, int y_, Color color_,nodeTypes name_){
        x = x_ + xOFFSET;
        y = y_ + yOFFSET;
        nType.color = color_;
        nType.name = name_;
    }

    string to_string(){
        return nType.to_string();
    }

    int get_distance(Node goal){
        int distance = (int)ceil(sqrt(pow(goal.get_x() - x,2) + pow(goal.get_y() - y,2)) * 10);
        return distance;
    }

    int set_heuristic(Node end){
        heuristic = get_distance(end);
        update_cost();
        return heuristic ;
    }

    int set_initial(Node start){
        initial = get_distance(start);
        update_cost();
        return initial;
    }

    void set_initial(int distance){
        initial = distance;
        update_cost();
        return;
    }

    void update_cost(){
        cost = heuristic + initial;
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
    bool operator()(Node* const& a, Node* const& b){
        return a->cost > b->cost;
    }
};

double time_taken(clock_t start, clock_t end){
    return double(end - start) / double(CLOCKS_PER_SEC);
}

bool setContains(set<Node*,Compare> nodeSet, Node* target){ 
    for (set<Node*,Compare>::iterator i =nodeSet.begin();i!=nodeSet.end();i++) { 
        if(*i == target)
            return true;
    }
    return false;
}

bool listContains(list<Node*> l, Node* target){ 
    for (auto const& i : l) 
        if(target == i)
            return true;

    return false;
}

// If a path is found, return true; otherwise, false
bool findPath(Node* start, Node* end, vector<vector<Node>> &mat){
    priority_queue<Node*,vector<Node*>,Compare> open_cost;   // Visited but not expanded (Uses custom compare operator)
    set<Node*> open_addresses;    // Visited but not expanded
    set<Node*> closed;  // Visited and expanded
    
    // Initializing start variable
    start->set_initial(0);
    start->set_heuristic(*end);
    start->parent = NULL;

    open_addresses.insert(start);
    open_cost.push(start);

    Node* currentNode;
    
    // Loop until destination is reached
    while(!open_addresses.empty()){
        // find lowest F score (cost) and move it to the closed list
        currentNode = open_cost.top();
        open_cost.pop();
        open_addresses.erase(currentNode);
        closed.insert(currentNode);

        // End loop if currentNode is the end Node
        if(currentNode == end){
            end->nType.color = ORANGE;
            std::cout<<"Path Found!!!"<<endl;
            break;
        }

        // Add all neighboring nodes to a vector
        int x = currentNode->get_x();
        int y = currentNode->get_y();

        vector<Node*> neighborNodes;
        // neighborNodes included all nodes within an 8 directional radius
        // around the currentNode. This does not include walls or nodes outside the border
        for(int offX=-1;offX<=1;offX++){
            if(x+offX < 0 || x+offX >= (int)mat.size())
                continue; 
            for(int offY=-1;offY<=1;offY++){
                if((offX == 0 && offY == 0) || y+offY < 0 || y+offY >= (int)mat[x].size() || mat[x+offX][y+offY].nType.name == WALL)
                    continue; 
                neighborNodes.push_back(&mat[x+offX][y+offY]); 
            }
        }
        neighborNodes.shrink_to_fit();
        
        // For each neighbor node, check whether it is in either list and calculate F cost
        for (auto neighbor : neighborNodes) {
            if(closed.count(neighbor))
                continue;

            bool inOpen = open_addresses.count(neighbor);
            int neighborCost =  currentNode->initial + currentNode->get_distance(*neighbor);    // current cost + cost to neighbor
            
            if(!inOpen || neighborCost < neighbor->initial){
                neighbor->set_initial(neighborCost);
                neighbor->parent = currentNode;
                neighbor->nType.color = PATH_COLOR;

                if(!inOpen){
                    neighbor->set_heuristic(*end);
                    open_addresses.insert(neighbor);
                    open_cost.push(neighbor);
                }
            }   
        }
    }
        if(currentNode != end)
            cout<<"Error: Path not found"<<endl;

    return currentNode == end;
}

void generateMaze(vector<vector<Node>> &mat, set<Node*> &walls){
    // Generate Border

    // Generate Random Horizontal
    int horizontalSegments = (mat.size() / 3);
    for(int row=0;row<(int)mat[0].size();row+=2){
        for(int i=0;i<=horizontalSegments;){
            Node* wallSegment = &mat[rand() % mat.size()][row];
            if(wallSegment->nType.name == WALL){
                continue;
            }
            else{
                wallSegment->nType.name = WALL;
                wallSegment->nType.color = WALL_COLOR;
                walls.insert(wallSegment);
                i++;
            }
        }
    }
    
    // Generate Random Verticle
    int verticleSegments = (mat[0].size() / 3);
    for(int col=0;col<(int)mat.size();col+=2){
        for(int i=0;i<=verticleSegments;){
            Node* wallSegment = &mat[col][rand() % mat[col].size()];
            if(wallSegment->nType.name == WALL){
                continue;
            }
            else{
                wallSegment->nType.name = WALL;
                wallSegment->nType.color = WALL_COLOR;
                walls.insert(wallSegment);
                i ++;
            }
        }
    }
    
}

int main () {
    std::cout << "Debugging Output" << endl << endl;

    // Used for optimizing
    clock_t time_start, time_end;

    // Initialize a matrix scaled by the amount of pixels desired on screen
    vector<vector<Node>> matrix(SCREENWIDTH / SCALE,vector<Node>(SCREENHEIGHT / SCALE));
    for(int w=0;w<(int)matrix.size();w++){
            for(int h=0;h<(int)matrix[w].size();h++){
                matrix[w][h].set_x(w);
                matrix[w][h].set_y(h);
                //std::cout<<w<<" "<<h<<endl;
                if(matrix[w][h].get_x() < 0 || matrix[w][h].get_y() < 0){
                    std::cout<<"break"<<endl;
                    break;
                }
            }
    }
    cout<<matrix.size()<<" x "<<matrix[0].size()<<endl;

    // Default start and end nodes
    Node* start = &matrix[0][0];
    Node* end = &matrix[30][20];
    set<Node*> walls;
    start->nType.color = BLUE;
    end->nType.color = ORANGE;


    time_start = clock();
    bool pathExist = findPath(start,end,matrix);
    time_end = clock();
    double time_elapsed = time_taken(time_start,time_end);

    std::cout <<endl<<endl<< "End of Debugging Output" << endl << endl;

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Path Finding Algorithm");
	SetTargetFPS(60);               // Set game to run at 60 frames-per-second

	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
	    // Update variables
	    //----------------------------------------------------------------------------------
		int mousePositionX = (GetMouseX() - xOFFSET) / SCALE;
        int mousePositionY = (GetMouseY() - yOFFSET) / SCALE;
	    bool update = (bool)false;

        // Cap mouse position coords
        if(mousePositionX >= (int)matrix.size()){
            mousePositionX = matrix.size() - 1;
        }

        if(mousePositionY >= (int)matrix[0].size()){
            mousePositionY = matrix[0].size() - 1;
        }

        // Set new start point
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && IsKeyDown(KEY_LEFT_CONTROL)){
            if(matrix[mousePositionX][mousePositionY].nType.name != WALL && &matrix[mousePositionX][mousePositionY] != end){
                start = &matrix[mousePositionX][mousePositionY];
                start->nType.color = BLUE;
                start->nType.name = PATH;
                time_start = clock();
                pathExist = findPath(start,end,matrix);
                update = pathExist;
                time_end = clock();
                time_elapsed = time_taken(time_start,time_end);
            }
        }


        // Set new end point
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL)){
            if(matrix[mousePositionX][mousePositionY].nType.name != WALL && &matrix[mousePositionX][mousePositionY] != start){
                end = &matrix[mousePositionX][mousePositionY];
                end->nType.color = ORANGE;
                end->nType.name = PATH;
                time_start = clock();
                pathExist = findPath(start,end,matrix);
                update = pathExist;
                time_end = clock();
                time_elapsed = time_taken(time_start,time_end);
            }
        }

        // Place Wall
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_W)){
            Node* node = &matrix[mousePositionX][mousePositionY];
            if(node != end || node != start){
                node->nType.name = WALL;
                node->nType.color = WALL_COLOR;
                walls.insert(node);
                update = true;
            }
        }

        // Delete Wall
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_E)){
            Node* node = &matrix[mousePositionX][mousePositionY];
            if(node->nType.name == WALL){
                node->nType.name = VOID;
                node->nType.color = BACKGROUND_COLOR;
                walls.erase(node);
                update = true;
            }
        }

        // Delete All Walls
        if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_E)){
            for(Node* segment : walls){
                segment->nType.name = VOID;
                segment->nType.color = BACKGROUND_COLOR;
            }
            walls.clear();
        }

        // Randomly Generate Maze
        if(IsKeyPressed(KEY_G) && IsKeyDown(KEY_LEFT_CONTROL)){
            for(Node* segment : walls){
                segment->nType.name = VOID;
                segment->nType.color = BACKGROUND_COLOR;
            }
            walls.clear();

            generateMaze(matrix,walls);
        }
        

	    //----------------------------------------------------------------------------------

	    // Draw
	    //----------------------------------------------------------------------------------
	    BeginDrawing();
	    ClearBackground(BACKGROUND_COLOR);
        
        // Explored Areas
        /*
        for(int w=0;w<(int)matrix.size();w++){
            for(int h=0;h<(int)matrix[w].size();h++){
                DrawRectangle(matrix[w][h].get_x_scaled(),matrix[w][h].get_y_scaled(),SCALE,SCALE,matrix[w][h].nType.color);
            }
        }
        */


        // Draw start and end point
        DrawRectangle(start->get_x_scaled(),start->get_y_scaled(),SCALE,SCALE,start->nType.color);
        DrawRectangle(end->get_x_scaled(),end->get_y_scaled(),SCALE,SCALE,end->nType.color);

        // Draw out path
        int pathCount =0;
        Node* next = end->parent;
        if(pathExist){
            while(next != start){
                DrawRectangle(next->get_x_scaled(),next->get_y_scaled(),SCALE,SCALE,GREEN);
                next = next->parent;
                pathCount++;
            }
        }
        

        // Draw walls
        for(Node* segment : walls){
            DrawRectangle(segment->get_x_scaled(),segment->get_y_scaled(),SCALE,SCALE,GRAY);
        }

        // Write out mouse coordinates
        string mousePosition = "x: " + to_string(mousePositionX) + " y: " + to_string(mousePositionY);
        DrawText(mousePosition.c_str(),700,0,10,BLACK);
        // Write out steps taken
		DrawText(("Step count: " + to_string(pathCount)).c_str(), 30, 0, 20, BLACK);
	    
        // Write out time elapsed during findPath()
        if(time_elapsed >= 1.0)
            DrawText(("Time elapsed: " + to_string((ceil(time_elapsed * 1000)/1000.00)) + "sec").c_str(), 30, 30, 20, BLACK);
        else
            DrawText(("Time elapsed: " + to_string(((int)(time_elapsed * 1000))) + "ms").c_str(), 30, 30, 20, BLACK);
        
        
        DrawText("Path Finding", 190, 200, 20, BLACK);
		
	    WaitTime(0.01);
		EndDrawing();
	    //----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------
    
    return 0;
}
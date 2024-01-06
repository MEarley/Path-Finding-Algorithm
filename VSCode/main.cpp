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
    int x = 9999;
    int y = 9999;
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
        nType = NodeType();
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
        int distance = (int)ceil(sqrt(pow(goal.get_x() - x,2) + pow(goal.get_y() - y,2)) * 10);
        return distance;
    }

    int set_heuristic(Node end){
        heuristic = get_distance(end);
        update_cost();
        return heuristic ;
    }

    int set_inital(Node start){
        inital = get_distance(start);
        update_cost();
        return inital;
    }

    void set_inital(int distance){
        inital = distance;
        update_cost();
        return;
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
        return a->cost < b->cost;
    }
};

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

void findPath(Node* start, Node* end, vector<vector<Node>> &mat){
    list<Node*> open;    // Visited but not expanded
    list<Node*> closed;  // Visited and expanded
    
    // Initializing start variable
    start->set_inital(0);
    start->set_heuristic(*end);
    start->parent = NULL;
    open.push_front(start);
    
    // Loop until destination is reached
    while(!open.empty()){
        // find lowest F score (cost) and move it to the closed list
        Node* currentNode = open.front();
        for (auto const& nodes : open){
            //cout<<nodes->cost<<" ";
            if(nodes->cost < currentNode->cost){
                //cout<<"Lowest: "<<nodes->cost;
                currentNode = nodes;
            }
        }
        //cout<<endl;

        open.remove(currentNode);
        
        //std::cout<<open.size()<<" "<<"Open Size"<<endl;

        if(currentNode == end){
            end->nType.color = ORANGE;
            std::cout<<"Path Found!!!"<<endl;
            break; // Path found
        }

        closed.push_front(currentNode);
        //cout<<listContains(closed,currentNode);
        //cout<<listContains(open, currentNode)<<endl;
        // Add all neighboring nodes to a vector
        int x = currentNode->get_x();
        int y = currentNode->get_y();
        vector<Node*> neighborNodes;
        for(int offX=-1,i=0;offX<=1;offX++){
            if(x+offX < 0 || x+offX > mat.size()){
                //cout<<"skip";
                continue;}
            
            for(int offY=-1;offY<=1;offY++){
                if((offX == 0 && offY == 0) || y+offY < 0 || y+offY > mat[x].size()){
                    //cout<<"skip";
                    continue;}
                neighborNodes.push_back(&mat[x+offX][y+offY]); 
                
            }
        }
        //cout<<endl;
        neighborNodes.shrink_to_fit();
        //std::cout<<neighborNodes.size()<<" "<<"Neighbor Size"<<endl;
        for (auto neighbor : neighborNodes) {
            //std::cout << neighbor->cost<<" ";
            if(listContains(closed,neighbor))
                continue;

            bool inOpen = listContains(open, neighbor);
            int neighborCost =  currentNode->inital + currentNode->get_distance(*neighbor);    // current cost + cost to neighbor
            if(!inOpen || neighborCost < neighbor->inital){
                neighbor->set_inital(neighborCost);
                neighbor->parent = currentNode;
                neighbor->nType.color = GREEN;

                if(!inOpen){
                    neighbor->set_heuristic(*end);
                    open.push_front(neighbor);
                }
            }
            
        }

    }

    
    
    
    
    
    return;
    /*
    set<Node*,Compare> open;    // Visited but not expanded
    set<Node*,Compare> closed;  // Visited and expanded
    //priority_queue<Node*,vector<Node*>,Compare> open;   // Visited but not expanded
    //priority_queue<Node*,vector<Node*>,Compare> closed; // Visited and expanded
    //cout<<(mat[5][25]).inital<<endl;
    start->set_inital(0);
    start->set_heuristic(*end);
    open.insert(start);
    end->heuristic = 0;
    end->set_inital(*start);
    Node* test;
    test = &mat[5][25];
    //cout<<(mat[5][25]).inital<<endl;
    //cout<<start->inital<<endl;
    //cout<<(test == start)<<"test"<<endl;
    std::cout<<start->get_distance(*end)<<" Distance"<<endl;

    
    
    //for (set<Node*,Compare>::iterator i =open.begin();i!=open.end();i++) { 
        //std::cout<<((*i)->cost)<<endl;
    //}

    std::cout<<(*open.rbegin())->cost<<endl;
    
    std::cout<<setContains(open,start)<<endl;
    std::cout<<"Contains start?"<<setContains(open,start)<<endl;
    std::cout<<"Contains end?"<<setContains(open,end)<<endl;

    Node* currentNode;
    std::cout<<*open.begin()<< " "<<start<<" "<<start->get_x()<<endl;
    std::cout<<end<<endl;
    int stop = 0;
    while(!open.empty()){
        // From the open list, grab the node with the smallest cost
        currentNode = *open.begin();
        // Move current node to closed queue
        closed.insert(currentNode);  
        //open.erase(currentNode);
        std::cout<< "open size "<<open.size()<<endl;
        std::cout<< "Erased "<<open.erase(currentNode)<<endl;
        std::cout<< "open size "<<open.size()<<endl;
        //std::cout<< "Contains? "<<setContains(open,currentNode)<<endl;
        //std::cout<< "open size "<<open.size()<<endl;
        stop++;
        //std::cout<<(*open.begin())->cost<<" "<<"Lowest F"<<endl;
        //std::cout<<(*open.rbegin())->cost<<" "<<"Highest F"<<endl;
        //Node* currentNodeAddress = *open.begin();
        
        
        if(currentNode == end){
            std::cout<<currentNode->parent->heuristic<<endl;
            break; // Path found
        }
        else if(stop == 500){
            std::cout<<"Limit reached"<<endl;
            break;
        }

        // Add all neighboring Nodes to a vector
        int x = currentNode->get_x();
        int y = currentNode->get_y();
        vector<Node*> neighborNodes(8);

        cout<<"matrix size: "<<mat.size()<<endl;
        for(int offX=-1,i=0;offX<=1;offX++){
            if(x+offX < 0 || x+offX > mat.size())
                continue;
            
            for(int offY=-1;offY<=1;offY++){
                if((offX == 0 && offY == 0) || y+offY < 0 || y+offY > mat[x].size())
                    continue;
                neighborNodes[i++] = &mat[x+offX][y+offY]; 
                
            }
        }
        cout<<"matrix size: "<<mat.size()<<endl;
        std::cout<<"neighbor size: "<<neighborNodes.size()<<endl;
        neighborNodes.shrink_to_fit();
        std::cout<<"neighbor size: "<<neighborNodes.size()<<endl;

        //return;
        //neighborNodes[0] = &mat[x][y-1];    // North
        //neighborNodes[1] = &mat[x+1][y-1];    // North-East
        //neighborNodes[2] = &mat[x+1][y];    // East
        //neighborNodes[3] = &mat[x+1][y+1];    // South-East
        //neighborNodes[4] = &mat[x][y+1];    // South
        //neighborNodes[5] = &mat[x-1][y+1];    // South-West
        //neighborNodes[6] = &mat[x-1][y];    // West
        //neighborNodes[7] = &mat[x-1][y-1];    // North-West

        //cout<<neighborNodes[3]->get_x()<<endl;
        for (auto neighbor : neighborNodes) { 
            //cout<<neighbor<< "Address"<<endl;
            int neighborCost =  currentNode->inital + currentNode->get_distance(*neighbor);    // current cost + cost to neighbor
            if(setContains(open,neighbor)){
                //cout<<"open"<<endl;
                if(neighbor->inital <= neighborCost){
                    continue;
                }
                else{
                    open.erase(neighbor);
                    neighbor->set_inital(neighborCost);
                    neighbor->parent = currentNode;
                    neighbor->nType.color = GREEN;
                    neighbor->nType.name = "PATH";
                    open.insert(neighbor);
                }
            }
            else if(setContains(closed,neighbor)){
                //cout<<"closed"<<endl;
                if(neighbor->inital <= neighborCost){
                    continue;
                }
                else{
                    closed.erase(neighbor);
                    neighbor->set_inital(neighborCost);
                    neighbor->parent = currentNode;
                    neighbor->nType.color = GREEN;
                    neighbor->nType.name = "PATH";
                    open.insert(neighbor);
                }
            }
            else{
                //cout<<"neither"<<endl;
                
                neighbor->set_heuristic(*end);
                neighbor->set_inital(neighborCost);
                std::cout<<neighbor->heuristic<<" H"<<endl;
                neighbor->parent = currentNode;
                neighbor->nType.color = GREEN;
                neighbor->nType.name = "PATH";
                open.insert(neighbor);
            }
            //neighbor->set_inital(neighborCost);
            //neighbor->parent = currentNode;
            //neighbor->nType.color = GREEN;
            //neighbor->nType.name = "PATH";
        } 
        //cout<<mat[x][y-1].cost<<" new cost"<<endl;
        
    }

    if(currentNode != end){
        std::cout<<"Error Path Not Found"<<endl;
    }

    return;
    */
}

int main () {
    std::cout << "Debugging Output" << endl << endl;

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
    start->nType.color = BLUE;
    end->nType.color = ORANGE;

    findPath(start,end,matrix);

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
	    	
        // Set new end point
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL)){
            end = &matrix[mousePositionX][mousePositionY];
            end->nType.color = ORANGE;
            findPath(start,end,matrix);
        }

	    //----------------------------------------------------------------------------------

	    // Draw
	    //----------------------------------------------------------------------------------
	    BeginDrawing();
	    ClearBackground(RAYWHITE);
        
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
        while(next != start){
            DrawRectangle(next->get_x_scaled(),next->get_y_scaled(),SCALE,SCALE,GREEN);
            next = next->parent;
            pathCount++;
        }

        // Print out mouse coordinates
        string mousePosition = "x: " + to_string(mousePositionX) + " y: " + to_string(mousePositionY);
        DrawText(mousePosition.c_str(),700,0,10,BLACK);
        // Print out steps taken
		DrawText(to_string(pathCount).c_str(), 30, 0, 20, BLACK);
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
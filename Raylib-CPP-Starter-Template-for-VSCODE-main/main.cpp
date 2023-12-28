#include <iostream>
#include <vector>
#include <queue>
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
    int g;
    int h;
    int f;
    public:
    Node* parent = NULL;
    NodeType nType;

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

    int get_x(){
        return x;
    }
    int get_y(){
        return y;
    }
    void set_x(int x_){
        x = (x_ * SCALE) + xOFFSET;
    }
    void set_y(int y_){
        y = (y_ * SCALE) + yOFFSET;
    }
};

void Findpath(Node start, Node end){
    queue<Node> toCheck;
    queue<Node> processed;


}

int main () {

    vector<vector<Node>> matrix(SCREENWIDTH / SCALE,vector<Node>(SCREENHEIGHT / SCALE));
    for(int w=0;w<(int)matrix.size();w++){
            for(int h=0;h<(int)matrix[w].size();h++){
                matrix[w][h].set_x(w);
                matrix[w][h].set_y(h);
            }
        }
    cout << "Hello World" << endl;
    Node* start = &matrix[5][25];
    Node* end = &matrix[30][5];
    start->nType.color = BLUE;
    end->nType.color = ORANGE;

    Node* test = &matrix[30][4];
    test->nType.color = GREEN;
    end->parent = test;




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
                DrawRectangle(matrix[w][h].get_x(),matrix[w][h].get_y(),SCALE,SCALE,matrix[w][h].nType.color);
            }
        }

        Node* next = end->parent;
        while(next != NULL){
            DrawRectangle(next->get_x(),next->get_y(),SCALE,SCALE,next->nType.color);
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
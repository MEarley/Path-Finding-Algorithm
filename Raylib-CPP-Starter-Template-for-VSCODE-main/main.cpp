#include <iostream>
#include <vector>
#include <raylib.h>

using namespace std;

const int SCALE = 10;
const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 450;
const int xOFFSET = SCREENWIDTH / 2;
const int yOFFSET = SCREENHEIGHT / 2;

#define SAND YELLOW

class Materials{
    public:
    Color color;
    string name;

    Materials(){
        color = WHITE;
        name = "void";
    }

    Materials(Color color_, string name_){
        color = color_;
        name = name_;
    }

    string to_string(){
        return name;
    }
};

class Particle{
    int x;
    int y;
    public:
    Materials material;

    Particle(){
        x = 0 + xOFFSET;
        y = 0 + yOFFSET;
        material = Materials();
    }

    Particle(Color color_,string name_){
        material.color = color_;
        material.name = name_;
    }

    Particle(int x_, int y_, Color color_,string name_){
        x = x_ + xOFFSET;
        y = y_ + yOFFSET;
        material.color = color_;
        material.name = name_;
    }

    string to_string(){
        return material.name;
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
        y = (-1 * y_ * SCALE) + yOFFSET;
    }
};

int main () {

    vector<vector<Particle>> matrix(SCREENWIDTH / SCALE,vector<Particle>(SCREENHEIGHT / SCALE));

    cout << "Hello World" << endl;
    matrix[0][0].material.color = RED;
    matrix[0][0].set_x(39);
    matrix[0][0].set_y(2);

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
	    ClearBackground(BLUE);

        for(int w=0;w<matrix.size();w++){
            for(int h=0;h<matrix[w].size();h++){
                DrawRectangle(matrix[w][h].get_x(),matrix[w][h].get_y(),SCALE,SCALE,matrix[w][h].material.color);
            }
        }
		
	    DrawText("Mapping Bot", 190, 200, 20, BLACK);
		
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
#pragma once
class Window;

class LoadingScreenRenderer {
public:
    LoadingScreenRenderer(Window* window);
    // init() function will initialize shaders and text renderer. 
	// This function must be called after window creation, because 
    // it depends on OpenGL being loaded.
    void init();
    void render();
    void cleanup();

private:
    Window* window;
};

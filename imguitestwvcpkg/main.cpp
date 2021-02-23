// C++
#include <string>
#include <iostream>

// GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Dear imgui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
//#include <imgui_impl_opengl3.cpp>
#include <imgui_impl_glfw.h>

using namespace std;
using namespace ImGui;

void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(int argc, char* argv[]) {
	// Initialize glfw
	cout << "Initializing GLFW" << endl;
	if (!glfwInit()) {
		cout << "GLFW Initialization Failed!" << endl;
		return 0;
	}

	glfwSetErrorCallback(error_callback);


	// Setup Dear ImGui
	cout << "Initializing Dear ImGui" << endl;
	IMGUI_CHECKVERSION();
	CreateContext();
	ImGuiIO& io = GetIO();
	ImFont* arial = io.Fonts->AddFontFromFileTTF("arial.ttf", 40);

	// Create window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	const float windowWidth = 2640, windowHeight = 2480, settingsWidth = 800;
	const ImVec2 settingSpace = ImVec2(0.0f, 20.0f);
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Amber Alpha", NULL, NULL);
	if (!window) {
		cout << "Window Open Failure!" << endl;
		return 0;
	}
	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	gladLoadGL(/*glfwGetProcAddress*/);
	glfwSwapInterval(1);

	// Imgui Window Init
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Setup ImGui style
	StyleColorsDark();

	int width, height;

	double time = glfwGetTime();

	bool* open = new bool(true);
	bool* tagged = new bool(false);
	bool* passworded = new bool(false);
	int* endos = new int();
	bool* execDel = new bool(true);
	bool* founder = new bool(false);
	int* numTeams = new int();
	int* trigPop = new int();
	bool* trigPass = new bool(false);
	bool* birbFind = new bool(false);
	bool* findTrig = new bool(true);

	bool oTagged = *tagged;
	bool oPassed = *passworded;
	int oEndos = *endos;
	bool oExecDel = *execDel;
	bool oFounder = *founder;
	int oNumTeams = *numTeams;
	int oTrigPop = *trigPop;
	bool oTrigPass = *trigPass;
	bool oBirbFind = *birbFind;
	bool oFindTrig = *findTrig;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(.45f, .55f, .6f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);

		// start new imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		NewFrame();

		// render gui
		bool oTagged = *tagged;
		bool oPassed = *passworded;
		int oEndos = *endos;
		bool oExecDel = *execDel;
		bool oFounder = *founder;
		int oNumTeams = *numTeams;
		int oTrigPop = *trigPop;
		bool oTrigPass = *trigPass;
		bool oBirbFind = *birbFind;
		bool oFindTrig = *findTrig;

		io.DisplaySize = ImVec2(settingsWidth, windowHeight - 100);
		Begin("Operation Settings", open);

		Checkbox("Regions Should be Tagged", tagged);
		if(oTagged != *tagged) cout << "Variable tagged set to " << *tagged << endl; 
		Dummy(settingSpace);

		Checkbox("Regions Should be Passworded", passworded);
		if (oPassed != *passworded) cout << "Variable passworded set to " << *passworded << endl;
		Dummy(settingSpace);

		Checkbox("Regions Should Have Executive Delegates", execDel);
		if (oExecDel != *execDel) cout << "Variable execDel set to " << *execDel << endl;
		Dummy(settingSpace);

		Checkbox("Regions Should Have a Founder", founder);
		if (oFounder != *founder) cout << "Variable founder set to " << *founder << endl;
		Dummy(settingSpace);

		Checkbox("Amber Should Find Trigger Regions", findTrig);
		if (oFindTrig != *findTrig) cout << "Variable findTrig set to " << *findTrig << endl;
		Dummy(settingSpace);

		Checkbox("Trigger Regions Should be Passworded", passworded);
		if (oTrigPass != *trigPass) cout << "Variable trigPass set to " << *trigPass << endl;
		Dummy(settingSpace);

		Checkbox("Enable Birb AutoFind", birbFind);
		if (oBirbFind != *birbFind) cout << "Variable birbFine set to " << *birbFind << endl;
		Dummy(settingSpace);

		PushItemWidth(settingsWidth / 4);
		InputInt("Number of Endorsements", endos, 1, 5);
		if (oEndos != *endos) cout << "Variable endos set to " << *endos << endl;
		Dummy(settingSpace);

		InputInt("Number of Teams", numTeams, 1, 5);
		if (oNumTeams != *numTeams) cout << "Variable numTeams set to " << *numTeams << endl;
		Dummy(settingSpace);

		InputInt("Maximum Trigger Region Population", trigPop, 1, 5);
		if (oTrigPop != *trigPop) cout << "Variable trigPop set to " << *trigPop << endl;
		Dummy(settingSpace);

		PopItemWidth();
		End();

		// render imgui
		Render();
		ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glfwSwapBuffers(window);

	}


	glfwDestroyWindow(window);
	glfwTerminate();
	delete open;

	return 0;
}


void error_callback(int error, const char* description) { cout << "Error " << error << ": " << description << endl; }

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}
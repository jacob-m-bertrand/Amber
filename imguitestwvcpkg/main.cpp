// C++
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <regex>
#include <unordered_set>

using namespace std;

// Classes
#include "SettingsProfile.h"
/*#include "C:\Users\jacob\Desktop\NationStates\TargetFinder\TargetFinder\Region.h"
#include "List.h"*/

// GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Dear imgui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

namespace ImGui {
	// ImGui::InputText() with std::string
	// Because text input needs dynamic resizing, we need to setup a callback to grow the capacity
	IMGUI_API bool  InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
	IMGUI_API bool  InputTextMultiline(const char* label, std::string* str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
	IMGUI_API bool  InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
}
using namespace ImGui;


int main(int argc, char* argv[]);

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
	const float windowWidth = 2640, windowHeight = 2000, settingsWidth = 1200, listWidth = 1200;
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
	
	
	SettingsProfile<int> ints;
	SettingsProfile<bool> bools;
	
	bools.setSetting(TAGGED, new bool(false));
	bools.setSetting(PASSWORDED, new bool(false));
	ints.setSetting(ENDOS, new int());
	bools.setSetting(EXEC_DEL, new bool(true));
	bools.setSetting(FOUNDER, new bool(false));
	ints.setSetting(NUM_TEAMS, new int());
	ints.setSetting(TRIG_POP, new int());
	bools.setSetting(TRIG_PASS, new bool(false));
	bools.setSetting(BIRB_FIND_ENABLED, new bool(false));
	bools.setSetting(FIND_TRIG, new bool(true));
	ints.setSetting(TRIGGER, new int());
	ints.setSetting(SEPARATION, new int());
	string* input = new string();
	string* output = new string();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(.45f, .55f, .6f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);

		// start new imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		NewFrame();


		//io.DisplaySize = ImVec2(settingsWidth, windowHeight - 100);
		SetNextWindowSize({ settingsWidth, windowHeight - 100 });
		SetNextWindowPos({ 50, 50 });
		Begin("Operation Settings", open, ImGuiWindowFlags_NoMove);
			Checkbox("Regions Should be Tagged", bools[TAGGED]);
			Dummy(settingSpace);

			Checkbox("Regions Should be Passworded", bools[PASSWORDED]);
			Dummy(settingSpace);

			Checkbox("Regions Should Have Executive Delegates", bools[EXEC_DEL]);
			Dummy(settingSpace);

			Checkbox("Regions Should Have a Founder", bools[FOUNDER]);
			Dummy(settingSpace);

			Checkbox("Amber Should Find Trigger Regions", bools[FIND_TRIG]);
			Dummy(settingSpace);

			Checkbox("Trigger Regions Should be Passworded", bools[PASSWORDED]);
			Dummy(settingSpace);

			Checkbox("Enable Birb AutoFind", bools[BIRB_FIND_ENABLED]);
			Dummy(settingSpace);

			PushItemWidth(settingsWidth / 4);

			InputInt("Number of Endorsements", ints[ENDOS], 1, 5);
			Dummy(settingSpace);

			InputInt("Number of Teams", ints[NUM_TEAMS], 1, 5);
			Dummy(settingSpace);

			InputInt("Maximum Trigger Region Population", ints[TRIG_POP], 1, 5);
			Dummy(settingSpace);

			InputInt("Maximum Trigger Region Population", ints[TRIGGER], 1, 5);
			Dummy(settingSpace);

			InputInt("Maximum Trigger Region Population", ints[SEPARATION], 1, 5);
			Dummy(settingSpace);

			if (Button("Start Target Finding")) {
				cout << "Whoa its go time" << endl;
			}

			PopItemWidth();
		End();

		SetNextWindowSize({ listWidth, (windowHeight - 100) / 2 });
		SetNextWindowPos({ windowWidth - 50 - listWidth, 50 });
		Begin("Input", open);
			PushItemWidth(settingsWidth);
		
			InputTextMultiline("", input, {settingsWidth, (windowHeight - 230) / 2 });
			cout << *input << endl;
			PopItemWidth();
		End();

		SetNextWindowSize({ listWidth, ((windowHeight - 100) / 2) });
		SetNextWindowPos({ windowWidth - 50 - listWidth, ((windowHeight - 100) / 2) + 100});
		Begin("List", open);
			PushItemWidth(settingsWidth);
			InputTextMultiline("", input, { settingsWidth, (windowHeight - 230) / 2 });
			cout << *input << endl;
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
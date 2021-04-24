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
#include <time.h>
#include <urlmon.h>

using namespace std;

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

// other
#pragma comment(lib, "Urlmon.lib")

// Boost
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

// Pugixml
#include <pugixml.hpp>

// Classes
#include "SettingsProfile.h"
#include "Region.h"
#include "List.h"

int main(int argc, char* argv[]);

void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void ToggleButton(const char* str_id, bool* v);

void unzip();

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
	io.Fonts->AddFontFromFileTTF("arial.ttf", 18);

	// Create window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	const float margin = 10;
	const float middleSpacing = 10;

	int* currentWinWidth = new int(0);
	int* currentWinHeight = new int(0);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	float settingsWidth, settingsHeight, inputWidth, inputHeight, outputWidth, outputHeight;
	float windowWidth = screenWidth / 2;
	float windowHeight = screenHeight / 2;

	const ImVec2 settingSpace = ImVec2(0.0f, 10.0f);
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Amber Beta", NULL, NULL);
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
	StyleColorsLight();

	int width, height;

	double time = glfwGetTime();

	
	bool* open = new bool(true);
	
	
	SettingsProfile<int> ints;
	SettingsProfile<bool> bools;
	
	bools.setSetting(DETAGS, new bool(true));
	bools.setSetting(PASSWORDED, new bool(false));
	ints.setSetting(ENDOS, new int(2));
	bools.setSetting(EXEC_DEL, new bool(true));
	bools.setSetting(FOUNDER, new bool(false));
	ints.setSetting(NUM_TEAMS, new int(1));
	ints.setSetting(TRIG_POP, new int(40));
	bools.setSetting(TRIG_PASS, new bool(false));
	bools.setSetting(BIRB_FIND_ENABLED, new bool(false));
	bools.setSetting(FIND_TRIG, new bool(true));
	ints.setSetting(TRIGGER, new int(10));
	ints.setSetting(SEPARATION, new int(60));
	ints.setSetting(NUM_TARGETS, new int(50));
	ints.setSetting(DELAY, new int(60));
	bools.setSetting(CRAZY_TAGGING, new bool(false));
	bools.setSetting(MAJOR, new bool(true));
	bools.setSetting(MINOR, new bool(false));
	string* input = new string();
	string* output = new string();
	bool* infoOn = new bool(false);
	string oldInput;

	// Set up advanced tag checking
	URLDownloadToFile(NULL, L"https://www.nationstates.net/pages/regions.xml.gz", L"regions.xml.gz", 0, NULL);
	unzip();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(.984f, .589f, .015f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);

		// start new imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		NewFrame();


		glfwGetWindowSize(window, currentWinWidth, currentWinHeight);
		windowWidth = *currentWinWidth;
		windowHeight = *currentWinHeight;
		settingsWidth = (windowWidth / 2) - margin - middleSpacing;
		settingsHeight = windowHeight - (2 * margin);
		inputWidth = settingsWidth;
		inputHeight = (windowHeight / 2) - margin - middleSpacing;
		outputWidth = settingsWidth;
		outputHeight = (windowHeight / 2) - margin - middleSpacing;


		SetNextWindowSize({ settingsWidth, settingsHeight });
		SetNextWindowPos({ margin, margin });
		Begin("Operation Settings", open, ImGuiWindowFlags_NoMove);
			Checkbox("Doing Detags", bools[DETAGS]);
			Dummy(settingSpace);

			Checkbox("Enable Crazy Tagging Mode", bools[CRAZY_TAGGING]);
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

			Checkbox("Major", bools[MAJOR]);
			if (*bools[MAJOR]) *bools[MINOR] = false;

			Checkbox("Minor", bools[MINOR]);
			if (*bools[MINOR]) *bools[MAJOR] = false;
			Dummy(settingSpace);

			PushItemWidth(settingsWidth / 4);

			InputInt("Number of Endorsements", ints[ENDOS], 1, 5);
			Dummy(settingSpace);

			InputInt("Number of Teams", ints[NUM_TEAMS], 1, 5);
			Dummy(settingSpace);

			InputInt("Maximum Trigger Region Population", ints[TRIG_POP], 1, 5);
			Dummy(settingSpace);

			InputInt("Trigger Time", ints[TRIGGER], 1, 500);
			Dummy(settingSpace);

			InputInt("Separation Time", ints[SEPARATION], 1, 500);
			Dummy(settingSpace);

			InputInt("Number of Targets", ints[NUM_TARGETS], 1, 500);
			Dummy(settingSpace);

			InputInt("Delay Time", ints[DELAY], 1, 500);
			Dummy(settingSpace);

			if (Button("Start Target Finding")) {
				List list(input, ints, bools);
				cout << "Making list" << endl;
				list.advancedTagChecking();
				list.makeList();
				cout << "Done." << endl;
				output = list.getList();
			}

			Dummy(settingSpace);
			Dummy(settingSpace);
			bool oldInfoOn = *infoOn;
			Checkbox("Show Information Window", infoOn);
			if (oldInfoOn != *infoOn) *input = "";
			if (*infoOn) {
				oldInput = *input;
				*input = "This is Amber, the North Pacific Army's tool for list generation. It was made by BMWSurfer in 2021 at the";
				*input += " request of Nimarya, then Minister of Defense.\n\nTo your left are a series of options, and in this box is";
				*input += " (usually) where you paste in a spyglass list. To do this, select all the columns with data, copy them, ";
				*input += "then paste them in the box. Below this box is where the list will output.\n\nIf you have any problems, ";
				*input += "please notify BMWSurfer at bmwsurferns@gmail.com or on discord.\n\n\n\n Version b1.0.0";
			}

			PopItemWidth();
		End();

		SetNextWindowSize({ inputWidth, inputHeight });
		SetNextWindowPos({ windowWidth - margin - inputWidth, margin });
		Begin(((*infoOn) ? "Information" : "Input"), open);
			PushItemWidth(inputWidth);
		
			if(*infoOn) TextWrapped(input->c_str());
			else InputTextMultiline("", input, { inputWidth, inputHeight });
			PopItemWidth();
		End();

		SetNextWindowSize({ outputWidth, outputHeight });
		SetNextWindowPos({ windowWidth - outputWidth - margin, windowHeight - margin - outputHeight});
		Begin("List", open);
			PushItemWidth(outputWidth);
			InputTextMultiline("", output, { outputWidth, outputHeight });
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


void unzip() {
	//Read from the first command line argument, assume it's gzipped
	ifstream file("regions.xml.gz", ios_base::in | ios_base::binary);

	boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;

	inbuf.push(boost::iostreams::gzip_decompressor());
	inbuf.push(file);

	//Convert streambuf to istream
	istream instream(&inbuf);

	ofstream fout("regions.xml");

	//Copy everything from instream to 
	fout << instream.rdbuf();

	//Cleanup
	file.close();
}

void ToggleButton(const char* str_id, bool* v)
{
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float height = ImGui::GetFrameHeight();
	float width = height * 1.55f;
	float radius = height * 0.50f;

	if (ImGui::InvisibleButton(str_id, ImVec2(width, height)))
		*v = !*v;
	ImU32 col_bg;
	if (ImGui::IsItemHovered())
		col_bg = *v ? IM_COL32(145 + 20, 211, 68 + 20, 255) : IM_COL32(218 - 20, 218 - 20, 218 - 20, 255);
	else
		col_bg = *v ? IM_COL32(145, 211, 68, 255) : IM_COL32(218, 218, 218, 255);

	draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
	draw_list->AddCircleFilled(ImVec2(*v ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}
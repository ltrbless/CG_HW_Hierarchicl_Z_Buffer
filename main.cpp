#include <stdio.h>
#include <Windows.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "MeshIO.h"
#include "Render.h"
#include "ZBuffer.h"
#include "ScanLineZBuffer.h"
#include "Tool.h"
#include "Eigen/Dense"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl2.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
// Opengl & Imgui 初始化
void UI_Init(GLFWwindow* window);

// OPengl & Imgui clearn up
void UI_Clear(GLFWwindow* window);

// Imgui prepare render
void Imgui_Init();

// Help
void Imgui_Help();

// 是否绘制
#define _RENDER_

int main()
{
	int display_w = 1280, display_h = 720;
#ifdef _RENDER_
	if (!glfwInit())  return 0;
	GLFWwindow* window = glfwCreateWindow(display_w, display_h, "Z-Buffer", NULL, NULL);
	UI_Init(window);
#endif

	Vector4unchar backgroundcolor(0, 0, 0, 255);
	// Vector4unchar backgroundcolor(0.45f * 255, 0.55f * 255, 0.60f * 255, 255);

	MeshIO meshio;
	int curmeshid;

	curmeshid = meshio.ReadObjFile("../extern/file/bunny.obj");
	meshio.SetColorById(curmeshid, Vector4unchar(0.45 * 255, 0.55 * 255, 0.60 * 255, 255));
	// meshio.SetColorById(curmeshid, Vector4unchar(255, 255, 255, 255));
	meshio.Scale(curmeshid, 3);
	meshio.SetLocation(curmeshid, Vec3d(0, 0, 0));

	// curmeshid = meshio.ReadObjFile("../extern/file/bunny.obj");
	// meshio.SetColorById(curmeshid, Vector4unchar(0.8 * 255, 0.3 * 255, 0.60 * 255, 255));
	// // meshio.SetColorById(curmeshid, Vector4unchar(255, 255, 255, 255));
	// meshio.Scale(curmeshid, 2);
	// meshio.SetLocation(curmeshid, Vec3d(0.3, 0.3, 0.3));


	Render* render = new ZBufferRender(display_w, display_h, backgroundcolor);
	// Render* render = new ScanlineZBuffer(display_w, display_h, backgroundcolor);

	render->RenderAllObj(Vec3d(-1.0, -1.0, -1.0), meshio.alltrimesh_);

#ifdef _RENDER_
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// User interface
		{
			Imgui_Init();
			ImGui::Begin("Z-buffer Setting");                        
			Imgui_Help();
			ImGui::End();
			ImGui::Render();
		}
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(backgroundcolor(0), backgroundcolor(1), backgroundcolor(2), backgroundcolor(3));
		glClear(GL_COLOR_BUFFER_BIT);


		// frameBuffer = new unsigned char [display_h * display_w * 3]; // 定义帧缓冲区

		glDrawPixels(display_w, display_h, GL_RGB, GL_UNSIGNED_BYTE, render->framebuffer_);

		// delete frameBuffer;

		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData()); //必须在绘制完Open之后接着绘制Imgui
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	// Cleanup
	UI_Clear(window);
#endif

	return 0;
}


void UI_Init(GLFWwindow* window)
{
    if(window == nullptr)
    {
        std::cout << "OpenGl window init fail!" << std::endl;
        abort();
    }
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL2_Init();
}

void Imgui_Init()
{
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowBgAlpha(-1.3); // 设置透明度为 0.3
}

void Imgui_Help()
{
    //ImGui::Checkbox("Demo Window", &globalVar::show_demo_window);      // Edit bools storing our window open/close state
    //ImGui::Checkbox("Another Window", &globalVar::show_another_window);
    if (ImGui::CollapsingHeader("Help"))
    {
        ImGui::Text("By Taoran Liu");
        ImGui::BulletText("Time: 2020.10.13");
        ImGui::BulletText("Email: taoranliu@zju.edu.cn");
        ImGui::BulletText("Phone: 19550210570");
        ImGui::Separator();

        ImGui::Text("USER GUIDE:");
        ImGui::BulletText("Press 'ESC' to close current window.");
        ImGui::BulletText("When you right-click one you can drag the model and twice is quit.");
        ImGui::BulletText("When you middle-click one you can scale the model and twice is quit.");
        ImGui::Separator();
        //ImGui::ShowUserGuide("");
    }
}

void UI_Clear(GLFWwindow* window)
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}


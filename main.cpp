#include <stdio.h>
#include <Windows.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "MeshIO.h"
#include "Render.h"
#include "ZBuffer.h"
#include "ScanLineZBuffer.h"
#include "HierarchicalZBuffer.h"
#include "OctreeHZBuffer.h"
#include "Tool.h"
#include "Eigen/Dense"
#include "Debug.h"
#include <sstream>
#include <cstring>
#include <fstream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl2.h"
#include<ctime>

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

double initdeep = -1000000;
clock_t startTime = clock();
clock_t endTime = clock();

int main(int argc, char** argv)
{
	int display_w = 1280, display_h = 720;
	// int display_w = 512, display_h = 512;
#ifdef _RENDER_
	if (!glfwInit())  return 0;
	GLFWwindow* window = glfwCreateWindow(display_w, display_h, "Z-Buffer", NULL, NULL);
	UI_Init(window);
#endif

	Vector4unchar backgroundcolor(0, 0, 0, 255);
	// Vector4unchar backgroundcolor(0.45f * 255, 0.55f * 255, 0.60f * 255, 255);

	MeshIO meshio;
	int curmeshid;
	// srand(1);
	// std::string path = "../extern/file/bunny_1k.obj";
	// if(argc > 1)
	// {
	// 	path = argv[1];
	// }
	// else
	// {
	// 	std::cout << "The fault path is be used because not design file path." << "\n";
	// }

	// double sx = -0.4;
	// double sy = -0.4;
	// double sz = 0.5;
	// double dx = 0.01;
	// double dy = 0.01;
	// double dz = -2;
	// for(int i = 0; i < 30; i++)
	// {
	// 	double random[3];
	// 	for(int j = 0; j < 3; j++)
	// 	{
	// 		random[j] = rand() % 1000 / 1000.0;
	// 	}
	// 	curmeshid = meshio.ReadObjFile(path);
	// 	meshio.SetColorById(curmeshid, Vector4unchar(random[0] * 255, random[1] * 255, random[2] * 255, 255));
	// 	meshio.Scale(curmeshid, 4);
	// 	double rd = rand() % 1000 / 1000.0 - 0.5;
	// 	meshio.SetLocation(curmeshid, Vec3d(sx, sy, sz));
	// 	sx += dx;
	// 	sy += dy;
	// 	sz += dz;
	// }


	// curmeshid = meshio.ReadObjFile("../extern/file/test.obj");
	// meshio.Scale(curmeshid, 0.3);
	// meshio.SetLocation(curmeshid, Vec3d(-0.5, 0.5, -1));

	// INFO_log("path is %s\n", path.c_str());

	// curmeshid = meshio.ReadObjFile("../extern/file/bunny_40k.obj");
	// meshio.SetColorById(curmeshid, Vector4unchar(0.8 * 255, 0.3 * 255, 0.60 * 255, 255));
	// // meshio.SetColorById(curmeshid, Vector4unchar(255, 255, 255, 255));
	// meshio.Scale(curmeshid, 2);
	// meshio.SetLocation(curmeshid, Vec3d(0, 0, -2));

	// Render* render = new ZBufferRender(display_w, display_h, backgroundcolor);
	// Render* renderZ = new ZBufferRender(display_w, display_h, backgroundcolor);
	// Render* renderS = new ScanlineZBuffer(display_w, display_h, backgroundcolor);
	// Render* renderHZ = new HZBuffer(display_w, display_h, backgroundcolor);
	// // Render* render = new OctHZBuffer(display_w, display_h, backgroundcolor, 5);
	// clock_t endTime;
    // startTime = clock();
	// renderZ->RenderAllObj(Vec3d(0, 0, 1), meshio.alltrimesh_);
	// endTime = clock();
    // std::cout << "ZBUFFER The model is : " << path << " the run time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;

	// endTime;
    // startTime = clock();
	// renderS->RenderAllObj(Vec3d(0, 0, 1), meshio.alltrimesh_);
	// endTime = clock();
    // std::cout << "SZBUFFER The model is : " << path << " the run time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;

	// endTime;
    // startTime = clock();
	// renderHZ->RenderAllObj(Vec3d(0, 0, 1), meshio.alltrimesh_);
	// endTime = clock();
    // std::cout << "HZBUFFER The model is : " << path << " the run time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;

#ifdef _RENDER_
	static int render_way = 0;
	static ImVec4 Color = ImVec4(0,0,0,1);
	Render* render = nullptr;
	Vector4unchar objColor;
	static char filepath[1000] = ""; 
	double* scale = new double[1];
	*scale = 1.0;
	int pre = -1;
	char* name[4] = {"Z_Buffer", "ScanLine_Z_Buffer", "Hierarchical_Z_Buffer", "Octree_Hierarchical_Z_Buffer"};

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// User interface
		
		Imgui_Init();
		ImGui::Begin("Z-buffer Setting");     
		// ImGui::ShowDemoWindow();                   
		Imgui_Help();

		
		if(ImGui::CollapsingHeader("Setting"))
		{
			ImGui::BulletText("Please input obj file path.");
			ImGui::InputText("(.obj)", filepath, 1000);
			ImGui::Separator();
			ImGui::BulletText("Set Obj Color.");
			ImGui::ColorEdit3("color", &Color.x); 
			objColor.x() = Color.x * 255;
			objColor.y() = Color.y * 255;
			objColor.z() = Color.z * 255;
			objColor.w() = Color.w * 255;

			ImGui::InputDouble("Double", scale);


			ImGui::Separator();
			ImGui::Text("Choose render way.");
			ImGui::Separator();
			ImGui::RadioButton("Z-Buffer", &render_way, 0); 
			ImGui::Separator();
			ImGui::RadioButton("Scanline Z-Buffer", &render_way, 1); 
			ImGui::Separator();
			ImGui::RadioButton("Hierarchical Z-Buffer", &render_way, 2);
			ImGui::Separator();
			ImGui::RadioButton("Octree Hierarchical Z-Buffer", &render_way, 3);
			ImGui::Separator();

			if(pre != render_way)
			{
				if(render_way == 0)
				{
					pre = render_way;
					INFO_log("Curent render way is Z-Buffer");
					if(render != nullptr) delete render;
					render = new ZBufferRender(display_w, display_h, backgroundcolor);
					// startTime = clock();
					// render->RenderAllObj(Vec3d(0, 0, 1), meshio.alltrimesh_);
					// endTime = clock();
					// std::cout << "HZBUFFER The model is : " << path << " the run time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;
				}
				if(render_way == 1)
				{
					pre = render_way;
					INFO_log("Curent render way is Scanline Z-Buffer");
					if(render != nullptr) delete render;
					render = new ScanlineZBuffer(display_w, display_h, backgroundcolor);
				}
				if(render_way == 2)
				{
					pre = render_way;
					INFO_log("Curent render way is Hierarchical Z-Buffer");
					if(render != nullptr) delete render;
					render = new HZBuffer(display_w, display_h, backgroundcolor);
				}
				if(render_way == 3)
				{
					pre = render_way;
					INFO_log("Curent render way is Octree Hierarchical Z-Buffer");
					if(render != nullptr) delete render;
					render = new OctHZBuffer(display_w, display_h, backgroundcolor, 3);
				}
			}

			if (ImGui::Button("Render"))
			{
				{
					meshio.id = -1;
					meshio.alltrimesh_.clear();
					render->ClearDeepBuffer();
					render->ClearFrameBuffer();
				}
				curmeshid = meshio.ReadObjFile(filepath);
				if(curmeshid != -1)
				{
					std::cout << curmeshid << " " << meshio.alltrimesh_.size() << " " << meshio.alltrimesh_[0].vecTopos.size() << "\n";
					meshio.SetColorById(curmeshid, objColor);
					meshio.SetLocation(curmeshid, Vec3d(0, 0, 0));
					meshio.Scale(curmeshid, *scale);
					// INFO_log("This obj finish read and have %d facets.", meshio.alltrimesh_[curmeshid - 1].vecTopos.size());
					startTime = clock();
					render->RenderAllObj(Vec3d(0, 0, 1), meshio.alltrimesh_);
					endTime = clock();
					// std::cout << "HZBUFFER The model is : " << filepath << " the run time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;
					INFO_log("%s way render run time is : %.3f s.", name[render_way], (double)(endTime - startTime) / CLOCKS_PER_SEC);
				} 
			}

			if (meshio.id > -1 && ImGui::Button("Clear"))
			{
				meshio.id = -1;
				meshio.alltrimesh_.clear();
				render->ClearDeepBuffer();
				render->ClearFrameBuffer();
			}
			
			ImGui::Separator();
	
		}

		if (ImGui::CollapsingHeader("Log"))
		{
			static int test_type = 0;
			static ImGuiTextBuffer log;
			static int lines = 0;
			if (ImGui::Button("Clear log")) {
				errno_t err;
				FILE *fp = nullptr;
				err = fopen_s(&fp, "Info.log", "w");
				fclose(fp);
				log.clear();
				lines = 0;
			}

			ImGui::Separator();
			std::ifstream info("Info.log");
			std::string data = "";
			for(int i = 0; std::getline(info, data); i++)
			{
				log.appendf("%s\n",data.c_str());
				data.clear();
			}
			info.close();


			ImGui::BeginChild("Log");
			ImGui::TextUnformatted(log.begin(), log.end());
			log.clear();
			ImGui::EndChild();
		}

		ImGui::End();
		

		/////////////////////////////////////////////////////////////////////
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(backgroundcolor(0), backgroundcolor(1), backgroundcolor(2), backgroundcolor(3));
		glClear(GL_COLOR_BUFFER_BIT);


		// frameBuffer = new unsigned char [display_h * display_w * 3]; // 定义帧缓冲区

		if(meshio.id > -1)
		{ 
			glDrawPixels(display_w, display_h, GL_RGB, GL_UNSIGNED_BYTE, render->framebuffer_);
		}

		// delete frameBuffer;

		ImGui::Render();
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
    ImGui::SetNextWindowBgAlpha(0.3); // 设置透明度为 0.3
}

void Imgui_Help()
{
    //ImGui::Checkbox("Demo Window", &globalVar::show_demo_window);      // Edit bools storing our window open/close state
    //ImGui::Checkbox("Another Window", &globalVar::show_another_window);
    if (ImGui::CollapsingHeader("Help"))
    {
        ImGui::Text("By Taoran Liu");
        ImGui::BulletText("Time: 2022.01.01");
        ImGui::BulletText("Email: taoranliu@zju.edu.cn");
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


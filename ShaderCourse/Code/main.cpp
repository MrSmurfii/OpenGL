#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Material.h"
#include "Mesh.h"
#include "Transform.h"
#include "Texture.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "Screen.h"
#include "CubeMap.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Sphere.h"

bool shouldQuit = false;
bool keyState[GLFW_KEY_LAST] = { false };
bool cameraMovementActive = false;

Camera camera;
double mouseX = 0.0;
double mouseY = 0.0;

Mesh quadMesh;
Mesh cubeMesh;
Mesh triangleMesh;
Texture crate;
Texture cheryl;
Material defaultMaterial;
Material skyboxMaterial;
Material lightSourceMaterial;
Material reflectionMaterial;
Transform playerTransform;
CubeMap skyboxTexture;
PointLight pointLight;
PointLight pointLight_2; 
SpotLight spotLight;
Sphere sphere;
Transform sphereTransform;


const float cubeVertexData[] = {
	//Front face
	-1.f, -1.f, +1.f,		0.f, 0.f, 1.f,		0.f, 0.f, // 0
	+1.f, -1.f, +1.f,		0.f, 0.f, 1.f,		1.f, 0.f, // 1
	+1.f, +1.f, +1.f,		0.f, 0.f, 1.f,		1.f, 1.f, // 2
	-1.f, +1.f, +1.f,		0.f, 0.f, 1.f,		0.f, 1.f, // 3
															 
	//back face												 
	-1.f, -1.f, -1.f,		0.f, 0.f, -1.f,		0.f, 0.f, // 4
	+1.f, -1.f, -1.f,		0.f, 0.f, -1.f,		1.f, 0.f, // 5
	+1.f, +1.f, -1.f,		0.f, 0.f, -1.f,		1.f, 1.f, // 6
	-1.f, +1.f, -1.f,		0.f, 0.f, -1.f,		0.f, 1.f, // 7

	// right face
	+1.f, -1.f, +1.f,		1.f, 0.f, 0.f,		0.f, 0.f, // 8
	+1.f, -1.f, -1.f,		1.f, 0.f, 0.f,		1.f, 0.f, // 9
	+1.f, +1.f, -1.f,		1.f, 0.f, 0.f,		1.f, 1.f, // 10
	+1.f, +1.f, +1.f,		1.f, 0.f, 0.f,		0.f, 1.f, // 11

	// left face
	-1.f, -1.f, +1.f,		-1.f, 0.f, 0.f,		0.f, 0.f, // 12
	-1.f, -1.f, -1.f,		-1.f, 0.f, 0.f,		1.f, 0.f, // 13
	-1.f, +1.f, -1.f,		-1.f, 0.f, 0.f,		1.f, 1.f, // 14
	-1.f, +1.f, +1.f,		-1.f, 0.f, 0.f,		0.f, 1.f, // 15

	// top face
	-1.f, +1.f, +1.f,		0.f, 1.f, 0.f,		0.f, 0.f, // 16
	+1.f, +1.f, +1.f,		0.f, 1.f, 0.f,		1.f, 0.f, // 17
	+1.f, +1.f, -1.f,		0.f, 1.f, 0.f,		1.f, 1.f, // 18
	-1.f, +1.f, -1.f,		0.f, 1.f, 0.f,		0.f, 1.f, // 19

	// bottom face
	-1.f, -1.f, +1.f,		0.f, -1.f, 0.f,		0.f, 0.f, // 20
	+1.f, -1.f, +1.f,		0.f, -1.f, 0.f,		1.f, 0.f, // 21
	+1.f, -1.f, -1.f,		0.f, -1.f, 0.f,		1.f, 1.f, // 22
	-1.f, -1.f, -1.f,		0.f, -1.f, 0.f,		0.f, 1.f, // 23

};

const unsigned int cubeIndexData[] = {
	// front
	0, 1, 2,		0, 2, 3,
	// back
	4, 5, 6,		4, 6, 7,
	// right
	8, 9, 10,		8, 10, 11,
	// left
	12, 13, 14,		12, 14, 15,
	// top
	16, 17, 18,		16, 18, 19,
	//bottom
	20, 21, 22,		20, 22, 23,
};

const char* skyboxImages[] = {
	"Res/Skybox/alps_ft.tga",
	"Res/Skybox/alps_bk.tga",
	"Res/Skybox/alps_up.tga", 
	"Res/Skybox/alps_dn.tga",  
	"Res/Skybox/alps_rt.tga", 
	"Res/Skybox/alps_lf.tga",  
};


bool IsKeyPressed(int key) {
	return keyState[key];
}

void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (action == GLFW_REPEAT)
		return;

	keyState[key] = (action == GLFW_PRESS);

	if (action == GLFW_PRESS)
	{
		printf("OnKeyEvent ( %d )\n", key);

		if (key == GLFW_KEY_ESCAPE)
			shouldQuit = true;
	}
}

void OnCursorMove(GLFWwindow* window, double x, double y) {
	mouseX = x;
	mouseY = y;
}

void OnMouseButton(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		cameraMovementActive = (action == GLFW_PRESS);
		glfwSetInputMode(window, GLFW_CURSOR, cameraMovementActive ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}
}

void RenderTriangle(const Transform& transform) {
	triangleMesh.Bind();
	defaultMaterial.Set("u_World", transform.GetMatrix());
	defaultMaterial.Use();
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void RenderQuad(const Transform& transform) {
	quadMesh.Bind();
	defaultMaterial.Set("u_World", transform.GetMatrix());
	defaultMaterial.Use();
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void RenderCube(const Transform& transform) {
	cubeMesh.Bind();
	defaultMaterial.Set("u_World", transform.GetMatrix());
	defaultMaterial.Use();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void RenderSkybox() {
	skyboxTexture.Bind();
	cubeMesh.Bind();
	skyboxMaterial.Use();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void RenderFullScreenQuad(){
	glBindVertexArray(0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void RenderLightSource(const PointLight& light){
	lightSourceMaterial.Set("u_World", Transform(light.position, quat_identity, glm::vec3(0.2f)).GetMatrix());
	lightSourceMaterial.Set("u_Color", light.color);
	cubeMesh.Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}


void RenderScene(bool renderPlayer) {

	crate.Bind();

	float time = glfwGetTime();
	

	RenderCube(Transform(glm::vec3(2.f, -2.f, 2.f), quat_identity, glm::vec3(50.f, 0.4f, 50.f)));
	// child and babies
	{
		Transform baseTrasform;
		baseTrasform.position = glm::vec3((cos(time) * 5.f), 2.f, (sin(time)* 5.f));
		baseTrasform.rotation = normalize(glm::angleAxis(time, glm::vec3(1.f, 1.f, 0.f)));
		baseTrasform.scale = glm::vec3(2.f, 2.f, 2.f);
		Transform childTransform;
		childTransform.position = glm::vec3(2.f, sin(time), 0.f);
		childTransform.scale = glm::vec3(0.5f);
		childTransform.rotation = glm::angleAxis(time, glm::vec3(1.f, 0.f, 0.f));
		Transform babyTransform;
		babyTransform.position = glm::vec3(0.f, sin(time) * 5.f, 0.f);
		babyTransform.scale = glm::vec3(0.5f);
		Transform triangleTransform;
		triangleTransform.position = glm::vec3(0.f, 0.f, 2.f);
		triangleTransform.scale = glm::vec3(2.f);

	}
	 // Saw
	{
		Transform BaseTransform;
		BaseTransform.position = glm::vec3(5.f, 2.f, 1.f);
		BaseTransform.rotation = glm::angleAxis(time, glm::normalize(glm::vec3(0.5f, -0.4f, 1.f)));

		RenderCube(BaseTransform);

		int NumTriangles = 30;
		for (int i = 0; i < NumTriangles; ++i)
		{
			float AngleInc = (glm::pi<float>() * 2.f) / NumTriangles;
			float Angle = time + AngleInc * i;
			float SawAngle = time * 5.f + AngleInc * i;

			Transform ChildTransform;
			ChildTransform.position = glm::vec3(2.5f, sin(time * 0.6f + AngleInc * i * 2.f) * 1.f, 0.f);
			ChildTransform.scale = glm::vec3(0.5f);

			Transform SpinTransform;
			SpinTransform.rotation = glm::angleAxis(Angle, glm::vec3(0.f, 1.f, 0.f));

			Transform SawTransform;
			SawTransform.rotation = glm::angleAxis(SawAngle, glm::vec3(0.f, 0.f, 1.f));

			RenderCube(BaseTransform * SpinTransform * ChildTransform * SawTransform);
		}

	}
		// Snek
	{
		Transform currentTransform;
		currentTransform.position = glm::vec3(-4.f, 5.f, -2.f);
		RenderCube(currentTransform);
		int iterations = 100;
		for (int i = 0; i < iterations; ++i)
		{
			Transform delta;
			delta.position = glm::vec3(-1.5f, 0.f, 0.f);
			delta.scale = glm::vec3(0.95f);
			delta.rotation = glm::angleAxis(sin(time + i * 0.002f) * 0.5f , glm::vec3(0.f, 1.f, 0.f));

			Transform twistDelta;
			twistDelta.rotation = glm::angleAxis(sin(time + i * 0.002f) * 0.5f, glm::vec3(1.f, 0.f, 0.f));

			currentTransform = currentTransform * delta;
			RenderCube(currentTransform);

		}
	}

	{
		glm::vec3 rotationAxis = glm::vec3(cos(time * 0.62f), sin(time * 0.9f) * 2.f, cos(-time * 0.5f));
		rotationAxis = normalize(rotationAxis);
		Transform baseTransform;
		baseTransform.position = glm::vec3(3.f, 2.f, 15.f);
		baseTransform.rotation = glm::angleAxis(time, rotationAxis);
		RenderCube(baseTransform);
		
		static Transform childTransform = Transform(glm::vec3(2.5f, 0.f, 0.f), quat_identity, glm::vec3(0.7f));
		
		//childTransform.position = glm::vec3(2.5f, 0.f, 0.f);
		//childTransform.scale = glm::vec3(0.7f);
		//childTransform.rotation = glm::angleAxis(time, glm::vec3(0.f, 1.f, 0.f));

		static bool attachedToCamera = false;

		if (IsKeyPressed(GLFW_KEY_T)) {
			
			if(!attachedToCamera)
			{
				Transform temp;
				temp = baseTransform * childTransform;
				Transform cameraSpace = camera.GetTransform().Inverse() * temp;

				childTransform = camera.GetTransform().Inverse() * baseTransform * childTransform;

				attachedToCamera = true;
			}

			RenderCube(camera.GetTransform() * childTransform);
		}
		else {
			if(attachedToCamera) {
				Transform temp;
				temp = camera.GetTransform() * childTransform;
				Transform boxSpace = baseTransform.Inverse() * temp;

				childTransform = boxSpace;

				attachedToCamera = false;
			}
			RenderCube(baseTransform * childTransform);
		}

	}

	RenderCube(Transform(glm::vec3(0.f, 0.f, -15.f), quat_identity, glm::vec3(20.f, 20.f, 0.05f)));
	RenderSkybox();
	RenderLightSource(pointLight);
	RenderLightSource(pointLight_2);


	sphere.DrawSphere(sphereTransform, reflectionMaterial);
	//Render Player
	if (renderPlayer) {
		cheryl.Bind();
		RenderCube(playerTransform);
	}
}

int main() {
	// Initialize GLFW, create a window and a context.
	glfwInit();

	GLFWwindow* window;
	window = glfwCreateWindow(screenWidth, screenHeight, "This is OpenGL!", nullptr, nullptr);

	glfwSetKeyCallback(window, OnKeyEvent);
	glfwSetCursorPosCallback(window, OnCursorMove);
	glfwSetMouseButtonCallback(window, OnMouseButton);
	glfwMakeContextCurrent(window);

	// After the context is bound, we can import the OpenGL extensions, through the extension wrangler.
	glewInit();

	/* Interleaved Buffer for position and color */
				/* Quad */

	float quadData[] = {
		-0.5f, -0.5f, 0.0f,		0.f, 0.f, 1.f,		0.f, 0.f,
		 0.5f, -0.5f, 0.0f,		0.f, 0.f, 1.f,		1.f, 0.f,
		 0.5f,  0.5f, 0.0f,		0.f, 0.f, 1.f,		1.f, 1.f,
		 -0.5f, 0.5f, 0.0f,		0.f, 0.f, 1.f,		0.f, 1.f
	};
	quadMesh.LoadVerts(quadData, sizeof(quadData), nullptr, 0);

	cubeMesh.LoadVerts(cubeVertexData, sizeof(cubeVertexData), cubeIndexData, sizeof(cubeIndexData));

	/* Triangle */
	float triData[] = {
		-0.5f, -0.5f, 0.0f,		0.f, 0.f, 1.f,		0.f, 0.f,
		 0.5f, -0.5f ,0.0f,		0.f, 0.f, 1.f,		1.f, 0.f,
		 0.f, 0.5f,	  0.0f,		0.f, 0.f, 1.f,		0.5f, 1.f
	};

	triangleMesh.LoadVerts(triData, sizeof(triData), nullptr, 0);


	cheryl.LoadFile("Res/img_cheryl.jpg");
	crate.LoadFile("Res/crate.png");
	cheryl.Bind();

	reflectionMaterial.LoadFiles("Res/Shader/reflection.vert", "Res/Shader/reflection.frag");

	defaultMaterial.LoadFiles("Res/Shader/default.vert", "Res/Shader/default.frag");
	defaultMaterial.Set("u_DirectionalLight.shadowBuffer", 2);
	defaultMaterial.Use();

	// Sphere
	sphereTransform.position = glm::vec3(20.f, 5.f, 20.f);
	sphere.SetSphereSize(5.f, 36, 18);
	sphere.LoadSphere();
	

	Material postProcessMaterial;
	postProcessMaterial.LoadFiles("Res/Shader/post_process.vert", "Res/Shader/post_process.frag");
	postProcessMaterial.Set("u_FrameColor", 0);
	postProcessMaterial.Set("u_FrameDepth", 1);

	skyboxMaterial.LoadFiles("Res/Shader/Skybox.vert", "Res/Shader/Skybox.frag");
	lightSourceMaterial.LoadFiles("Res/Shader/light_source.vert", "Res/Shader/light_source.frag");

	skyboxTexture.LoadFiles(skyboxImages);

	float ratio = float(screenWidth) / (float)screenHeight;

	camera.position = glm::vec3(0.f, 5.f, 5.f);

	glm::vec3 lightDirection = glm::normalize(glm::vec3(-1.f, -0.55f, 0.35f));
	defaultMaterial.Set("u_DirectionalLight.direction", lightDirection);
	defaultMaterial.Set("u_DirectionalLight.color", glm::vec3(0.9f, 0.9f, 1.f));

	pointLight.position = glm::vec3(0.f, 1.f, 0.f);
	pointLight.color = glm::vec3(1.f, 0.7f, 0.4f);
	pointLight.radius = 10.f;
	pointLight.UploadToMaterial(0, defaultMaterial);
	
	pointLight_2.position = glm::vec3(0.f, 1.f, 0.f);
	pointLight_2.color = glm::vec3(0.f, 0.2f, 1.f);
	pointLight_2.radius = 20.f;
	pointLight_2.UploadToMaterial(1, defaultMaterial);

	spotLight.position = glm::vec3(0.f, 2.f, 10.f);
	spotLight.direction = glm::vec3(0.f, 0.f, -1.f);
	spotLight.color = glm::vec3(0.2f, 1.f, 0.2f);
	spotLight.angle = glm::radians(30.f);
	spotLight.radius = 40.f;
	spotLight.UploadToMaterial(defaultMaterial);

	glm::mat4 perspective;
	perspective = glm::perspective(glm::radians(60.f), ratio, 0.05f, 150.f);
	defaultMaterial.Set("u_Projection", perspective);

	glEnable(GL_DEPTH_TEST);
	float lastFrameTime = 0.f;

	double lastMouseX = 0.0;
	double lastMouseY = 0.0;

	FrameBuffer lightBuffer;
	lightBuffer.Create(4096, 4096);

	FrameBuffer frameBuffer;
	frameBuffer.Create(2000, 2000);

	glm::mat4 lightProjection;
	glm::mat4 lightView;
	lightProjection = glm::ortho(-30.f, 30.f, -30.f, 30.f, -30.f, 30.f);
	lightView = glm::lookAt(glm::vec3(0.f), lightDirection, glm::vec3(0.f, 1.f, 0.f));

	//Main Loop
	while (!glfwWindowShouldClose(window) && !shouldQuit) {
		defaultMaterial.Use();

		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		double mouseDeltaX = mouseX - lastMouseX;
		double mouseDeltaY = mouseY - lastMouseY;
		lastMouseX = mouseX;
		lastMouseY = mouseY;

		if (cameraMovementActive) {

			camera.AddYaw(mouseDeltaX * 0.1f);
			camera.AddPitch(mouseDeltaY * 0.1f);

			glm::vec3 cameraMovement = glm::vec3(0.f);
			if (IsKeyPressed(GLFW_KEY_W))
				cameraMovement += camera.GetForwardVector();
			if (IsKeyPressed(GLFW_KEY_S))
				cameraMovement += -camera.GetForwardVector();
			if (IsKeyPressed(GLFW_KEY_A))
				cameraMovement += -camera.GetRightVector();
			if (IsKeyPressed(GLFW_KEY_D))
				cameraMovement += camera.GetRightVector();
			if (IsKeyPressed(GLFW_KEY_E))
				cameraMovement += glm::vec3(0.f, 1.f, 0.f);
			if (IsKeyPressed(GLFW_KEY_Q))
				cameraMovement -= glm::vec3(0.f, 1.f, 0.f);

			camera.position += cameraMovement * 5.f * deltaTime;
		}
		playerTransform.position = camera.position;
		playerTransform.rotation = glm::quatLookAt(camera.GetForwardVector(), glm::vec3(0.f, 1.f, 0.f));
		reflectionMaterial.Set("cameraPosition", camera.position);

		if (IsKeyPressed(GLFW_KEY_F)) {
			spotLight.position = camera.position;
			spotLight.direction = camera.GetForwardVector();
			spotLight.UploadToMaterial(defaultMaterial);
		}

		defaultMaterial.Set("u_EyePosition", camera.position);
		defaultMaterial.Set("u_View", camera.GetViewMatrix());

		pointLight.position = glm::vec3(sin(currentTime) * 2.f, 0.2f, glm::cos(currentTime) * 2.f);
		pointLight.color = glm::vec3(glm::sin(currentTime), glm::cos(currentTime), glm::sin(-currentTime));
		pointLight.UploadToMaterial(0, defaultMaterial);

		// Clear the screen
		glClearColor(0.1f, 0.5f, 0.3f, 1.f);

		// Render the light pass

		defaultMaterial.Set("u_Projection", lightProjection);
		defaultMaterial.Set("u_View", lightView);
		
		lightSourceMaterial.Set("u_Projection", lightProjection);
		lightSourceMaterial.Set("u_View", lightView);

		lightBuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderScene(true);
		
		lightBuffer.Unbind();

		
		//Render to frame buffer
		reflectionMaterial.Set("u_Projection", perspective);
		reflectionMaterial.Set("u_View", camera.GetViewMatrix());

		defaultMaterial.Set("u_Projection", perspective);
		defaultMaterial.Set("u_View", camera.GetViewMatrix());
		defaultMaterial.Set("u_DirectionalLight.viewProjection", lightProjection * lightView);

		lightSourceMaterial.Set("u_Projection", perspective);
		lightSourceMaterial.Set("u_View", camera.GetViewMatrix());

		skyboxMaterial.Set("u_Projection", perspective);
		skyboxMaterial.Set("u_View", camera.GetViewMatrix());


		lightBuffer.depthTexture.Bind(2);

		frameBuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderScene(false);
		frameBuffer.Unbind();
		
		//Render to screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		defaultMaterial.Set("u_View", camera.GetViewMatrix());
		postProcessMaterial.Use();
		frameBuffer.colorTexture.Bind();
		frameBuffer.depthTexture.Bind(1);
		RenderFullScreenQuad();

		//Swap the back-buffer to the front and poll and handle window events.
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();

	////
	return 0;
}
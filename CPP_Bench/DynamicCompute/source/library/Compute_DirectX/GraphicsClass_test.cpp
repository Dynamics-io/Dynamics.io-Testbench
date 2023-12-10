#include "GraphicsClass_test.h"

using namespace DynamicCompute::Compute::DX;


DynamicCompute::Compute::DX::GraphicsClass::GraphicsClass()
{
	m_Direct3D = nullptr;
}

GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	m_Direct3D = new D3DClass;

	result = m_Direct3D->Initialize(
		screenWidth,
		screenHeight,
		VSYNC_ENABLED,
		hwnd,
		FULL_SCREEN,
		SCREEN_DEPTH,
		SCREEN_NEAR
	);

	if (!result) {
		throw std::exception("Could not initialize Direct3D!\n");
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the Direct3D object.
	if (m_Direct3D) {
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = nullptr;;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;

	// Render the graphics scene.
	result = Render();
	if (!result) {
		return false;
	}

	return true;
}


bool GraphicsClass::Render()
{
	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);


	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}


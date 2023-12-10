#pragma once

#include "stdafx.h"

#include "InputClass_test.h"
#include "GraphicsClass_test.h"

namespace DynamicCompute {
	namespace Compute {
		namespace DX {

			class SystemClass_test
			{
			public:
				SystemClass_test();
				SystemClass_test(const SystemClass_test&){}
				~SystemClass_test(){}

				bool Initialize();
				void Shutdown();
				void Run();

				LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

			private:
				bool Frame();
				void InitializeWindows(int&, int&);
				void ShutdownWindows();

			private:
				LPCSTR m_applicationName;
				HINSTANCE m_hinstance;
				HWND m_hwnd;

				InputClass* m_Input;
				GraphicsClass* m_Graphics;

				int screenWidth = 0;
				int screenHeight = 0;
			};

			static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

			static SystemClass_test* ApplicationHandle = 0;

		}
	}
}


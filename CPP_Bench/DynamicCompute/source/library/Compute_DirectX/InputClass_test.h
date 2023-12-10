#pragma once

#include "stdafx.h"

namespace DynamicCompute {
	namespace Compute {
		namespace DX {

			class InputClass
			{
			public:
				InputClass() {}
				InputClass(const InputClass&) {}
				~InputClass();

				void Initialize();

				void KeyDown(unsigned int);
				void KeyUp(unsigned int);

				bool IsKeyDown(unsigned int);

			private:
				bool m_keys[256];
			};

		}
	}
}



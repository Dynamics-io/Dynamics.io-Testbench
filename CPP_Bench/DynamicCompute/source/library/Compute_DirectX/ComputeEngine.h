#pragma once

#include "stdafx.h"

#include "directX_utils.h"
#include "Compute_Interface/PlatformStructures_private.h"

#define DEFAULT_WORK_GROUP_SIZE 16
#define DEFAULT_FILE_EXTENSION "fxc"

namespace DynamicCompute {
    namespace Compute {
        namespace DX {

            class ComputeContext;
            class ComputeEngine;
            class ComputeKernel;
            class ComputeBuffer;
            class ComputeProgram;


            class ComputeBuffer {
                friend class ComputeContext;
            public:
                enum class Buffer_Type
                {
                    READ = 0,
                    Write = 1,
                    Read_Write = 2
                };

                ComputeContext* mContext{ nullptr };
                Buffer_Type mType{};
                UINT mNumElements;
                UINT mStride;

                ID3D11Device5* m_device{ nullptr };
                ID3D11DeviceContext4* m_deviceContext{ nullptr };

                ID3D11Buffer* mBuffer{ nullptr };
                ID3D11Buffer* mStagingBuffer{ nullptr };

                bool mDestroyed{ false };
                bool mCanCallDispose{ false };

            public:
                ComputeBuffer() : mCanCallDispose(false) {}
                ~ComputeBuffer();

                int SetData(void* data);

                int GetData(void* outData);

                size_t GetFullSize() {
                    return mNumElements * mStride;
                }

                Buffer_Type GetType(){
                    return mType;
                }

                ID3D11Buffer* Get_DX_Mem() {
                    return mBuffer;
                }

                void Dispose();

            private:
                ComputeBuffer(ComputeContext* context, Buffer_Type type, UINT numElements, UINT stride);
            };


            class ComputeKernel {
                friend class ComputeProgram;

                std::string mName{ "" };
                std::string mFilePath{ "" };
                ComputeProgram* mProgram{ nullptr };

                ID3D11Device5* m_device{ nullptr };
                ID3D11DeviceContext4* m_deviceContext{ nullptr };

                ID3D11ComputeShader* m_computeShader{ nullptr };

                uint32_t mWorkGroupSize{ DEFAULT_WORK_GROUP_SIZE };
                std::string mFileExtension{ DEFAULT_FILE_EXTENSION };

                std::vector<ID3D11ShaderResourceView*> mResourceViews;
                std::vector<ID3D11UnorderedAccessView*> mAccessViews;


                bool mInitialized{ false };
                bool mCanCallDispose{ false };
                bool mDestroyed{ false };

            public:
                ComputeKernel() : mCanCallDispose(false) {}
                ~ComputeKernel();

                int SetBuffer(ComputeBuffer* buffer, int arg);

                HRESULT BuildKernel();

                int Execute(uint32_t x, uint32_t y, uint32_t z);

                void Dispose();

            private:
                ComputeKernel(std::string name, ComputeProgram* program);

                HRESULT addResourceView(ComputeBuffer* buffer, int arg);

                HRESULT addUnorderedAccessView(ComputeBuffer* buffer, int arg);
            };


            class ComputeProgram {
                friend class ComputeContext;

                std::string mName{ "" };
                ComputeContext* mContext{ nullptr };

                std::string mDirectory{ "" };

                std::map<std::string, ComputeKernel> kernels;

                bool mInitialized{ false };
                bool mCanCallDispose{ false };
                bool mDestroyed{ false };

            public:
                ComputeProgram() : mCanCallDispose(false) {}
                ~ComputeProgram();

                std::string GetName() {
                    return mName;
                }

                std::string GetDirectory() {
                    return mDirectory;
                }

                ComputeContext* GetContext() {
                    return mContext;
                }

                // Not used as source is obtained at the kernel level.
                int Set_Source(const char* source);

                // Not used as source is obtained at the kernel level.
                int Set_Binary(const void* binary, size_t length);

                // Not used as source is obtained at the kernel level.
                int Set_Binary_File(std::string file_path);

                ComputeKernel* GetKernel(std::string name);

                int Buildkernels();

                void Dispose();


            private:
                ComputeProgram(std::string name, ComputeContext* context);
            };


            class ComputeContext
            {
                friend class ComputeEngine;

                ID3D11Device5* m_device{ nullptr };
                ID3D11DeviceContext4* m_deviceContext{ nullptr };

                std::map<std::string, ComputeProgram> programs;
                std::list<ComputeBuffer> mBuffers;

                bool mDestroyed{ false };
                bool mCanCallDispose{ false };

            public:
                ComputeContext() : mCanCallDispose(false) {}
                ~ComputeContext();

                ID3D11Device5* GetDevice() {
                    return m_device;
                }

                ID3D11DeviceContext4* GetDeviceContext() {
                    return m_deviceContext;
                }

                ComputeProgram* Add_Program(std::string name);
                ComputeProgram* Add_Program_Source(std::string name, const char* source);
                ComputeProgram* Add_Program_Binary(std::string name, const void* binary, size_t length);
                ComputeProgram* Add_Program_Binary_File(std::string name, std::string file_path);
                ComputeProgram* Programs(std::string name);

                ComputeKernel* GetKernel(std::string p_name, std::string name);
                ComputeBuffer* CreateBuffer(ComputeBuffer::Buffer_Type type, UINT numElements, UINT stride);

                void Dispose();

            private:
                ComputeContext(DirectX_Device_Info device);
            };


            class ComputeEngine
            {
                static bool mInitialized;

                static std::string mApp_dir;

                static std::list<ComputeContext> mContexts;

            public:
                static int Init(std::string dir);

                static ComputeContext* GetNewContext(DirectX_Device_Info device);

                static std::string GetAppDir()
                {
                    return mApp_dir;
                }

                static bool IsInitialized() {
                    return mInitialized;
                }

                static void Dispose();

            private:

            };

        }
    }
}
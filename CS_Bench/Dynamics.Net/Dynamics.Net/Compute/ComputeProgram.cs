using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Metadata;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Dynamics.Compute
{
    public class ComputeProgram : NativeObject
    {
        [DllImport(Settings.DLL)]
        private static extern void IComputeProgram_Init(IntPtr handle, string name, int size);

        [DllImport(Settings.DLL)]
        private static extern int IComputeProgram_FinishBuild(IntPtr handle);

        [DllImport(Settings.DLL)]
        private static extern int IComputeProgram_GetKernelID(IntPtr handle, string name, int size);

        [DllImport(Settings.DLL)]
        private static extern int IComputeProgram_KernelSetBuffer(IntPtr handle, string name, int size, IntPtr/*ComputeBuffer*/ buffer, BindIndex arg);

        [DllImport(Settings.DLL)]
        private static extern int IComputeProgram_RunKernel_1(IntPtr handle, string name, int size, int size_x, int size_y, int size_z);

        [DllImport(Settings.DLL)]
        private static extern int IComputeProgram_RunKernel_2(IntPtr handle, int kernel_id, int size_x, int size_y, int size_z);

        //private static extern  IntPtr IComputeProgram_GetKernelFunction(IntPtr handle, int kernel_id);

        [DllImport(Settings.DLL)]
        private static extern ProgramBuildState IComputeProgram_GetState(IntPtr handle);

        [DllImport(Settings.DLL)]
        private static extern int IComputeProgram_GetBuildResultCode(IntPtr handle);

        // private static extern  std::string GetBuildErrorMessage() = 0;

        // private static extern  std::string GetProgramName() = 0;

        [DllImport(Settings.DLL)]
        private static extern void IComputeProgram_Dispose(IntPtr handle);


        public enum ProgramBuildState
        {
            None,
			Inited,
			Constructed,
			Built,
			BuildError
        };

        public enum FileType
        {
            Text,
			Binary
        };

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public struct BindIndex
        {
            // Used in kernels where buffers are bound to function parameters
            public int ParameterIndex;

            // Used in kernels where buffers are bound in the global scope
            public int GlobalIndex;

            // Used in DirectX kernels, where read/write buffers have seperate index counters.
            public int RegisterIndex;
        };

        public class ProgramInfo : NativeObject
        {
            [DllImport(Settings.DLL)]
            private static extern IntPtr IComputeProgram_ProgramInfo_New(char[] program_name, int size, ComputeProgram.FileType type);

            [DllImport(Settings.DLL)]
            private static extern void IComputeProgram_ProgramInfo_AddKernel(IntPtr handle, char[] name, int size);

            public ProgramInfo(nint hdl) : base(hdl)
            {
            }

            public ProgramInfo(string program_name, FileType type) : base()
            {
                SetHandle(IComputeProgram_ProgramInfo_New(program_name.ToCharArray(), program_name.Length, type));
            }

            public void AddKernel(string name)
            {
                IComputeProgram_ProgramInfo_AddKernel(handle, name.ToCharArray(), name.Length);
            }
        }
        internal ComputeProgram(IntPtr hdl) :
            base(hdl)
        {
        }

        public void Init(string name)
        {
            IComputeProgram_Init(handle, name, name.Length);
        }

        public int FinishBuild()
        {
            return IComputeProgram_FinishBuild(handle);
        }

        public int GetKernelID(string name)
        {
            return IComputeProgram_GetKernelID(handle, name, name.Length);
        }

        public int KernelSetBuffer(string name, ComputeBuffer buffer, BindIndex arg)
        {
            return IComputeProgram_KernelSetBuffer(handle, name, name.Length, buffer, arg);
        }

        public int RunKernel(string name, int size_x, int size_y, int size_z)
        {
            return IComputeProgram_RunKernel_1(handle, name, name.Length, size_x, size_y, size_z);
        }

        public int RunKernel(int kernel_id, int size, int size_x, int size_y, int size_z)
        {
            return IComputeProgram_RunKernel_2(handle, kernel_id, size_x, size_y, size_z);
        }

        public ProgramBuildState GetState()
        {
            return IComputeProgram_GetState(handle);
        }

        public int GetBuildResultCode()
        {
            return IComputeProgram_GetBuildResultCode(handle);
        }

        public void Dispose()
        {
            IComputeProgram_Dispose(handle);
        }
    }
}

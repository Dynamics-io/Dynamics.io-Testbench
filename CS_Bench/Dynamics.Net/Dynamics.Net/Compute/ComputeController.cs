using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Dynamics.Compute
{
    public class ComputeController : NativeObject
    {
        [DllImport(Settings.DLL)]
        private static extern IntPtr IComputeController_AddProgram_ref(IntPtr handle, IntPtr info_handle);

        [DllImport(Settings.DLL)]
        private static extern IntPtr IComputeController_GetProgram(IntPtr handle, string name, int size);

        [DllImport(Settings.DLL)]
        private static extern IntPtr IComputeController_NewReadBuffer(IntPtr handle, int numElements, int stride);

        [DllImport(Settings.DLL)]
        private static extern IntPtr IComputeController_NewWriteBuffer(IntPtr handle, int numElements, int stride);

        [DllImport(Settings.DLL)]
        private static extern IntPtr IComputeController_NewReadWriteBuffer(IntPtr handle, int numElements, int stride);

        [DllImport(Settings.DLL)]
        private static extern void IComputeController_Dispose(IntPtr handle);


        internal ComputeController(IntPtr hdl) 
            : base(hdl)
        {
        }

        public ComputeProgram AddProgram(ComputeProgram.ProgramInfo info)
        {
            // Native function disposes ProgramInfo object. ProgramInfo is invalid after this call. 
            return new ComputeProgram(IComputeController_AddProgram_ref(handle, info));
        }

        public ComputeProgram IComputeController_GetProgram(string name)
        {
            return new ComputeProgram(IComputeController_GetProgram(handle, name, name.Length));
        }

        public ComputeBuffer NewReadBuffer(int numElements, int stride)
        {
            return new ComputeBuffer(IComputeController_NewReadBuffer(handle, numElements, stride));
        }

        public ComputeBuffer NewWriteBuffer(int numElements, int stride)
        {
            return new ComputeBuffer(IComputeController_NewWriteBuffer(handle, numElements, stride));
        }

        public ComputeBuffer NewReadWriteBuffer(int numElements, int stride)
        {
            return new ComputeBuffer(IComputeController_NewReadWriteBuffer(handle, numElements, stride));
        }

        public void Dispose()
        {
            IComputeController_Dispose(handle);
        }
    }
}

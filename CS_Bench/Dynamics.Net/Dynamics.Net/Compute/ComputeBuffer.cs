using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using Dynamics;

namespace Dynamics.Compute
{
    public class ComputeBuffer : NativeObject
    {
        [DllImport(Settings.DLL)]
        private static extern int IComputeBuffer_SetData(IntPtr handle, IntPtr data);

        [DllImport(Settings.DLL)]
        private static extern int IComputeBuffer_GetData(IntPtr handle, IntPtr outData);

        [DllImport(Settings.DLL)]
        private static extern int IComputeBuffer_GetSize(IntPtr handle);

        [DllImport(Settings.DLL)]
        private static extern void IComputeBuffer_Dispose(IntPtr handle);

        [DllImport(Settings.DLL)]
        private static extern IntPtr IComputeBuffer_Get_Native_Ptr(IntPtr handle);

        internal ComputeBuffer(IntPtr hdl) :
            base(hdl)
        {
        }

        public int SetData(object data)
        {
            GCHandle pinnedArray = GCHandle.Alloc(data, GCHandleType.Pinned);
            IntPtr pointer = pinnedArray.AddrOfPinnedObject();

            int res = IComputeBuffer_SetData(handle, pointer);

            pinnedArray.Free();

            return res;
        }

        public int GetData(object data)
        {
            GCHandle pinnedArray = GCHandle.Alloc(data, GCHandleType.Pinned);
            IntPtr pointer = pinnedArray.AddrOfPinnedObject();

            int res = IComputeBuffer_GetData(handle, pointer);

            pinnedArray.Free();

            return res;
        }

        public void Dispose()
        {
            IComputeBuffer_Dispose(handle);
        }

        public IntPtr Get_CL_Mem()
        {
            return IComputeBuffer_Get_Native_Ptr(handle);
        }

        public ulong GetSize()
        {
            return (ulong)IComputeBuffer_GetSize(handle);
        }

    }
}

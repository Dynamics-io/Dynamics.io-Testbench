using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Dynamics
{
    public class NativeObject : IEquatable<NativeObject>
    {
        protected IntPtr handle;

        private static Dictionary<IntPtr, NativeObject> cache = new Dictionary<IntPtr, NativeObject>();

        public NativeObject()
        {

        }

        public NativeObject(IntPtr hdl)
        {
            if (hdl == IntPtr.Zero)
            {
                throw new Exception("Native Object must be created with valid handle.");
            }

            handle = hdl;
        }

        protected void SetHandle(IntPtr hdl)
        {
            handle = hdl;
        }

        public IntPtr GetHandle()
        {
            return handle;
        }

        public bool Equals(NativeObject other)
        {
            if (other == null)
                return false;
            return handle.Equals(other.handle);
        }

        public static implicit operator IntPtr(NativeObject orig)
        {
            return orig.GetHandle();
        }

        public static T GetObject<T>(IntPtr handle, bool do_cache = true) where T : NativeObject
        {
            return (T)Activator.CreateInstance(typeof(T), handle);


            /*if (!do_cache)
                return (T)new NativeObject(handle);

            if (cache.ContainsKey(handle))
            {
                return (T)cache[handle];
            }

            NativeObject newActor = new NativeObject(handle);

            cache.Add(handle, newActor);

            return (T)newActor;*/
        }
    }
}

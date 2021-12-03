using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

public static class LeakLib {
    public const string LibName="mem-leak-lib";

    [DllImport(LeakLib.LibName)]
    private static extern int memleak_create_one_object (out IntPtr p);

    public static IntPtr Create() {
	IntPtr ptr;
	int hr = memleak_create_one_object (out ptr);
	if (hr != 0) {
	    throw new Exception ($"returned 0x{hr:x}");
	}
	return ptr;
    }
}

namespace FromCom
{
    
    [ComImport]
    [Guid ("78338C12-1AB5-435D-BCDA-A17417496F4A")]
    [InterfaceType (ComInterfaceType.InterfaceIsIUnknown)]
    public interface IOne {
	void CommandOne ();
    }

    [ComImport]
    [Guid ("58228253-A9B1-4F6C-A4D2-0F997CEE74FD")]
    [ClassInterface (ClassInterfaceType.None)]
    public class _One : IOne {
	[MethodImpl (MethodImplOptions.InternalCall, MethodCodeType = MethodCodeType.Runtime)]
	public virtual extern void CommandOne ();
    }

    [Guid ("58228253-A9B1-4F6C-A4D2-0F997CEE74FD")]
    public class One : _One {
	static One () {
	    ExtensibleClassFactory.RegisterObjectCreationCallback (static (IntPtr aggr) => LeakLib.Create());
	}
    }
}

public class Program
{
    public static void Main()
    {
	Console.WriteLine ("Hello");
	HideFromGC (static () => {
	    var x = new FromCom.One();
	    Console.WriteLine ("Created an object");
	    x.CommandOne ();
	    Console.WriteLine ("Called CommandOne");
	    x = null;

	});
	Collect();
	Console.WriteLine ("Exiting");
    }

    private static void Collect () {
	GC.Collect ();
	GC.WaitForPendingFinalizers();
	GC.Collect ();
	GC.WaitForPendingFinalizers();
    }

    [MethodImpl (MethodImplOptions.NoInlining)]
    private static void HideFromGC (Action f) {
	HideFromGC (20, f);
    }

    [MethodImpl (MethodImplOptions.NoInlining)]
    private static void HideFromGC (int rec, Action f) {
	if (rec <= 0)
	    f ();
	else
	    HideFromGC (rec - 1, f);
    }

}

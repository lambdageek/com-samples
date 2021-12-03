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
	[MethodImpl (MethodImplOptions.InternalCall, MethodCodeType = MethodCodeType.Runtime)]
	void CommandOne ();
	[MethodImpl (MethodImplOptions.InternalCall, MethodCodeType = MethodCodeType.Runtime)]
	void CommandTwo ([In] [MarshalAs(UnmanagedType.Interface)] ToCom.IBoxXYZ box);
	[MethodImpl (MethodImplOptions.InternalCall, MethodCodeType = MethodCodeType.Runtime)]
	void Add ([In] [MarshalAs(UnmanagedType.Interface)] ToCom.IBoxXYZ box);
    }

    [ComImport]
    [Guid ("58228253-A9B1-4F6C-A4D2-0F997CEE74FD")]
    [ClassInterface (ClassInterfaceType.None)]
    public class _One : IOne {
	[MethodImpl (MethodImplOptions.InternalCall, MethodCodeType = MethodCodeType.Runtime)]
	public virtual extern void CommandOne ();

	[MethodImpl (MethodImplOptions.InternalCall, MethodCodeType = MethodCodeType.Runtime)]
	public virtual extern void CommandTwo ([In] [MarshalAs(UnmanagedType.Interface)] ToCom.IBoxXYZ box);

	[MethodImpl (MethodImplOptions.InternalCall, MethodCodeType = MethodCodeType.Runtime)]
	public virtual extern void Add ([In] [MarshalAs(UnmanagedType.Interface)] ToCom.IBoxXYZ box);
    }

    [Guid ("58228253-A9B1-4F6C-A4D2-0F997CEE74FD")]
    public class One : _One {
	static One () {
	    ExtensibleClassFactory.RegisterObjectCreationCallback (static (IntPtr aggr) => LeakLib.Create());
	}
    }
}

namespace ToCom {
    [ComImport]
    [Guid ("E6E1F38F-9700-473A-980C-AA41B6FDE3E8")]
    [InterfaceType (ComInterfaceType.InterfaceIsIUnknown)]
    public interface IBoxXYZ {
	[MethodImpl (MethodImplOptions.InternalCall, MethodCodeType=MethodCodeType.Runtime)]
	public int ThankYou ();
    }

    public class BoxXYZ : IBoxXYZ, IDisposable {
	int count;
	public BoxXYZ() { }
	public virtual int ThankYou () {
	    ++count;
	    Console.WriteLine ($"ThankYou called {count} times");
	    return count;
	}

	~BoxXYZ() {
	    Console.WriteLine ($"Finalizing BoxXYZ, count is {count}");
	}

	public void Dispose () {
	    Console.WriteLine ("Disposing BoxXYZ");
	    GC.SuppressFinalize (this);
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
	    x.CommandTwo (new ToCom.BoxXYZ());
	    Console.WriteLine ("Called CommandTwo");
	    for (int i = 0; i < 4; ++i)
		x.Add (new ToCom.BoxXYZ());
	    Console.WriteLine ("Added 4 Boxes to OneObject");
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

# RCW and CCW

Example of doing COM interop using Mono 6.12

## Build

The build assumes you're on an Intel Mac and using Xcode clang.  Modify the Makefile for other platforms. 

```console
$ msbuild rcw-mem-leak.csproj /t:Restore,Build
...
Build succeeded.
    0 Warning(s)
    0 Error(s)
```

Run 

```console
$ mono out/bin/netframework472/rcw-mem-leak.ex
Hello
...
Exiting
```


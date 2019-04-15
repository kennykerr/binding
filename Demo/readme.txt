Demo 1 script:

start VS 2019
pre-install C++/WinRT VSIX ("winrt"), but show in Extensions
create C++ desktop Windows app, target 18362 (19h1, for xaml island support)
OR, create C++/WinRT console app and retarget to windows subsystem 
build
Add xamlisland.h, desktopwindow.h, simple TextBlock
Talk about simple direct HWND win32 under hood
build/run
add sxs fusion manifest for xaml islands :
    <compatibility xmlns="urn:schemas-microsoft-com:compatibility.v1">
    <application>
    <maxversiontested Id="10.0.18362.0"/>
    </application>
    </compatibility>
build/run
add:
struct MyControl : UserControlT<MyControl, IInspectable>
{
    MyControl()
    {
        TextBlock tb;
        tb.Text(L"Hello Desktop XAML, from local control");
        tb.FontSize(30.0f);
        Content(tb);
    }
};
island.Content(make<MyControl>());
build/run

create runtime component dll
Add blankusercontrol item template
build all
add winmd reference to app
add:
    RuntimeComponent1::BlankUserControl c;
    island.Content(c);
add to sxs fusion manifest for reg-free winrt:
    <file name="RuntimeComponent1.dll">
    <activatableClass
    name="RuntimeComponent1.BlankUserControl"
    threadingModel="both"
    xmlns="urn:schemas-microsoft-com:winrt.v1" />
    </file>
add vcrtforwarder package (currently internal source, nuget.org by //build) ("vcrtforwarders", check prerelease)
build/run

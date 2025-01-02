# zygisk-unity-mod-menu
实现在模拟器上hook注入显示菜单

项目是基于Perfare/Zygisk-Il2CppDumper https://github.com/Perfare/Zygisk-Il2CppDumper

使用的模板是 zygisk-mod-menu：https://github.com/fedes1to/Zygisk-ImGui-Menu

通过Perfare佬的思路，在模拟器上使用x86_64的so加载arm64的so，达到注入的效果

但是此时加载的arm64的so无法hook input，导致无法触摸按钮

采取的解决方法是在x86_64的so中去hook eglSwapBuffers和input

通过读写"/data/data/" + std::string(GamePackageName) + "/files/sharedata.bin"，实现菜单的数据传递


# 如何使用
该项目的案例是FGO

首先修改GamePackageName 为你的包名

Hook一个在游戏中经常调用的函数，例如例子中的Updata

在其中调用readSharedDataFromFile(path.c_str(),&readdata);就可以了

没有去测试真机，真机也不用这么复杂，直接使用上面的模板就行了


# INSTALLATION

## Installation and Configuration environment 
> the programming and Integration environment have been configured on a Windows 10 system environment.
> It is based on Visual Studio Code and Cmake , Gnu GCC compiler.

> **IMPORTANT NOTE : THE TERMINAL TO BE USED IS "MSYS 2 MINGW 64" and no OTHER ONE**
> 
> **IF You HAVE SOME TROUBLES USING GCC TOOLCHAIN, YOU CAN UPDATE ALL PACKAGES USING `pacman -Suy` COMMAND**

### Using GCC with MinGW

In this tutorial, you configure Visual Studio Code to use the GCC C++ compiler (g++) and GDB debugger from [mingw-w64](http://mingw-w64.org) to create programs that run on Windows.

After configuring VS Code, you will compile and debug a simple Hello World program in VS Code. This tutorial does not teach you about GCC, GDB, Mingw-w64, or the C++ language. For those subjects, there are many good resources available on the Web.

If you have any problems, feel free to file an issue for this tutorial in the [VS Code documentation repository](https://github.com/microsoft/vscode-docs/issues).

### Prerequisites 

To successfully complete this tutorial, you must do the following steps:

1. Install [graphviz](https://gitlab.com/api/v4/projects/4207231/packages/generic/graphviz-releases/8.0.2/windows_10_cmake_Release_graphviz-install-8.0.2-win64.exe) and put `C:\Program Files\Graphviz\bin` in path variable environment under windows 

1. Install [Doxygen](https://www.doxygen.nl/files/doxygen-1.9.6-setup.exe) for Windows (or other distribution)

1. Install [Visual Studio Code](/download).

1. Install the [C/C++ extension for VS Code](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools). You can install the C/C++ extension by searching for 'c++' in the Extensions view (`kb(workbench.view.extensions)`).

    ![C/C++ extension](images/cpp/cpp-extension.png)

1. Get the latest version of Mingw-w64 via [MSYS2](https://www.msys2.org/), which provides up-to-date native builds of GCC, Mingw-w64, and other helpful C++ tools and libraries. You can download the latest installer from the MSYS2 page or use this [link to the installer](https://github.com/msys2/msys2-installer/releases/download/2022-06-03/msys2-x86_64-20220603.exe).

2. Follow the **Installation** instructions on the [MSYS2 website](https://www.msys2.org/) to install Mingw-w64. Take care to run each required Start menu and `pacman` command.

3. Install the Mingw-w64 toolchain (`pacman -S --needed base-devel mingw-w64-x86_64-toolchain`). Run the `pacman` command in a MSYS2 terminal. Accept the default to install all the members in the `toolchain` group.

4.Install Gcc (`pacman -S mingw-w64-ucrt-x86_64-gcc`), Gdb (`pacman -S mingw-w64-ucrt-x86_64-gdb`)

5. Add the path to your Mingw-w64 `bin` folder to the Windows `PATH` environment variable by using the following steps:
   1. In the Windows search bar, type 'settings' to open your Windows Settings.
   2. Search for **Edit environment variables for your account**.
   3. Choose the `Path` variable in your **User variables** and then select **Edit**.
   4. Select **New** and add the Mingw-w64 destination folder path to the system path. The exact path depends on which version of Mingw-w64 you have installed and where you installed it. If you used the settings above to install Mingw-w64, then add this to the path: `C:\msys64\mingw64\bin`. 
   5. Select **OK** to save the updated PATH. You will need to reopen any console windows for the new PATH location to be available.

6. Install `pacman -S cmake`

7. Install the Mingw-w64 Lcov (`pacman -S lcov`). Run the `pacman` command in a MSYS2 terminal. Selet "all". cd into the lcov package folder, run makepkg-mingw -sCLf
   
8. Install Package on Vscode the [VSCode LCOV] (https://marketplace.visualstudio.com/items?itemName=alexdima.vscode-lcov)


### Using CMake

1. Install Cmake https://github.com/Kitware/CMake/releases/download/v3.26.0/cmake-3.26.0-windows-x86_64.msi

1. Add the path to your Cmake `bin` folder to the Windows `PATH` environment variable by using the following steps:
   1. In the Windows search bar, type 'settings' to open your Windows Settings.
   1. Search for **Edit environment variables for your account**.
   1. Choose the `Path` variable in your **User variables** and then select **Edit**.
   1. Select **New** and add the CMake destination folder path to the system path. The exact path depends on which version of Cmake you have installed and where you installed it. If you used the settings above to install Cmake, then add this to the path: `C:\Program Files\CMake\bin;`. 
   1. Select **OK** to save the updated PATH. You will need to reopen any console windows for the new PATH location to be available.

### Check your MinGW installation

To check that your Mingw-w64 tools are correctly installed and available, open a **new** Command Prompt and type:

```bash
gcc --version
g++ --version
gdb --version
make --version (Warning, perhaps, you will need to replace it using : mingw32-make.exe )  
``` 

### CMake Usage
```bash
cmake .. -G "MinGW Makefiles"
``` 

## Doxygen

If needed, you can compile Doxygen Source code 
In a specific folder, using <span style="color:green">*MSYS 2 MINGW 64*</span> shell command :
```bash
git clone https://github.com/doxygen/doxygen.git
cd doxygen && mkdir build && cd build
cmake -G "Unix Makefiles" ..
make
cp bin/doxygen.exe /c/msys64/mingw64
``` 

Install Graphviz if needed
```bash
pacman -S mingw64/mingw-w64-x86_64-graphviz,
``` 

## Collaborate with your team

- [ ] [Invite team members and collaborators](https://docs.gitlab.com/ee/user/project/members/)
- [ ] [Create a new merge request](https://docs.gitlab.com/ee/user/project/merge_requests/creating_merge_requests.html)
- [ ] [Automatically close issues from merge requests](https://docs.gitlab.com/ee/user/project/issues/managing_issues.html#closing-issues-automatically)
- [ ] [Enable merge request approvals](https://docs.gitlab.com/ee/user/project/merge_requests/approvals/)
- [ ] [Automatically merge when pipeline succeeds](https://docs.gitlab.com/ee/user/project/merge_requests/merge_when_pipeline_succeeds.html)

## Test and Deploy

Use the built-in continuous integration in GitLab.

- [ ] [Get started with GitLab CI/CD](https://docs.gitlab.com/ee/ci/quick_start/index.html)
- [ ] [Analyze your code for known vulnerabilities with Static Application Security Testing(SAST)](https://docs.gitlab.com/ee/user/application_security/sast/)
- [ ] [Deploy to Kubernetes, Amazon EC2, or Amazon ECS using Auto Deploy](https://docs.gitlab.com/ee/topics/autodevops/requirements.html)
- [ ] [Use pull-based deployments for improved Kubernetes management](https://docs.gitlab.com/ee/user/clusters/agent/)
- [ ] [Set up protected environments](https://docs.gitlab.com/ee/ci/environments/protected_environments.html)

***  

## License
(c) Plastic Omnium 2023 : " Any copy and re-use of this project without the written consent of Plastic Omnium is forbidden " 

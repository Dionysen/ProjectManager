#include "projectmanager.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <ratio>
#include <string>

ProjectManager::ProjectManager()
    : currentProject("untitled"), projectPath(getenv("HOME")),
      configFile(getenv("HOME")) {
    projectPath += "/Documents/cpp"; // 默认项目文件夹的路径，可以修改
    if (!std::filesystem::exists(projectPath))
        std::filesystem::create_directory(projectPath);
    configFile += "/.dionysen_conf";
    readConfig(); // 从配置文件读取projectPath和currentProject，如果配置文件不存在则创建配置文件，写入构造函数初始化的两个值
    if (!std::filesystem::exists(projectPath + "/.dion_trash"))
        std::filesystem::create_directory(projectPath + "/.dion_trash");
}

bool ProjectManager::
    checkProject() { // 从CMakeLists检查项目文件是否完整，完整返回true，否则返回false
    bool intergal = true;
    std::cout << "\033[33m-- [Checking project intergal]\n\033[0m";
    std::cout << "\033[33m-- Reading config ...\n\033[0m";
    readConfig();
    std::cout << "\033[33m-- Finish\n\033[0m";
    std::cout << "\033[33m-- Checking CMakeLists.txt ...\n\033[0m";
    std::string cmakelistsPath =
        projectPath + "/" + currentProject + "/CMakeLists.txt";
    if (!std::filesystem::exists(cmakelistsPath)) {
        std::cerr << "Can't find the \"CMakeLists.txt\" in current project "
                     "directory, would you want to create one? [y/n]\n";
        if (getchar() == 'y')
            initCMakeLists();
        else
            return false;
    }
    std::cout << "\033[33m-- Finish\n\033[0m";
    std::cout << "\033[33m-- Checking project's source files ...\n\033[0m";
    std::fstream fs;
    fs.open(cmakelistsPath, std::ios::in);
    std::string buffer;
    while (1) {
        fs >> buffer;
        if (buffer.find("add_executable", 0) != -1)
            break;
    }

    while (fs >> buffer) {
        if (buffer.substr(buffer.size() - 1, 1) == ")") {
            buffer.erase(buffer.size() - 1, 1);
            if (!std::filesystem::exists(projectPath + "/" + currentProject +
                                         "/" + buffer)) {
                std::cerr
                    << buffer
                    << "\033[31m in the CMakeLists.txt is not exist\n\033[0m";
                intergal = false;
            }
            break;
        }
        if (!std::filesystem::exists(projectPath + "/" + currentProject + "/" +
                                     buffer)) {
            std::cerr << buffer
                      << "\033[31m in the CMakeLists.txt not exist\n\033[0m";
            intergal = false;
        }
    }
    std::cout << "\033[33m-- Finish\n\033[0m";
    return intergal;
}

void ProjectManager::
    checkProjectDirectory() { // 检查项目文件夹与其中的build文件夹是否存在并提供创建选项
    readConfig();
    if (!std::filesystem::exists(projectPath + "/" + currentProject)) {
        std::cout << "The current project directory is not exists.\n";
        std::cout << "Are you want to create it? [y/n]\n";
        char select = getchar();
        if (select == 'y' || select == 'Y')
            createProject();
    } else {
        if (!std::filesystem::exists(projectPath + "/" + currentProject +
                                     "/build")) {
            std::cout
                << "The current project's build directory is not exists.\n";
            std::cout << "Are you want to create one? [y/n]\n";
            char select = getchar();
            if (select == 'y' || select == 'Y') {
                initBuild();
            }
        }
    }
}

void ProjectManager::createProject() {
    readConfig();
    initDirectory();
    initMain();
    initCMakeLists();
    initBuild();
    checkProjectDirectory();
    checkProject();
}

void ProjectManager::list() const {
    std::cout << "\033[33mCurrent path:\t " << projectPath << "\n";
    std::cout << "Current Project: " << currentProject << "\n\033[0m";
    std::string cmd = "cd " + projectPath + "; ls -I tempList >> ./tempList";
    system(cmd.c_str());
    std::fstream fs;
    fs.open(projectPath + "/tempList", std::ios::in);
    std::cout << "\033[1mProjects:\n\033[0m";
    std::string tempProject;
    auto index = 1;
    while (fs >> tempProject) {
        std::cout << index << ". \033[32m\033[1m" << tempProject << "\n\033[0m";
        index++;
    }
    cmd = "cd " + projectPath + "; rm ./tempList";
    system(cmd.c_str());
}

void ProjectManager::addClass(std::string projectName, std::string className) {
    if (std::isdigit(className.front())) {
        std::cerr << "\033[31m\033[1mClass name can't begin with a "
                     "digit!\n\033[0mPlease try again.\n";
    } else {
        std::string classPathH =
            projectPath + "/" + currentProject + "/" + className + ".h";
        std::string classPathCpp =
            projectPath + "/" + currentProject + "/" + className + ".cpp";
        if (std::filesystem::exists(classPathH) ||
            std::filesystem::exists(classPathCpp)) { // 判断类的文件是否已经存在
            std::cerr << "File is exist!\nAdd class " + className +
                             " failed!\n";
        } else {
            // 修改CMakeLists
            std::string cmakelistsPath =
                projectPath + "/" + currentProject + "/CMakeLists.txt";
            std::string buffer = "", i = "";
            std::fstream ofs;
            ofs.open(cmakelistsPath, std::ios::in);
            while (ofs >> i) {
                if (i.back() == ')') {
                    i += "\n";
                } else {
                    i += " ";
                }

                buffer += i;
            }
            ofs.close();
            std::string rep = className + ".h " + className + ".cpp main.cpp";
            buffer.replace(buffer.find("main.cpp", 0), 8, rep);
            std::filesystem::remove(cmakelistsPath);
            // std::cout << "buffer:\n" << buffer << std::endl;
            ofs.open(cmakelistsPath, std::ios::out);
            ofs << buffer;
            ofs.close();

            // 添加.h .cpp
            // .h
            ofs.open(classPathH, std::ios::out);
            std::transform(className.begin(), className.end(),
                           className.begin(), ::toupper);
            std::string classH =
                "#ifndef " + className + "_H_\n#define " + className + "_H_\n";
            std::transform(className.begin(), className.end(),
                           className.begin(), ::tolower);
            classH +=
                "class " + className + " {\n  private:\n\npublic:\n\n};\n";
            std::transform(className.begin(), className.end(),
                           className.begin(), ::toupper);
            classH += "#endif /*" + className + "*/";

            ofs << classH;
            ofs.close();
            // .cpp
            std::transform(className.begin(), className.end(),
                           className.begin(),
                           ::tolower); // 再转成小写
            ofs.open(classPathCpp, std::ios::out);
            ofs << "#include \"" + className + ".h\"\n";
            ofs.close();
        }
        buildWithoutRun();
        run();
    }
}

void ProjectManager::initMain() {
    std::fstream ofs;
    std::string mainPath = projectPath + "/" + currentProject + "/main.cpp";
    ofs.open(mainPath, std::ios::out);
    if (ofs.is_open()) {
        ofs << "#include <iostream>\n"
               "int main(int argc, char *argv[]) {\n"
               "    std::cout << \"Hello, world!\" << std::endl;\n"
               "    return 0;\n"
               "}";
        ofs.close();
    } else {
        std::cout << "craete main.cpp failed!\n";
    }
}

void ProjectManager::initCMakeLists() {
    std::string cmakelistsPath =
        projectPath + "/" + currentProject + "/CMakeLists.txt";
    std::ofstream ofs;
    ofs.open(cmakelistsPath, std::ios::out);
    ofs << "cmake_minimum_required(VERSION 3.0.0)\n"
           "project("
        << currentProject
        << " VERSION 0.1.0)\n"
           "set(CMAKE_CXX_STANDARD 20)\n"
           "include(CTest)\n"
           "enable_testing()\n"
           "set(CMAKE_EXPORT_COMPILE_COMMANDS ON)\n"
           "set(CMAKE_BUILD_TYPE Debug)\n"
           "include_directories(includes)\n"
           "add_executable("
        << currentProject
        << " main.cpp)\n"
           "set(CPACK_PROJECT_NAME ${PROJECT_NAME})\n"
           "set(CPACK_PROJECT_VERSION ${PROJECT_VERSION}) \n"
           "include(CPack)\n";
    ofs.close();
}

void ProjectManager::readConfig() {
    std::string configBuffer = "";
    if (!std::filesystem::exists(
            configFile)) { // Config not exist, and create one
        std::system("touch ~/.dionysen_conf");
        std::ofstream ofs;
        ofs.open(configFile, std::ios::out);
        if (ofs.is_open()) {
            ofs << "project-path: " << projectPath << std::endl;
            ofs << "projecy-name: " << currentProject << std::endl;
            ofs.close();
        } else {
            std::cout << "create config failed\n";
        }
    } else {
        std::ifstream
            ifs; // 读取文件时是空格相隔，一次读一段，因此可以通过判断关键词来判断是否为需要的信息
        ifs.open(configFile, std::ios::in);
        if (ifs.is_open()) {
            while (ifs >> configBuffer) {
                if (configBuffer == "project-path:") {
                    ifs >> projectPath;
                } else if (configBuffer == "project-name:") {
                    ifs >> currentProject;
                }
            }
            ifs.close();
        } else {
            std::cout << "Open config failed\n";
        }
    }
}

void ProjectManager::initDirectory() {
    std::string projectDirectory = projectPath + "/" + currentProject;
    if (std::filesystem::exists(projectDirectory)) {
        if (std::filesystem::is_directory(projectDirectory)) {
            std::cout << "\033[31mCreate project failed, the project named "
                      << currentProject << " is exist!\n\033[0m";
            return;
        }
    } else {
        std::filesystem::create_directory(projectDirectory);
    }
}

void ProjectManager::initBuild() {
    std::string buildDirectory = projectPath + "/" + currentProject + "/build";
    if (std::filesystem::exists(buildDirectory)) {
        if (std::filesystem::is_directory(buildDirectory)) {
            std::cout << "\033[33mThe build directory is exist!\n\033[0m";
        }
    } else {
        std::filesystem::create_directory(buildDirectory);
    }
    std::cout << "\033[33m-- Execuating the first build ... \n\033[0m";
    std::string cmd = "cd " + projectPath + "/" + currentProject +
                      "/build; cmake ..; make ; ./" + currentProject;
    system(cmd.c_str());
    std::cout << "\033[33m-- Finish! \n\033[0m";
}

void ProjectManager::setCurrentProject(std::string projectName) {
    std::fstream ofs;
    ofs.open(configFile, std::ios::trunc | std::ios::out);
    ofs << "project-path: " << projectPath << std::endl;
    ofs << "project-name: " << projectName << std::endl;
    ofs.close();
    readConfig();
}

void ProjectManager::setDefaultPath(std::string projectPath) {
    if (projectPath.substr(1, 1) == "~") {
        std::string homePath = getenv("HOME");
        projectPath.replace(1, 1, homePath);
    }
    std::fstream ofs;
    ofs.open(configFile, std::ios::trunc | std::ios::out);
    ofs << "project-path: " << projectPath << std::endl;
    ofs << "project-name: " << currentProject << std::endl;
    ofs.close();
    readConfig();
}

void ProjectManager::delProject(std::string projectName) {
    std::cout << "\033[33mDelete the project " + projectName +
                     " actually is moving the project to trash. \nIf you want "
                     "to delete "
                     "it "
                     "permanently, please remove the content in "
                     "\".dion_trash\".\n\033[0m";
    std::cout << "\033[1mAre you want to go on? [y/n]\n\033[0m";
    char key = getchar();
    if (key == 'y' || key == 'Y') {
        std::string cmd =
            "cd " + projectPath + "; mv -f " + projectName + " ./.dion_trash";
        system(cmd.c_str());
    }
}

void ProjectManager::buildWithoutRun() {
    std::string cmd =
        "cd " + projectPath + "/" + currentProject + "/build; cmake ..; make";
    system(cmd.c_str());
}

void ProjectManager::make() {
    std::string cmd =
        "cd " + projectPath + "/" + currentProject + "/build; make";
    system(cmd.c_str());
}

void ProjectManager::run() {
    std::string cmd = "cd " + projectPath + "/" + currentProject +
                      "/build; clear; ./" + currentProject;
    system(cmd.c_str());
}
void ProjectManager::clean() {
    std::string cmd =
        "cd " + projectPath + "/" + currentProject + "; rm -rf ./build/*";
    system(cmd.c_str());
}

void ProjectManager::debugBuild() {
    std::string cmd = "cd " + projectPath + "/" + currentProject +
                      "/build; cmake .. -DCMAKE_BUILD_TYPE=Debug; cmake "
                      "--build . --config Debug";
    system(cmd.c_str());
}

void ProjectManager::showHelp() {
    std::cout
        << "Usage: pm [OPTION]...\n"
        << "Manage your projects.\nThere are some short options to use:\n"
        << "-l, --list\tlist the current project path and current project\n"
        << "-c, --create\tcreate a project with name you give\n"
        << "-d, --delproj\tdelete a project\n"
        << "-a, --addclass\tadd a cpp class to the current project\n"
        << "-D, --delclass\tdelete a class of the current project\n"
        << "-b\tbuild the current project\n"
        << "-r\trun the current project\n"
        << "-m, --make\tbuild the project with make\n"
        << "-s, --setproj\tset te current project\n"
        << "-S, --setpath\tset the default project path\n"
        << "-h, --help\tdisplay this help and exit\n";
}

void ProjectManager::delClass(std::string className) {
    bool classIsExist = false;
    std::string cmakelistsPath =
        projectPath + "/" + currentProject + "/CMakeLists.txt";
    std::fstream fs;
    if (!std::filesystem::exists(cmakelistsPath)) {
        std::cerr << "CMakeLists.txt is not exist\n";
        return;
    }
    fs.open(cmakelistsPath, std::ios::in);
    std::string buffer;
    while (1) {
        fs >> buffer;
        if (buffer.find("add_executable", 0) != -1) // 先找到add_executable
            break;
    }
    while (fs >> buffer) { // 此时再进行读写，会读取到.h or .cpp 文件
        if (buffer.substr(buffer.size() - 1, 1) == ")") {
            buffer.erase(buffer.size() - 1, 1);
            if (buffer.find(className, 0))
                classIsExist = true;
            break;
        } else {
            if (buffer.find(className, 0))
                classIsExist = true;
        }
    }
    if (!classIsExist) { // 如果指定的类不存在，
        std::cerr << "The class named " << className << " is not exist!\n";
        return;
    }
    if (checkProject()) {
        std::string classPathH =
            projectPath + "/" + currentProject + "/" + className + ".h";
        std::string classPathCpp =
            projectPath + "/" + currentProject + "/" + className + ".cpp";
        if (std::filesystem::exists(classPathH))
            std::filesystem::remove(classPathH);
        else
            std::cerr << "File " << className << ".h is not exist.\n";
        if (std::filesystem::exists(classPathCpp))
            std::filesystem::remove(classPathCpp);
        else
            std::cerr << "File " << className << ".cpp is not exist.\n";

        // 修改CMakeLists
        std::string cmakelistsPath =
            projectPath + "/" + currentProject + "/CMakeLists.txt";
        std::string buffer = "", i = "";
        std::fstream ofs;
        ofs.open(cmakelistsPath, std::ios::in);
        while (ofs >> i) {
            if (i.back() == ')') {
                i += "\n";
            } else {
                i += " ";
            }
            buffer += i;
        }
        ofs.close();
        buffer.erase(buffer.find(className + ".h", 0), className.size() + 3);
        buffer.erase(buffer.find(className + ".cpp", 0), className.size() + 5);
        std::filesystem::remove(cmakelistsPath);
        // std::cout << "buffer:\n" << buffer << std::endl;
        ofs.open(cmakelistsPath, std::ios::out);
        ofs << buffer;
        ofs.close();
    } else {
        std::cerr << "Project integrity is compromised!\nPlease check it "
                     "mannually!\n";
    }
}

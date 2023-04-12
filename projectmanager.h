#ifndef _PROJECTMANAGER_H_
#define _PROJECTMANAGER_H_

#pragma once
#include <filesystem>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <string>
#include <unistd.h>

class ProjectManager {
  private:
    std::string configFile;
    std::string configBuffer;

  public:
    std::string projectPath; // 指工程文件夹所在的目录
    std::string currentProject;
    ProjectManager();
    void readConfig();
    bool checkProject();
    void checkProjectDirectory();
    // 以下为创建项目时所用函数的封装：
    void initCMakeLists();
    void initMain();
    void initDirectory();
    void initBuild();

    // 以下为命令行方法：
    void setDefaultPath(std::string projectPath);
    void setCurrentProject(std::string projectName);
    void delProject(std::string projectName);
    void createProject();
    void list() const;
    void buildWithoutRun();
    void make();
    void run();
    void addClass(std::string projectName, std::string className);
    void showHelp();
    void delClass(std::string className);
    void clean();
    void debugBuild();
};

#endif // _PROJECTMANAGER_H_

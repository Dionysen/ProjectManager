#include <iostream>
#include <ostream>

#include "projectmanager.h"

int main(int argc, char *argv[]) {
    ProjectManager pm;
    int opt, option_index = 0;
    option long_options[] = {
        {"create", 1, nullptr, 'c'},  {"addclass", 1, nullptr, 'a'},
        {"setproj", 1, 0, 's'},       {"delproj", 1, nullptr, 'd'},
        {"list", 0, nullptr, 'l'},    {"build", 0, nullptr, 'b'},
        {"run", 0, nullptr, 'r'},     {"help", 0, nullptr, 'h'},
        {"setpath", 1, nullptr, 'S'}, {"delclass", 1, nullptr, 'D'},
        {"make", 0, nullptr, 'm'},    {"clean", 0, nullptr, 'C'},
        {"debug", 0, nullptr, 1}};
    while ((opt = getopt_long(argc, argv, "lbrma:c:hd:s:D:S:C", long_options,
                              &option_index)) != -1) {
        switch (opt) {
        case 'l':
            pm.list();
            break;
        case 'c':
            pm.setCurrentProject(optarg);
            pm.createProject();
            break;
        case 'a':
            pm.addClass(pm.currentProject, optarg);
            break;
        case 'b':
            pm.buildWithoutRun();
            break;
        case 'r':
            pm.run();
            break;
        case 'm':
            pm.make();
            break;
        case 'd':
            pm.delProject(optarg);
            break;
        case 'h':
            pm.showHelp();
            break;
        case 's':
            pm.setCurrentProject(optarg);
            break;
        case 'S':
            pm.setDefaultPath(optarg);
            break;
        case 'D':
            pm.delClass(optarg);
            break;
        case 'C':
            pm.clean();
            break;
        case 1:
            pm.debugBuild();
            break;
        }
    }
    return 0;
}

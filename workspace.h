//
// Created by Pixel_Fish on 2026/1/30.
//

#ifndef WORKING_WORKSPACE_H
#define WORKING_WORKSPACE_H

#include <cmath>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <utility>
#include <vector>
#include <queue>
#include <iomanip>
#include <string>
#include <map>
#include <stack>
#include <set>
#include <bitset>
#include <random>
#include <unordered_map>
#include <fstream>
#include <OpenXLSX.hpp>

class SongManager {
    public:
        explicit SongManager(std::string str, std::string hard, std::string chaos, std::string glitch = "");
        SongManager() = default;
        ~SongManager() = default;
        void set(const std::string &str,const std::string &hard,const std::string &chaos,const std::string &glitch = "");
        std::string GetSongName() const;
        std::string GetHardNum() const;
        std::string GetChaosNum() const;
        std::string GetGlitchNum() const;
    private:
        std::string name;
        std::string Hard, Chaos, Glitch;
};

class Task {
public:
    explicit Task(std::string str, std::string dif, std::string name_);
    Task() = default;
    ~Task() = default;
    void set(const std::string &str,const std::string &dif, const std::string &name);
    std::string GetName();
    std::string GetDifficulty();
    std::string GetTaskName();
private:
    std::string name;
    std::string difficulty;
    std::string task_name;
};


class TaskPool {
    public:
        void AddItem(const std::string &name, int weight);
        std::string Draw();
    private:
        std::vector<std::pair<std::string, int>> items;
        std::vector<int> weights;

};

std::string forceToString(const OpenXLSX::XLCellValue& val);

std::vector<SongManager> GetRandomSongs(const std::vector<SongManager> &source, int count);

void SaveToExcel(const std::vector<SongManager> &source, const std::string &OutPutFileName);

void SaveToJson(const std::string &InPutFileName, const std::string &OutPutFileName);

int Random_Number(const int &L, const int &R);

#endif //WORKING_WORKSPACE_H
//
// Created by Pixel_Fish on 2026/1/31.
//
#include "workspace.h"

using std::cin;
using std::cout;
using std::endl;
using namespace OpenXLSX;

SongManager::SongManager(std::string str, std::string hard, std::string chaos, std::string glitch) : name(std::move(str)), Hard(std::move(hard)), Chaos(std::move(chaos)), Glitch(std::move(glitch))
{

}

std::string SongManager::GetSongName() const {
    return name;
}

std::string SongManager::GetHardNum() const {
    return Hard;
}

std::string SongManager::GetChaosNum() const {
    return Chaos;
}

std::string SongManager::GetGlitchNum() const {
    return Glitch;
}

void SongManager::set(const std::string &str,const std::string &hard,const std::string &chaos,const std::string &glitch) {
    name = str;
    Hard = hard;
    Chaos = chaos;
    Glitch = glitch;
}

Task::Task(std::string str, std::string dif, std::string name_) : name(std::move(str)), difficulty(std::move(dif)), task_name(std::move(name_)) {

}

void Task::set(const std::string &str,const std::string &dif, const std::string &name_) {
    name = str;
    difficulty = dif;
    task_name = name_;
}

std::string Task::GetName() {
    return name;
}

std::string Task::GetDifficulty() {
    return difficulty;
}

std::string Task::GetTaskName() {
    return task_name;
}


std::string forceToString(const OpenXLSX::XLCellValue& val) {
    switch (val.type()) {
        case OpenXLSX::XLValueType::String:
            return val.get<std::string>();
        case OpenXLSX::XLValueType::Integer:
            return std::to_string(val.get<long long>());
        case OpenXLSX::XLValueType::Float:
            return std::to_string(val.get<double>());
        case OpenXLSX::XLValueType::Boolean:
            return val.get<bool>() ? "TRUE" : "FALSE";
        case OpenXLSX::XLValueType::Empty:
            return "";
        default:
            return "UNKNOWN";
    }
}

void TaskPool::AddItem(const std::string &name, int weight) {
    items.emplace_back(name, weight);
    weights.push_back(weight);
}

std::string TaskPool::Draw() {
    if (!items.empty()) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::discrete_distribution<> d(weights.begin(), weights.end());
        int index = d(gen);
        return items[index].first;
    }
    return "";
}

std::vector<SongManager> GetRandomSongs(const std::vector<SongManager>& source, int count) {
    size_t N = std::min(static_cast<size_t>(count), source.size());

    std::vector<SongManager> result;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::sample(source.begin(), source.end(), std::back_inserter(result), N, gen);

    return result;
}

int Random_Number(const int &L, const int &R) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(L, R);

    int randomNum = dis(gen);
    return randomNum;
}

void SaveToExcel(const std::vector<SongManager> &source, const std::string &OutPutFileName) {
    OpenXLSX::XLDocument doc;

    try {
        doc.create(OutPutFileName, true);
        auto Sheet = doc.workbook().worksheet("Sheet1");

        Sheet.column(1).setWidth(30);
        Sheet.column(2).setWidth(15);
        Sheet.column(3).setWidth(15);
        Sheet.column(4).setWidth(15);

        Sheet.cell(1, 1).value() = "歌曲名称";
        Sheet.cell(1, 2).value() = "Hard难度";
        Sheet.cell(1, 3).value() = "Chaos难度";
        Sheet.cell(1, 4).value() = "Glitch难度";

        auto rowCount = static_cast<uint32_t>(source.size() + 1);


        for (uint32_t r = 2; r <= rowCount; r ++) {
            Sheet.cell(r, 1).value() = source[r - 2].GetSongName();
            Sheet.cell(r, 2).value() = source[r - 2].GetHardNum();
            Sheet.cell(r, 3).value() = source[r - 2].GetChaosNum();
            Sheet.cell(r, 4).value() = source[r - 2].GetGlitchNum();
        }

        doc.save();
        doc.close();
        cout << "The file has been successfully exported to " << OutPutFileName << endl;

    } catch (const std::exception & e) {
        cout << "error: " << e.what() << endl;
    }
}

void SaveToJson(const std::string &InPutFileName, const std::string &OutPutFileName) {

    std::vector<Task> result;
    std::vector<SongManager> arr;

    try {
        XLDocument doc;
        const std::string &filename = InPutFileName;
        doc.open(filename);

        cout << "filename: " << filename << endl;

        auto Sheet_Name = doc.workbook().worksheetNames()[0];
        auto Sheet = doc.workbook().worksheet(Sheet_Name);

        uint32_t rowCount = Sheet.rowCount();
        uint32_t colCount = Sheet.columnCount();

        for (uint32_t r = 2; r <= rowCount; r ++) {

            std::string SongName, h_, c_, g_; // Hard, Chaos, Glitch
            for (uint32_t c = 1; c <= colCount; c ++) {
                auto cell = Sheet.cell(r, c);
                const auto & val = cell.value();

                if (val.type() == XLValueType::String || val.type() == XLValueType::Integer || val.type() == XLValueType::Float) {
                    if (c == 1) SongName = forceToString(val);
                    else if (c == 2) h_ = forceToString(val);
                    else if (c == 3) c_ = forceToString(val);
                    else if (c == 4) g_ = forceToString(val);
                }
            }
            if (!SongName.empty()) {
                SongManager temp(SongName, h_,c_, g_);
                arr.push_back(temp);
            }
            else {
                break;
            }
        }
    } catch (const std::exception & e) {
        cout << " error :" << e.what() << endl;
    }

    if (arr.size() != 14) {
        cout << "Error : The number of songs is incorrect !" << endl;
        return;
    }

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(arr.begin(),arr.end(), g);

    TaskPool Easy, Hard, Special;

    /*
     * 这是对于hard难度而言的任务池子
     */
    std::vector<std::string> tasks = {"达成A评价", "达成S评价", "达成FULL COMBO", "达成Million Master", "达成90tp以上", "达成93tp以上", "达成95tp以上", "达成97tp以上", "达成99tp以上"};
    std::vector<std::string> Special_tasks = {
        "Good， Bad， Miss 三组数字构成等比或等差数列，且公比或公差不为0",
        "结算分数有三个非0连续相同的数字",
        "Good数量 > bad + miss数量",
        "得分前两位数字与tp的前两位数字相同",
        "最大Combo数大于 难度整数位 * 50",
        "达成S评价并且good = miss = bad",
        "使得分数的每位数字从左到右依次不上升，分数不能为0或1000000；",
        "最大连击数114且tp不低于97",
        "恰好有good = 5，bad = 1，miss = 4",
        "分数里包含2026，0217，0514中的一个"
    };

    std::vector<int> weight1 = {15, 20, 10, 5, 10, 10, 10, 10, 10};
    std::vector<int> weight2 = {250, 150, 50, 50, 100, 100, 200, 75, 25};

    for (int i = 0; i < tasks.size(); i ++) {
        Easy.AddItem(tasks[i], weight1[i]);
    }

    for (int i = 0; i < tasks.size(); i ++) {
        Hard.AddItem(tasks[i], weight2[i]);
    }

    for (const auto &x : Special_tasks) {
        Special.AddItem(x, 1);
    }


    constexpr int MAXL = 11;
    for (int i = 0; i < MAXL; i ++) {
        result.emplace_back(arr[i].GetSongName(), "Hard" + arr[i].GetHardNum() , Easy.Draw());
        if (!arr[i].GetGlitchNum().empty()) {
            if (Random_Number(1, 100) <= 75) {
                result.emplace_back(arr[i].GetSongName(), "Chaos" + arr[i].GetChaosNum() , Hard.Draw());
            }
            else {
                result.emplace_back(arr[i].GetSongName(), "Glitch" + arr[i].GetGlitchNum(), Hard.Draw());
            }
        }
        else result.emplace_back(arr[i].GetSongName(), "Chaos" + arr[i].GetChaosNum(), Hard.Draw());
    }

    for (int i = 0; i < 3; i ++) {
        int next = MAXL + i;
        result.emplace_back(arr[next].GetSongName(), "任意", Special.Draw());
    }

    std::shuffle(result.begin(),result.end(), g);

    std::ofstream outFile("bingo.txt");

    if (outFile.is_open()) {
        outFile << "[" << endl;
        uint32_t tot = result.size(), cnt = 1;
        for (auto x : result) {
            outFile << "    {\"name\":";
            outFile << "\"";
            outFile << "在 " << x.GetName() << " 的 " << x.GetDifficulty() << " 难度中 " << x.GetTaskName();
            outFile <<"\"}";
            if (cnt < tot) outFile << ",";
            outFile << endl;
            cnt ++;
        }
        outFile << "]" << endl;
        outFile.close();
        cout << "The file has been successfully saved!!!" << endl;
    }
    else {
        cout << "Error : File saving failed!" << endl;
    }

}


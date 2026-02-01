#include "workspace.h"

using namespace OpenXLSX;
using std::cin;
using std::cout;
using std::endl;
void Main_Menu();

void reload(std::vector<SongManager> & arr) {
    try {
        XLDocument doc;
        std::string filename = "song_package.xlsx";
        doc.open(filename);

        cout << "Judging Assistance System ver0.1" << endl;

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
    cout << "Successfully loaded all songs !!!" << endl;
}

bool Main_Menu(const std::vector<SongManager> &arr) {
    int operation = 0;

    cout << "Input 1 to export the random song table" << endl;
    cout << "Input 2 to generate the JSON text for bingo" << endl;
    cout << "Input 3 to exit the program" << endl;

    cin >> operation;

    if (operation == 1) {
        std::vector<SongManager> RandomSong = GetRandomSongs(arr, 16);
        SaveToExcel(RandomSong, "SelectedSongs.xlsx");;
        return true;
    }
    else if (operation == 2) {
        SaveToJson("SelectedSongs.xlsx", "");
        return true;
    }
    return false;
}
int main() {

    std::vector<SongManager> AllSong;
    reload(AllSong);
    while (Main_Menu(AllSong)) {
    }
    return 0;
}
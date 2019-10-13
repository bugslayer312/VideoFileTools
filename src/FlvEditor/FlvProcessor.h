#pragma once

#include <sstream>
#include <list>

struct FlvTag;

class FlvProcessor {
public:
    FlvProcessor(std::string const& filepath, std::istream& ist);
    void Run(bool breakOnVideoTag, bool breakAtEnd);

private:
    void ReadHeader();
    void StoreUpToPos(std::ostream& ost, std::ios::pos_type pos);
    void StoreUpToEnd(std::ostream& ost);
    void SaveChanges();
    std::string AskCommand();
    void PrintCommandsHelp();
    void EditTag(FlvTag& flvTag, std::ios::pos_type beforeTagPos, std::ios::pos_type afterTagPos);
private:
    struct EditedTagStream {
        std::ios::pos_type InStartPos;
        std::ios::pos_type InEndPos;
        std::stringstream StoreStream;
    };

    std::string const& m_filepath;
    std::istream& m_ist;
    std::list<EditedTagStream> m_editedTagsData;
};

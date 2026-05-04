//
// Created by gaopeng on 2026/3/22.
//

#ifndef DATATYPECONSISTENCYCHECK_H
#define DATATYPECONSISTENCYCHECK_H

#include <common-utils/xlsx.h>
#include <set>
#include <string>
#include <unordered_set>
struct TypeInfo {
    std::string name;
    int64_t minVal;
    int64_t maxVal;
    bool isSigned;
};
class DataTypeConsistencyCheck {
    friend class ValueConsistencyCheck;

protected:
    const std::unordered_set<std::string> BASIC_TYPES = {"bool",   "uint8",  "uint16", "uint32",  "uint64",  "sint8",
                                                         "sint16", "sint32", "sint64", "float32", "float64", "\\"};
    int dataTypeNameCol;
    int categoryCol;

    int memPosiCol;
    int memNameCol;
    int memTypeRefCol;

    int lenTypeCol;
    int minLenCol;
    int maxLenCol;

    int baseTypeCol;
    int sigLenCol;
    int factorCol;
    int offsetCol;
    int minPhyCol;
    int maxPhyCol;
    int unitCol;
    int tableCol;
    std::string filePath;
    std::list<std::string> errors;

public:
    [[nodiscard]] std::list<std::string> getErrors() const { return errors; }

    void set_file_path(const std::string& file_path) { filePath = file_path; }
    explicit DataTypeConsistencyCheck();
    explicit DataTypeConsistencyCheck(std::string& filePath);
    ~DataTypeConsistencyCheck() = default;
    void readDataType();

private:
    std::set<std::string> dataTypeNames;
    void recordConsistencyCheck(const OpenXLSX::XLWorksheet& sheet, int startRow, int endRow);

    void arrayConsistencyCheck(const OpenXLSX::XLWorksheet& sheet, int row);

    void unusedCellForRefType(const OpenXLSX::XLWorksheet& sheet, int row);

    void checkDataType(const OpenXLSX::XLWorksheet& sheet);
    void valueConsistencyCheck(const OpenXLSX::XLWorksheet& sheet, int row);
    void checkMustDefinedCell(const OpenXLSX::XLWorksheet& sheet, int row);
    int checkValueTable(const std::string& valueTable, const std::string& cellAddress);
    int checkBitField(const std::string& valueTable, const std::string& cellAddress);
    void checkSignalLength(double maxValue, int len, int row);
    void checkSignalLength(int64_t maxValue, int length, const std::string& baseType, int row);
};

#endif // DATATYPECONSISTENCYCHECK_H

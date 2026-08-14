#include "bm25.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: bm25-search <text-directory> <query words...>\n";
        return 2;
    }
    const std::filesystem::path directory = argv[1];
    if (!std::filesystem::is_directory(directory)) {
        std::cerr << "Not a directory: " << directory << '\n';
        return 2;
    }
    std::ostringstream query;
    for (int index = 2; index < argc; ++index) {
        if (index > 2) query << ' ';
        query << argv[index];
    }

    unays::BM25Index search_index;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".txt") continue;
        std::ifstream input(entry.path());
        if (!input) {
            std::cerr << "Could not read " << entry.path() << '\n';
            continue;
        }
        const std::string content{std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
        search_index.add_document(std::filesystem::relative(entry.path(), directory).generic_string(), content);
    }
    if (search_index.size() == 0) {
        std::cerr << "No readable .txt files found.\n";
        return 1;
    }
    const auto results = search_index.search(query.str());
    for (const auto& result : results) {
        std::cout << std::fixed << std::setprecision(6) << result.score << '\t' << result.document_id << '\n';
    }
    return results.empty() ? 1 : 0;
}

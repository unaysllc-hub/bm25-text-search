#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace unays {

struct SearchResult {
    std::string document_id;
    double score{};
};

class BM25Index {
public:
    void add_document(std::string document_id, std::string_view text);
    [[nodiscard]] std::vector<SearchResult> search(std::string_view query, std::size_t limit = 10,
                                                   double k1 = 1.2, double b = 0.75) const;
    [[nodiscard]] std::size_t size() const noexcept { return documents_.size(); }

private:
    struct Document {
        std::string id;
        std::unordered_map<std::string, std::size_t> frequencies;
        std::size_t length{};
    };

    std::vector<Document> documents_;
    std::unordered_map<std::string, std::size_t> document_frequency_;
    std::size_t total_terms_{};
};

std::vector<std::string> tokenize(std::string_view text);

}  // namespace unays

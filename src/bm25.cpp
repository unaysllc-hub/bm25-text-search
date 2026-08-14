#include "bm25.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <unordered_set>

namespace unays {

std::vector<std::string> tokenize(std::string_view text) {
    std::vector<std::string> tokens;
    std::string current;
    for (const unsigned char character : text) {
        if (std::isalnum(character)) {
            current.push_back(static_cast<char>(std::tolower(character)));
        } else if (!current.empty()) {
            tokens.push_back(std::move(current));
            current.clear();
        }
    }
    if (!current.empty()) tokens.push_back(std::move(current));
    return tokens;
}

void BM25Index::add_document(std::string document_id, std::string_view text) {
    if (document_id.empty()) throw std::invalid_argument("document id cannot be empty");
    if (std::any_of(documents_.begin(), documents_.end(), [&](const Document& document) {
            return document.id == document_id;
        })) {
        throw std::invalid_argument("document id must be unique");
    }

    Document document{.id = std::move(document_id), .frequencies = {}, .length = 0};
    std::unordered_set<std::string> unique_terms;
    for (auto& token : tokenize(text)) {
        ++document.frequencies[token];
        unique_terms.insert(std::move(token));
        ++document.length;
    }
    for (const auto& term : unique_terms) ++document_frequency_[term];
    total_terms_ += document.length;
    documents_.push_back(std::move(document));
}

std::vector<SearchResult> BM25Index::search(std::string_view query, std::size_t limit, double k1, double b) const {
    if (k1 <= 0.0) throw std::invalid_argument("k1 must be positive");
    if (b < 0.0 || b > 1.0) throw std::invalid_argument("b must be between zero and one");
    if (documents_.empty() || limit == 0) return {};

    const auto query_tokens = tokenize(query);
    const std::unordered_set<std::string> unique_query_terms(query_tokens.begin(), query_tokens.end());
    const double document_count = static_cast<double>(documents_.size());
    const double average_length = std::max(1.0, static_cast<double>(total_terms_) / document_count);
    std::vector<SearchResult> results;

    for (const auto& document : documents_) {
        double score = 0.0;
        for (const auto& term : unique_query_terms) {
            const auto frequency = document.frequencies.find(term);
            const auto document_frequency = document_frequency_.find(term);
            if (frequency == document.frequencies.end() || document_frequency == document_frequency_.end()) continue;
            const double tf = static_cast<double>(frequency->second);
            const double df = static_cast<double>(document_frequency->second);
            const double idf = std::log(1.0 + (document_count - df + 0.5) / (df + 0.5));
            const double normalization = tf + k1 * (1.0 - b + b * static_cast<double>(document.length) / average_length);
            score += idf * (tf * (k1 + 1.0)) / normalization;
        }
        if (score > 0.0) results.push_back({document.id, score});
    }

    std::sort(results.begin(), results.end(), [](const SearchResult& left, const SearchResult& right) {
        return left.score == right.score ? left.document_id < right.document_id : left.score > right.score;
    });
    if (results.size() > limit) results.resize(limit);
    return results;
}

}  // namespace unays

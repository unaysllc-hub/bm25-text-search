#include "bm25.hpp"

#include <cassert>
#include <cmath>
#include <stdexcept>

int main() {
    unays::BM25Index index;
    index.add_document("biology.txt", "Cells form tissues and organs in living organisms.");
    index.add_document("history.txt", "Ancient cities used stone walls and trade routes.");
    index.add_document("advanced-biology.txt", "Cell membranes control transport into and out of a cell.");
    assert(index.size() == 3);

    const auto biology = index.search("cell membrane");
    assert(!biology.empty());
    assert(biology.front().document_id == "advanced-biology.txt");
    assert(biology.front().score > 0.0);

    const auto missing = index.search("quantum");
    assert(missing.empty());

    bool duplicate_rejected = false;
    try {
        index.add_document("biology.txt", "duplicate");
    } catch (const std::invalid_argument&) {
        duplicate_rejected = true;
    }
    assert(duplicate_rejected);
    return 0;
}
